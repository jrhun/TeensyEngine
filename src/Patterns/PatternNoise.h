#pragma once

#include "Pattern.h"

DEFINE_GRADIENT_PALETTE(Black_White_gp) {
	0, 255, 255, 255,
		32, 0, 0, 0,
		64, 0, 0, 0,
		96, 0, 0, 0,
		128, 255, 255, 255,
		160, 0, 0, 0,
		192, 0, 0, 0,
		224, 0, 0, 0,
		255, 255, 255, 255
};

class PatternNoise1 : public _Pattern {
public:
	PatternNoise1() : _Pattern("Noise1") {}



	uint8_t drawFrame() {
		noise.fillNoise();


		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				// We use the value at the (i,j) coordinate in the noise
				// array for our brightness, and the flipped value from (j,i)
				// for our pixel's index into the color palette.

				uint8_t index = noise.At(i, j);// noise[j][i];
				uint8_t bri = noise.At(i, j);// [i][j];

				// if this palette is a 'loop', add a slowly-changing base value
				//if (colorLoop) {
				//	index += ihue;
				//}

				// brighten up, as the color palette itself often contains the
				// light/dark dynamic range desired
				if (bri > 127) {
					bri = 255;
				}
				else {
					bri = dim8_raw((bri + 32) * 2);
				}

				//CRGB color = ColorFromPalette(Black_White_gp, index, bri);
				CRGB color = gfx.getColour(index + Data::getHue(), bri);
				//CRGB color = CHSV(index, 255, bri);


				gfx.putPixel(i,j, color);
			}
		}

		return returnVal;
	}

};
