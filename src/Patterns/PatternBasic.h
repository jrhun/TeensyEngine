#pragma once

#include "Pattern.h"

class PatternRowThenCol : public Pattern {
public:
	PatternRowThenCol() : Pattern("Rows then Colums") {}

	uint8_t drawFrame() {
		if (Pattern::useDefaultEffect) {
			gfx.fade(40);
		}

		//      unsigned long now = millis();

		// call once every 50msec
		static uint16_t index = 0;
		static uint8_t offset = 0;

		uint8_t colourOffset = 64;
		//  static uint8_t hue = 0;
		CRGB colour = gfx.getColour();
		//  adjustGamma(colour);
		if (index < SCREEN_HEIGHT) { // 0 to 32
			uint8_t row = index;
			colour = gfx.getColour(row);
			//gfx.drawLine(0, index, SCREEN_WIDTH, index, colour);
			gfx.drawLine(0, index, SCREEN_WIDTH / 2, index, row, row + colourOffset);
			gfx.drawLine(SCREEN_WIDTH / 2, index, SCREEN_WIDTH, index, row + colourOffset, row);
			//ledControl.setRow(index, colour);
			index++;
		}
		else if (index < (SCREEN_HEIGHT * 2)) { // 32 to 64
			uint8_t row = SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT);
			colour = gfx.getColour(row);
			//gfx.drawLine(0, row, SCREEN_WIDTH, row, colour);
			gfx.drawLine(0, row, SCREEN_WIDTH / 2 - 1, row, colourOffset / 2 + index % 32, colourOffset / 2 + index % 32 + colourOffset);
			gfx.drawLine(SCREEN_WIDTH / 2, row, SCREEN_WIDTH - 1, row, colourOffset / 2 + index % 32 + colourOffset, colourOffset / 2 + index % 32);
			//ledControl.setRow(SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT), colour);
			index++;
		}
		else if (index < ((SCREEN_HEIGHT * 2) + SCREEN_WIDTH / 2)) { // 64 to 96
			uint8_t col = (index - (SCREEN_HEIGHT * 2) + offset) % SCREEN_WIDTH / 2;
			colour = gfx.getColour(col);
			gfx.drawLine(col, 0, col, SCREEN_HEIGHT - 1, col, col + colourOffset / 2);
			colour = gfx.getColour(col + 64);
			gfx.drawLine(col + 32, 0, col + 32, SCREEN_HEIGHT - 1, col + 64, col + 64 - colourOffset / 2);
			//ledControl.setCol(index - (SCREEN_HEIGHT * 2), colour);
			index++;
		}
		else if (index < ((SCREEN_HEIGHT * 2) + SCREEN_WIDTH)) { // 96 to 128
			uint8_t col = (SCREEN_WIDTH / 2 - 1) - (index - SCREEN_HEIGHT * 2 - SCREEN_WIDTH / 2);
			//uint8_t col = (16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 8 * 4);
			col = (col + offset) % SCREEN_WIDTH;
			colour = gfx.getColour(col);
			gfx.drawLine(col, 0, col, SCREEN_HEIGHT - 1, col, col + colourOffset / 2);
			colour = gfx.getColour(col + 64);
			gfx.drawLine(col + 32, 0, col + 32, SCREEN_HEIGHT - 1, col + 64, col + 64 - colourOffset / 2);
			//ledControl.setCol((16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 16 * 4), colour);
			index++;
		}
		else {
			index = 0;
			offset = offset + random8(8) - 4;
			offset %= 64;
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
			gfx.fade(10);
		}


		for (int i = 0; i < 4; i++) {
			gfx.putPixel(random8(SCREEN_WIDTH), random8(SCREEN_HEIGHT), gfx.getColour(random8(64)));
		}



		return returnVal;
	}

};


class PatternVWaves : public Pattern{
public:
	PatternVWaves() : Pattern("Vertical Wave") {}

	uint8_t drawFrame() {

		if (Pattern::useDefaultEffect) {

			gfx.fade(64);
			//blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, 177);
		}

		uint8_t scale = 256 / SCREEN_WIDTH;
		static uint16_t index = 0;
		const uint8_t microSteps = 16;

		CRGB colour;// = gfx.getColour();
		//CRGB colour2 = gfx.getColour(64);

		uint8_t x = 0;
		static uint16_t offset = 0;
		for (uint8_t i = 8; i > 0; i--) {

			for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
				//    uint8_t x = quadwave8(y * 2 - (Data::index/microSteps)%256) / scale;
				x = quadwave8(y * 2 - (index / microSteps) % 256) / scale;
				//    x = (y * 2 - (Data::index/microSteps)%256) / scale;
				//    if ((Data::index/microSteps) %256 > 128)
				//      x = COLS-x;
				//    x = (x + ((Data::index / 128) % COLS)) % COLS;
				colour = gfx.getColour(y);
				colour.fadeToBlackBy((8-i) * 20 + y * 2);
				gfx.blendPixel((x + i+ (offset)) % SCREEN_WIDTH, y, colour);
			}
			for (int8_t y = SCREEN_HEIGHT-1; y >= 0; y--) {
				x = quadwave8((y + SCREEN_HEIGHT / 2) * 2 + 128 - (index / microSteps) % 256) / scale;
				//    x = (x + ((Data::index / 128) % COLS)) % COLS;
				colour = gfx.getColour(64+y);
				colour.fadeToBlackBy((8 - i) * 20 + y *2);
				gfx.blendPixel((x + i + (offset)) % SCREEN_WIDTH, y, colour);
			}
			//colour.fadeToBlackBy(60);
			//colour.fadeToBlackBy(60);

		}
		
		//offset += 1;
		offset %= SCREEN_WIDTH;
		index += microSteps;

		return returnVal;
	}

};