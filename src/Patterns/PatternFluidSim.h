#pragma once

#include "Pattern.h"

//sample running http://neuroid.co.uk/lab/fluid/
//js http://neuroid.co.uk/lab/fluid/fluid.js
//processing https://github.com/weymouth/lily-pad
// forum talking about latice boltman calculations https://github.com/CodingTrain/Rainbow-Topics/issues/178

// use int16 with step of 256 for finer movements but still avoiding floats 
//only one particle in each spot, no blending for water/sand


struct _Particle {
	_Particle() {}
	~_Particle() {}

	int16_t px; 
	int16_t py;
	int16_t vx;
	int16_t vy;

	uint8_t hue; 

};

//try easy way first, no particles, use the LED buffer 
//noita method https://www.youtube.com/watch?v=prXuyMCgbTc
class PatternFluidSim : public _Pattern {
public:
	PatternFluidSim() : _Pattern("Fluid Sim") {}

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