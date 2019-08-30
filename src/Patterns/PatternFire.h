#pragma once

#include "Pattern.h"

class PatternFire : public Pattern {
public:
	PatternFire()
		: Pattern("Fire") {}

	CRGB HeatColor(uint8_t temperature)
	{
		CRGB heatcolor;

		// Scale 'heat' down from 0-255 to 0-191,
		// which can then be easily divided into three
		// equal 'thirds' of 64 units each.
		uint8_t t192 = scale8_video(temperature, 191);

		// calculate a value that ramps up from
		// zero to 255 in each 'third' of the scale.
		uint8_t heatramp = t192 & 0x3F; // 0..63
		heatramp <<= 2; // scale up to 0..252

		// now figure out which third of the spectrum we're in:
		if (t192 & 0x80) {
			// we're in the hottest third
			heatcolor.r = 255; // full red
			heatcolor.g = 255; // full green
			heatcolor.b = heatramp; // ramp up blue

		}
		else if (t192 & 0x40) {
			// we're in the middle third
			heatcolor.r = 255; // full red
			heatcolor.g = heatramp; // ramp up green
			heatcolor.b = 0; // no blue

		}
		else {
			// we're in the coolest third
			heatcolor.r = heatramp; // ramp up red
			heatcolor.g = 0; // no green
			heatcolor.b = 0; // no blue
		}

		return heatcolor;
	}


	uint8_t drawFrame() {
		// set cooling to ~125 and then oscilate sparking from 10-255 in time with the music
  // sparking base of 80 works well as a default too
		uint8_t COOLING = 125; // higher = shorter flames
		const uint8_t SPARKING = 80; // higher = moer roaring, lower = flickery
		static byte heat[SCREEN_WIDTH][SCREEN_HEIGHT];// use noise array to save ram

		//static uint16_t offset = 0;

		for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {

			// Step 1.  Cool down every cell a little
			for (int i = 0; i < SCREEN_HEIGHT; i++) {
				heat[x][i] = qsub8(heat[x][i], random8(0, ((COOLING * 10) / SCREEN_HEIGHT) + 2));
			}

			// Step 2.  Heat from each cell drifts 'up' and diffuses a little
			for (int k = SCREEN_HEIGHT - 1; k >= 2; k--) {
				heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
			}

			// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
			if (random8() < SPARKING) {
				int y = random8(2);
				heat[x][y] = qadd8(heat[x][y], random8(160, 255));
			}

			// Step 4.  Map from heat cells to LED colors
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				// Scale the heat value from 0-255 down to 0-240
				// for best results with color palettes.
				byte colorindex = scale8(heat[x][j], 240);
				gfx.putPixel(x, (SCREEN_HEIGHT - 1) - j, HeatColor(colorindex));
			}
		}

		//offset++;
		return 15;
	}

};