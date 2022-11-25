#pragma once

#include "Pattern.h"

//sample running http://neuroid.co.uk/lab/fluid/
//js http://neuroid.co.uk/lab/fluid/fluid.js
//processing https://github.com/weymouth/lily-pad
// forum talking about latice boltman calculations https://github.com/CodingTrain/Rainbow-Topics/issues/178

// use int16 with step of 256 for finer movements but still avoiding floats 
//only one particle in each spot, no blending for water/sand




//based on https://github.com/mike-rankin/ESP32_CoinCell_Color_TFT/blob/master/Code/colorcoin_test/colorcoin_test.ino
//also dislike this method... 
struct Grain {
	Grain() {}

	int16_t px, py;
	int16_t vx, vy;

	uint8_t hue;
};

class PatternFluidSim : public _Pattern {
public:
	PatternFluidSim() : _Pattern("Fluid Sim") {
		clear();
	}

	void clear() {
		for (uint16_t i = 0; i < 256; i++) {
			bitField[i] = 0;
		}
		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			grains[i].px = grains[i].py = grains[i].vx = grains[i].vy = 0;
			grains[i].hue = 0;
		}
	}

	void start() {
		clear();
		int16_t i, j, x, y; 
		for (i = 0; i < NUM_GRAINS; i++) {
			do {
				grains[i].px = random16(SCREEN_WIDTH * 256);
				grains[i].py = random16(SCREEN_HEIGHT* 256);
				for (j = 0; (j < i) && (((grains[i].px / 256) != (grains[j].px / 256)) ||
					((grains[i].py / 256) != (grains[j].py / 256))); j++);
			} while (j < i); // Keep retrying until a clear spot is found
			x = grains[i].px / 256;
			y = grains[i].py / 256;
			grains[i].vx = grains[i].vy = 0; 
			grains[i].hue = random8(32);
			markPos(x, y, true);
			
		}
	}


	uint8_t drawFrame() {

		float dir = myMap(GuiVars1, 0, 2, 0, 2 * PI); 
		float posX, posY; 
		posX = cos(dir); 
		posY = sin(dir); 
		float vel = myMap(GuiVars2, 0, 2, 10, 250);


		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			int16_t vx, vy, newx, newy;
			int16_t x1, y1, x2, y2;
			int32_t v2 = 0;
			vx = grains[i].vx + posX * vel; //base 9.8ms
			vy = grains[i].vy + posY * vel;
			v2 = (int32_t)(vx * vx) + (int32_t)(vy * vy);
			//if too big trim it
			if (v2 > 400000) {
				vx = (vx * 5) / 8;
				vy = (vy * 5) / 8;
			}
			newx = grains[i].px + vx;
			newy = grains[i].py + vy;
			if (newx > SCREEN_WIDTH * 256) {
				newx = (SCREEN_WIDTH - 1) * 256;
				vx /= -2;
			}
			else if (newx < 0) {
				newx = 0; 
				vx /= -2;
			}
			//newx %= SCREEN_WIDTH * 256;
			if (newy > SCREEN_HEIGHT * 256) {
				newy = (SCREEN_HEIGHT - 1) * 256;
				vy /= -2;
			}
			else if (newy < 0) {
				newy = 0;
				vy /= -2;
			}
			x1 = grains[i].px;
			x1 /= 256;
			y1 = (grains[i].py);
			y1 /= 256;
			x2 = newx / 256;
			y2 = newy / 256;
			if (x1 != x2 || y1 != y2 && checkPosTaken(x2, y2)) { //moving to new position but occupied
				if (abs(vx) > abs(vy)) { //x axis faster
					y2 = y1; //
					if (checkPosTaken(x2, y2) == false) {
						newy = grains[i].py;
						vy = (vy / -2) + random8(8);         // and bounce Y velocity
					}
					else { // X pixel is taken, so try Y...
						y2 = newy / 256; x2 = x1;
						if (checkPosTaken(x2, y2) == false) { // Pixel is free, take it, but first...
							newx = grains[i].px; // Cancel X motion
							vx = (vx / -2) + random8(8);         // and bounce X velocity
						}
						else { // Both spots are occupied
							newx = grains[i].px; // Cancel X & Y motion
							newy = grains[i].py;
							vx = (vx / -2) + random8(8);         // Bounce X & Y velocity
							vy = (vy / -2) + random8(8);
						}
					}
				}
				else { //y axis faster 
					x2 = x1;
					if (checkPosTaken(x2, y2) == false) { // Pixel's free!  Take it!  But...
						newx = grains[i].px; // Cancel X motion
						vx = (vx / -2) + random8(8);        // and bounce X velocity
					}
					else { // Y pixel is taken, so try X...
						x2 = newx / 256; y2 = y1;
						if (checkPosTaken(x2, y2) == false) { // Pixel is free, take it, but first...
							newy = grains[i].py; // Cancel Y motion
							vy = (vy / -2) + random8(8);        // and bounce Y velocity
						}
						else { // Both spots are occupied
							newx = grains[i].px; // Cancel X & Y motion
							newy = grains[i].py;
							vx = (vx / -2) + random8(8);         // Bounce X & Y velocity
							vy = (vy / -2) + random8(8);
						}
					}
				}
			}
			grains[i].px = newx; // Update grain position
			grains[i].py = newy; // possibly only a fractional change
			grains[i].vx = vx;
			grains[i].vy = vy;
			y2 = newy / 256; x2 = newx / 256;
			if (x1 != x2 || y1 != y2)
			{
				markPos(x1, y1, false); // erase old pixel
				markPos(x2, y2, true); // Set new pixel
			}

		}

		gfx.clear();
		for (uint16_t i = 0; i < NUM_GRAINS; i++) {
			gfx.putPixel(grains[i].px / 256, grains[i].py / 256, gfx.getColour(grains[i].hue));
		}



		return returnVal;
	}

	uint8_t bitField[256]; //SCREEN_WIDTH * SCREEN_HEIGHT / 8
	bool checkPosTaken(int16_t x, int16_t y) {
		uint16_t i = x + y * SCREEN_WIDTH;
		uint8_t n = i % 8; //what bit we want
		i /= 8; //what byte
		bool taken = (bitField[i] >> n) & uint8_t(1);
		return taken; 
	}
	void markPos(int16_t x, int16_t y, bool set) {
		uint16_t i = x + y * SCREEN_WIDTH; 
		uint8_t n = i % 8; //what bit we want
		i /= 8; //what byte
		if (set) {
			bitField[i] |= uint8_t(1) << n; //set bit
		}
		else {
			bitField[i] &= ~(uint8_t(1) << n); //clear bit
		}
	}



	static const uint16_t NUM_GRAINS = 8*64; 
	Grain grains[NUM_GRAINS];
};

//try easy way first, no particles, use the LED buffer 
//noita method https://www.youtube.com/watch?v=prXuyMCgbTc
// i don't like hoow this looks, and not sure how to add in impulse force
class PatternFluidSim2 : public _Pattern {
public:
	PatternFluidSim2() : _Pattern("Fluid Sim") {}

	void start() {
		for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			buffer[i] = 0;
		}
	}

	uint8_t drawFrame() {
		gfx.clear();

		//add particle
		static uint8_t counter = 0;
		if (++counter == 5) {
			counter = 0;
			point(SCREEN_WIDTH / 2, 5) = random8();
		}

		//bottom up, move falling ones out the way first
		static uint8_t nextUpdate = 0;
		if (++nextUpdate == 10) {
			nextUpdate = 0;
			for (int8_t y = SCREEN_HEIGHT - 2; y >= 0; y--) {
				for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
					if (point(x, y) != 0) {
						if (point(x, y + 1) == 0) {
							point(x, y + 1) = point(x, y);
							point(x, y) = 0;
						}
						else {
							if (random8() > 150) {
								if (point((x - 1) + SCREEN_WIDTH, y + 1) == 0) {
									point((x - 1) + SCREEN_WIDTH, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y + 1) == 0) {
									point(x + 1, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y) == 0) {
									point((x - 1) + SCREEN_WIDTH, y) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y) == 0) {
									point(x + 1, y) = point(x, y);
									point(x, y) = 0;
								}
							}
							else {
								if (point(x + 1, y + 1) == 0) {
									point(x + 1, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y + 1) == 0) {
									point((x - 1) + SCREEN_WIDTH, y + 1) = point(x, y);
									point(x, y) = 0;
								}
								else if (point(x + 1, y) == 0) {
									point(x + 1, y) = point(x, y);
									point(x, y) = 0;
								}
								else if (point((x - 1) + SCREEN_WIDTH, y) == 0) {
									point((x - 1) + SCREEN_WIDTH, y) = point(x, y);
									point(x, y) = 0;
								}

							}
						}
					}
				}
			}
		}


		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
				uint8_t v = point(x, y);
				if (v) {
					uint8_t b = v % 0b1111; 
					b = myMap(b, 0, 15, 196, 255, true);
					CRGB c = CHSV(140 + (v >> 4) * 2, 255, b); // only sets a colour if buffer[i] isn't 0
					gfx.putPixel(x, y, c);
				}

			}
		}
		return returnVal;
	}


	//void drawParticle(_Particle* p) {
	//	gfx.putPixel(p->px / STEP, p->px / STEP, gfx.getColour(p->hue));
	//}
	//const int16_t STEP = 256;

	uint8_t& point(uint8_t x, uint8_t y) {
		return buffer[_XY(x, y)];
	}

	uint16_t _XY(uint8_t x, uint8_t y) {
		x %= SCREEN_WIDTH;
		if (y < SCREEN_HEIGHT and y >= 0)
			return (x + y * SCREEN_WIDTH);
		else return 0;
	}

	uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT]; //use bitwise check vs just use as val
};