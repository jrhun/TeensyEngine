#pragma once


#include "Color.h"
#include "../data.h"


int16_t inoise16_raw(uint32_t x, uint32_t y, uint32_t z)
{
	// Find the unit cube containing the point
	uint8_t X = (x >> 16) & 0xFF;
	uint8_t Y = (y >> 16) & 0xFF;
	uint8_t Z = (z >> 16) & 0xFF;

	// Hash cube corner coordinates
	uint8_t A = P(X) + Y;
	uint8_t AA = P(A) + Z;
	uint8_t AB = P(A + 1) + Z;
	uint8_t B = P(X + 1) + Y;
	uint8_t BA = P(B) + Z;
	uint8_t BB = P(B + 1) + Z;

	// Get the relative position of the point in the cube
	uint16_t u = x & 0xFFFF;
	uint16_t v = y & 0xFFFF;
	uint16_t w = z & 0xFFFF;

	// Get a signed version of the above for the grad function
	int16_t xx = (u >> 1) & 0x7FFF;
	int16_t yy = (v >> 1) & 0x7FFF;
	int16_t zz = (w >> 1) & 0x7FFF;
	uint16_t N = 0x8000L;

	u = EASE16(u); v = EASE16(v); w = EASE16(w);

	// skip the log fade adjustment for the moment, otherwise here we would
	// adjust fade values for u,v,w
	int16_t X1 = LERP(grad16(P(AA), xx, yy, zz), grad16(P(BA), xx - N, yy, zz), u);
	int16_t X2 = LERP(grad16(P(AB), xx, yy - N, zz), grad16(P(BB), xx - N, yy - N, zz), u);
	int16_t X3 = LERP(grad16(P(AA + 1), xx, yy, zz - N), grad16(P(BA + 1), xx - N, yy, zz - N), u);
	int16_t X4 = LERP(grad16(P(AB + 1), xx, yy - N, zz - N), grad16(P(BB + 1), xx - N, yy - N, zz - N), u);

	int16_t Y1 = LERP(X1, X2, v);
	int16_t Y2 = LERP(X3, X4, v);

	int16_t ans = LERP(Y1, Y2, w);

	return ans;
}

uint16_t inoise16(uint32_t x, uint32_t y, uint32_t z) {
	int32_t ans = inoise16_raw(x, y, z);
	ans = ans + 19052L;
	uint32_t pan = ans;
	pan *= 440L;
	return (pan >> 8);
}

int16_t inoise16_raw(uint32_t x, uint32_t y)
{
	// Find the unit cube containing the point
	uint8_t X = x >> 16;
	uint8_t Y = y >> 16;

	// Hash cube corner coordinates
	uint8_t A = P(X) + Y;
	uint8_t AA = P(A);
	uint8_t AB = P(A + 1);
	uint8_t B = P(X + 1) + Y;
	uint8_t BA = P(B);
	uint8_t BB = P(B + 1);

	// Get the relative position of the point in the cube
	uint16_t u = x & 0xFFFF;
	uint16_t v = y & 0xFFFF;

	// Get a signed version of the above for the grad function
	int16_t xx = (u >> 1) & 0x7FFF;
	int16_t yy = (v >> 1) & 0x7FFF;
	uint16_t N = 0x8000L;

	u = EASE16(u); v = EASE16(v);

	int16_t X1 = LERP(grad16(P(AA), xx, yy), grad16(P(BA), xx - N, yy), u);
	int16_t X2 = LERP(grad16(P(AB), xx, yy - N), grad16(P(BB), xx - N, yy - N), u);

	int16_t ans = LERP(X1, X2, v);

	return ans;
}

uint16_t inoise16(uint32_t x, uint32_t y) {
	int32_t ans = inoise16_raw(x, y);
	ans = ans + 17308L;
	uint32_t pan = ans;
	pan *= 484L;
	return (pan >> 8);
}

int16_t inoise16_raw(uint32_t x)
{
	// Find the unit cube containing the point
	uint8_t X = x >> 16;

	// Hash cube corner coordinates
	uint8_t A = P(X);
	uint8_t AA = P(A);
	uint8_t B = P(X + 1);
	uint8_t BA = P(B);

	// Get the relative position of the point in the cube
	uint16_t u = x & 0xFFFF;

	// Get a signed version of the above for the grad function
	int16_t xx = (u >> 1) & 0x7FFF;
	uint16_t N = 0x8000L;

	u = EASE16(u);

	int16_t ans = LERP(grad16(P(AA), xx), grad16(P(BA), xx - N), u);

	return ans;
}

uint16_t inoise16(uint32_t x) {
	return ((uint32_t)((int32_t)inoise16_raw(x) + 17308L)) << 1;
}

int8_t inoise8_raw(uint16_t x, uint16_t y, uint16_t z)
{
	// Find the unit cube containing the point
	uint8_t X = x >> 8;
	uint8_t Y = y >> 8;
	uint8_t Z = z >> 8;

	// Hash cube corner coordinates
	uint8_t A = P(X) + Y;
	uint8_t AA = P(A) + Z;
	uint8_t AB = P(A + 1) + Z;
	uint8_t B = P(X + 1) + Y;
	uint8_t BA = P(B) + Z;
	uint8_t BB = P(B + 1) + Z;

	// Get the relative position of the point in the cube
	uint8_t u = x;
	uint8_t v = y;
	uint8_t w = z;

	// Get a signed version of the above for the grad function
	int8_t xx = ((uint8_t)(x) >> 1) & 0x7F;
	int8_t yy = ((uint8_t)(y) >> 1) & 0x7F;
	int8_t zz = ((uint8_t)(z) >> 1) & 0x7F;
	uint8_t N = 0x80;

	u = EASE8(u); v = EASE8(v); w = EASE8(w);

	int8_t X1 = lerp7by8(grad8(P(AA), xx, yy, zz), grad8(P(BA), xx - N, yy, zz), u);
	int8_t X2 = lerp7by8(grad8(P(AB), xx, yy - N, zz), grad8(P(BB), xx - N, yy - N, zz), u);
	int8_t X3 = lerp7by8(grad8(P(AA + 1), xx, yy, zz - N), grad8(P(BA + 1), xx - N, yy, zz - N), u);
	int8_t X4 = lerp7by8(grad8(P(AB + 1), xx, yy - N, zz - N), grad8(P(BB + 1), xx - N, yy - N, zz - N), u);

	int8_t Y1 = lerp7by8(X1, X2, v);
	int8_t Y2 = lerp7by8(X3, X4, v);

	int8_t ans = lerp7by8(Y1, Y2, w);

	return ans;
}

uint8_t inoise8(uint16_t x, uint16_t y, uint16_t z) {
	//  return scale8(76+(inoise8_raw(x,y,z)),215)<<1;
	int8_t n = inoise8_raw(x, y, z);  // -64..+64
	n += 64;                            //   0..128
	uint8_t ans = qadd8(n, n);        //   0..255
	return ans;
}

int8_t inoise8_raw(uint16_t x, uint16_t y)
{
	// Find the unit cube containing the point
	uint8_t X = x >> 8;
	uint8_t Y = y >> 8;

	// Hash cube corner coordinates
	uint8_t A = P(X) + Y;
	uint8_t AA = P(A);
	uint8_t AB = P(A + 1);
	uint8_t B = P(X + 1) + Y;
	uint8_t BA = P(B);
	uint8_t BB = P(B + 1);

	// Get the relative position of the point in the cube
	uint8_t u = x;
	uint8_t v = y;

	// Get a signed version of the above for the grad function
	int8_t xx = ((uint8_t)(x) >> 1) & 0x7F;
	int8_t yy = ((uint8_t)(y) >> 1) & 0x7F;
	uint8_t N = 0x80;

	u = EASE8(u); v = EASE8(v);

	int8_t X1 = lerp7by8(grad8(P(AA), xx, yy), grad8(P(BA), xx - N, yy), u);
	int8_t X2 = lerp7by8(grad8(P(AB), xx, yy - N), grad8(P(BB), xx - N, yy - N), u);

	int8_t ans = lerp7by8(X1, X2, v);

	return ans;
	// return scale8((70+(ans)),234)<<1;
}



uint8_t inoise8(uint16_t x, uint16_t y) {
	//return scale8(69+inoise8_raw(x,y),237)<<1;
	int8_t n = inoise8_raw(x, y);  // -64..+64
	n += 64;                         //   0..128
	uint8_t ans = qadd8(n, n);     //   0..255
	return ans;
}

// output range = -64 .. +64
int8_t inoise8_raw(uint16_t x)
{
	// Find the unit cube containing the point
	uint8_t X = x >> 8;

	// Hash cube corner coordinates
	uint8_t A = P(X);
	uint8_t AA = P(A);
	uint8_t B = P(X + 1);
	uint8_t BA = P(B);

	// Get the relative position of the point in the cube
	uint8_t u = x;

	// Get a signed version of the above for the grad function
	int8_t xx = ((uint8_t)(x) >> 1) & 0x7F;
	uint8_t N = 0x80;

	u = EASE8(u);

	int8_t ans = lerp7by8(grad8(P(AA), xx), grad8(P(BA), xx - N), u);

	return ans;
}

uint8_t inoise8(uint16_t x) {
	int8_t n = inoise8_raw(x);    //-64..+64
	n += 64;                      // 0..128
	uint8_t ans = qadd8(n, n);     // 0..255
	return ans;
}

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
		if (x < SCREEN_WIDTH and y < SCREEN_HEIGHT) {
			return noise[x][y];
		}
		return 0;
	}

	uint16_t xOffset;
	uint16_t yOffset;
	uint16_t zOffset;
	uint16_t xScale;
	uint16_t yScale;

	uint16_t speed = 256;

	uint8_t noise[SCREEN_WIDTH][SCREEN_HEIGHT];

	uint8_t noiseSmoothing = 240;

private:
	void fillNoise8bit() {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			int ioffset = xScale * i;
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				int joffset = yScale * j;

				uint8_t data = inoise16(xOffset + ioffset, yOffset + joffset, zOffset) >> 8;

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
		xOffset += speed / 16; // keep interesting by changing x/y offsets
		yOffset += speed / 30;
		zOffset += speed;
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
		uint8_t dataSmoothing = 0;
		if (speed < 50) {
			dataSmoothing = 200 - (speed * 4);
		}
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				

				uint8_t steps = (65536 / SCREEN_HEIGHT); // = 8 for 32, or 4 for 64

				uint8_t lon = steps * j;
				uint8_t lat = steps * i / 2;

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
				uint8_t steps2 = 3;
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
				data = inoise16(_x << 8 + xOffset, _y << 8 + yOffset, _z << 8 + zOffset) >> 8;
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

void fillNoise(noiseType_t type) {
	switch (type) {
	case perlin:

		break;
	case random:

		break;
	case spherical:

		break;
	case cylindrical:

		break;
	case fourDimensional:

		break;
	}
}


Noise noise;