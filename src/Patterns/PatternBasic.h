#pragma once

#include "Pattern.h"

class PatternRowThenCol : public Pattern {
public:
	PatternRowThenCol()	: Pattern("Rows then Colums") {}

	uint8_t drawFrame() {
		if (Pattern::useDefaultEffect) {
			gfx.fade(50);
		}

		//      unsigned long now = millis();

		// call once every 50msec
		static uint16_t index = 0;
		//  static uint8_t hue = 0;
		CRGB colour = gfx.getColour();
		//  adjustGamma(colour);
		if (index < SCREEN_HEIGHT) {
			gfx.drawLine(0, index, SCREEN_WIDTH, index, colour);
			//ledControl.setRow(index, colour);
			index++;
		}
		else if (index < (SCREEN_HEIGHT * 2)) {
			uint8_t row = SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT);
			gfx.drawLine(0, row, SCREEN_WIDTH, row, colour);
			//ledControl.setRow(SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT), colour);
			index++;
		}
		else if (index < ((SCREEN_HEIGHT * 2) + 8 * 4)) {
			uint8_t col = index - (SCREEN_HEIGHT * 2);
			gfx.drawLine(col, 0, col, SCREEN_HEIGHT, colour);
			gfx.drawLine(col+32, 0, col+32, SCREEN_HEIGHT, colour);
			//ledControl.setCol(index - (SCREEN_HEIGHT * 2), colour);
			index++;
		}
		else if (index < ((SCREEN_HEIGHT * 2) + 16 * 4)) {
			uint8_t col = (16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 8 * 4);
			gfx.drawLine(col-32, 0, col-32, SCREEN_HEIGHT, colour);
			gfx.drawLine(col, 0, col, SCREEN_HEIGHT, colour);
			//ledControl.setCol((16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 16 * 4), colour);
			index++;
		}
		else {
			index = 0;
		}
		

		//      uint16_t offset = millis() - now;
		return returnVal;
	}
};


class PatternSparks : public Pattern {
public:
	PatternSparks() : Pattern("Sparks") {}

	uint8_t drawFrame() {

		if (Pattern::useDefaultEffect) {
			gfx.fade(30);
		}

		
		for (int i = 0; i < 4; i++) {
			gfx.putPixel(random8(SCREEN_WIDTH), random8(SCREEN_HEIGHT), gfx.getColour(random8(64)));
		}
		


		return returnVal;
	}

};
