#pragma once


#include "Color.h"
#include "../data.h"


//https://gist.github.com/KdotJPG/b1270127455a94ac5d19

enum noiseType_t { perlin, randomFill, spherical, cylindrical, fourDimensional };

class Noise {
public:
	Noise() {
		xOffset = random16();
		yOffset = random16();
		zOffset = random16();
		xScale = 4000;
		yScale = 4000;
	}



	void fillNoise(noiseType_t t = perlin) {
		switch (t) {
		case perlin:
			fillNoise8bit();
			break;
		case randomFill:

			break;
		case spherical:
			fillSphereicalNoise8();
			break;
		case cylindrical:
			fillCylindricalNoise8();
			break;
		case fourDimensional:

			break;
		}
	}

	uint8_t At(uint8_t x, uint8_t y) {
		if (x < SCREEN_WIDTH and y < SCREEN_WIDTH) {
			return noise[x][y];
		}
		return 0;
	}

	uint32_t xOffset;
	uint32_t yOffset;
	uint32_t zOffset;
	uint16_t xScale;
	uint16_t yScale;

	uint16_t speed = 12;

	uint8_t noise[SCREEN_WIDTH][SCREEN_WIDTH];

	uint8_t noiseSmoothing = 100;

private:
	void fillNoise8bit() {
		static uint8_t x = 0, y = 0, z = 0;
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			uint32_t ioffset = xScale * i;
			for (int j = 0; j < SCREEN_WIDTH; j++) {
				uint32_t joffset = yScale * j;

				uint8_t data = inoise16(xOffset + ioffset, yOffset + joffset, zOffset) >> 8;
				//uint8_t data = inoise8(xOffset + 20 * i, yOffset + 20 * j, zOffset);

				// The range of the inoise8 function is roughly 16-238.
				// These two operations expand those values out to roughly 0..255
				data = qsub8(data, 16);
				data = qadd8(data, scale8(data, 39));

				if (noiseSmoothing) {
					uint8_t olddata = noise[i][j];
					uint8_t newdata = scale8(olddata, noiseSmoothing) + scale8(data, 256 - noiseSmoothing);
					data = newdata;
				}

				noise[i][j] = data;
			}
		}
		//x++;
		//y--;
		//z++;
		xOffset += (speed << 8 / 15) ; // keep interesting by changing x/y offsets
		yOffset += (speed << 8 / 28);
		zOffset += speed << 7;
	}

	//returns x y z for a given theta phi at radius r
	// angles given in 8.8 notation
	void circularToCartesian(int16_t &_x, int16_t &_y, int16_t &_z,
		uint8_t theta, uint8_t phi, uint8_t rho)
	{
		// x = rho * sin phi cos theta
		// y = rho sin phi sin theta
		// if we want cylindrical, z = z
		// if we want spherical, z = rho cos phi
		// note that for our actual array, y and z are swapped
		// 16bit gives numbers that are too large with a base radius of 32768
		// if we use 8 bit sin and cos values, base radius of 128
		int8_t sinPhi = sin8(phi) - 128;
		int8_t cosPhi = cos8(phi) - 128;
		int8_t sinTheta = sin8(theta) - 128;
		int8_t cosTheta = cos8(theta) - 128;

		_x = rho * (sinPhi * cosTheta) / (128 * 4);
		_y = rho * (sinPhi * sinTheta) / (128 * 4);
		_z = rho * cosPhi; // offset to keep returns positive
	}

	void fillCylindricalNoise8() {
		uint8_t dataSmoothing = 50;
		if (speed < 50) {
			dataSmoothing = 200 - (speed * 4);
		}
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				

				uint16_t steps = (65536 / SCREEN_HEIGHT); // = 8 for 32, or 4 for 64

				uint16_t lon = steps * j;
				uint16_t lat = steps * i / 2;

				int16_t x, y, z;

				const uint8_t rho = 1;
				x = rho * (sin16(lat) * cos16(lon) / 256); //diameter of 65536 / 256
				z = rho * (sin16(lat) * sin16(lon) / 256);
				y = (i * 256 / SCREEN_WIDTH) - 128;  // scale from -128 to 128 to match x,z

				uint8_t data;
				data = inoise16(x + xOffset, y + yOffset, z + zOffset) >> 8;
				data = qsub8(data, 16);
				data = qadd8(data, scale8(data, 39));

				if (dataSmoothing) {
					uint8_t olddata = noise[i][j];
					uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
					data = newdata;
				}
				noise[i][j] = data;
			}
		}
		xOffset += speed / 16; // keep interesting by changing x/y offsets
		yOffset += speed / 30;
		zOffset += speed;
	}

	void fillSphereicalNoise8() {
		uint8_t scale = 50;
		uint8_t dataSmoothing = 0;
		if (speed < 50) {
			dataSmoothing = 200 - (speed * 4);
		}
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				// Our circular to cartesian conversion presumes z is height, but we want y as height
				// therefore theta gives us our x and z position
				// and phi gives us our y
				// (https://math.oregonstate.edu/home/programs/undergrad/CalculusQuestStudyGuides/vcalc/coord/coord.html)
				// both should go in a complete circle, the start doesn't really matter for theta
				// But phi should go from -scale to scale (*Amp)
				// therefore we want cos(lat) = 1 when j = 0, cos(lat) = -1 when j = 32*step / 2 = 128, only want half the range
				//      uint8_t rho = *scale; // set rho to scale
				uint8_t rho = scale;
				uint8_t steps = (256 / SCREEN_HEIGHT); // = 8 for 32, or 4 for 64
				// increase steps slightly for phi so we don't have all leads at the top lighting together
				// of note maybe we should only do +/-90 deg as with the world map to avoid warping at high lattitudes
				//uint8_t steps2 = 3;
				uint8_t lon = steps * j;
				uint8_t lat = steps * i / 2;

				int16_t _x, _y, _z;
				// spherical (y = r * cos(phi))
				circularToCartesian(_x, _z, _y, lon, lat, rho); // swap y and z, as we want y not z to be height

				// cylindrical (y = y)
				//if (buttons[5]) {
				//	uint8_t yScale = 256 / SCREEN_WIDTH;
				//	_y = (i * yScale) - 128; // therefore y = i * *scale, scale from -128 to 128 to match x,z
				//}

				uint8_t data;
				data = inoise16((_x << 8) + xOffset, (_y << 8) + yOffset, (_z << 8) + zOffset) >> 8;
				data = qsub8(data, 16);
				data = qadd8(data, scale8(data, 39));


				if (dataSmoothing) {
					uint8_t olddata = noise[i][j];
					uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
					data = newdata;
				}

				noise[i][j] = data;
			}
		}
		//inc += 0.001 * (speed);
		//offset += speed;
		//xOffset += speed / 8;
		//y -= speed / 16;

		xOffset += speed / 16; // keep interesting by changing x/y offsets
		yOffset += speed / 30;
		zOffset += speed;
	}
};


Noise noise;