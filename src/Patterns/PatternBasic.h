#pragma once

#include "Pattern.h"


class PatternSolid : public _Pattern {
public:
	PatternSolid() : _Pattern("Solid") {}

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		uint8_t val = myMap(*beat, 0, 255, 128, 255);

		gfx.fill(gfx.getColour().nscale8(val));



		return returnVal;
	}

};

class PatternRowThenCol : public _Pattern {
public:
	PatternRowThenCol() : _Pattern("Rows then Colums") {}

	uint8_t drawFrame() {
		_Pattern::drawFrame();
		

		//      unsigned long now = millis();

		// call once every 50msec
		static uint16_t index = 0;
		static uint8_t offset = 0;

		uint16_t nextIndex = 0;
		nextIndex = myMap(beat8(beat.getBPM()>>10), 0, 255, 0, 128);
		

		uint8_t colourOffset = 64;
		//  static uint8_t hue = 0;
		CRGB colour = gfx.getColour();
		//  adjustGamma(colour);
		while (index <= nextIndex) {

			if (!_Pattern::useCustomEffect) {
				gfx.fade(40);
			}

			if (index < SCREEN_HEIGHT) { // 0 to 32
				uint8_t row = index;
				colour = gfx.getColour(row);
				//gfx.drawLine(0, index, SCREEN_WIDTH, index, colour);
				gfx.drawLine(0, index, SCREEN_WIDTH / 2, index, row, row + colourOffset);
				gfx.drawLine(SCREEN_WIDTH / 2, index, SCREEN_WIDTH, index, row + colourOffset, row);
				//ledControl.setRow(index, colour);
				//index++;
			}
			else if (index < (SCREEN_HEIGHT * 2)) { // 32 to 64
				uint8_t row = SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT);
				colour = gfx.getColour(row);
				//gfx.drawLine(0, row, SCREEN_WIDTH, row, colour);
				gfx.drawLine(0, row, SCREEN_WIDTH / 2 - 1, row, colourOffset / 2 + index % 32, colourOffset / 2 + index % 32 + colourOffset);
				gfx.drawLine(SCREEN_WIDTH / 2, row, SCREEN_WIDTH - 1, row, colourOffset / 2 + index % 32 + colourOffset, colourOffset / 2 + index % 32);
				//ledControl.setRow(SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT), colour);
				//index++;
			}
			else if (index < ((SCREEN_HEIGHT * 2) + SCREEN_WIDTH / 2)) { // 64 to 96
				uint8_t col = (index - (SCREEN_HEIGHT * 2) + offset) % SCREEN_WIDTH / 2;
				colour = gfx.getColour(col);
				gfx.drawLine(col, 0, col, SCREEN_HEIGHT - 1, col, col + colourOffset / 2);
				colour = gfx.getColour(col + 64);
				gfx.drawLine(col + 32, 0, col + 32, SCREEN_HEIGHT - 1, col + 64, col + 64 - colourOffset / 2);
				//ledControl.setCol(index - (SCREEN_HEIGHT * 2), colour);
				//index++;
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

				//index++;
			}
			else {
				index = 0;
				nextIndex = 0;
				offset = offset + random8(SCREEN_WIDTH/4) - SCREEN_WIDTH/8;
				offset %= SCREEN_WIDTH;
				break;
			}

			index++;
		}
		if (index == SCREEN_WIDTH + SCREEN_HEIGHT*2) index = 0;
		if (index > nextIndex + 20) index = nextIndex;


		//      uint16_t offset = millis() - now;
		return returnVal;
	}
};


class PatternSparks : public _Pattern {
public:
	PatternSparks() : _Pattern("Sparks") {
		numVars = 2;
		vars[0] = &chanceRef;
		vars[1] = &fadeRef;
	}

	//variables
	uint8_t chance = 128;
	uint8_t fade = 10;

	VariableReference chanceRef{ "Chance", &chance };
	VariableReference fadeRef{ "Fade", &fade, 10, 5, 20 };

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		//uint8_t chance = beatsin8(90); //128 = 50%

		chance = *beat;

		if (!_Pattern::useCustomEffect) {
			uint8_t fade = 10;
			if (chance > GuiVars1*128) //128 gui1
				fade = myMap(chance, GuiVars1 * 128, 255, 10, GuiVars3*20); //gui3 23
			gfx.fade(fade);
		}

		uint8_t maxI = 1;
		if (chance > GuiVars2 * 128) { //49 gui2
			maxI = myMap(chance, GuiVars2 * 128, 255, 0, GuiVars4 * 5); //gui4 7 or 8
		}
		for (int i = 0; i < maxI; i++) {
			//if (i == 0 or chance > 200)
				gfx.putPixel(random8(SCREEN_WIDTH), random8(SCREEN_HEIGHT), gfx.getColour(random8(64)));
		}



		return returnVal;
	}

};


class PatternVWaves : public _Pattern{
public:
	PatternVWaves() : _Pattern("Vertical Wave") {}

	virtual void start() {
		beat.halfBPM = true;
	}
	virtual void stop() {
		beat.halfBPM = false;
	}

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		if (!_Pattern::useCustomEffect) {

			gfx.fade(64);
			//gfx.blur(177);
		}

		uint8_t scale = 256 / SCREEN_WIDTH;
		static uint16_t index = 0;
		const uint8_t microSteps = 16;

		CRGB colour;// = gfx.getColour();
		//CRGB colour2 = gfx.getColour(64);

		uint8_t x = 0;
		static uint16_t offset = 0;
		uint8_t width = 0;
		//uint8_t maxWidth = myMap(beatsin8(60), 0, 255, 16, 1);
		uint8_t maxWidth = myMap(*beat, 0, 255, 16, 1);
		
//        for (int8_t i = 8; i > 0; i--) {

			for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
				//    uint8_t x = quadwave8(y * 2 - (Data::index/microSteps)%256) / scale;
				x = quadwave8(y * 2 - (index / microSteps) % 256) / scale;
				//    x = (y * 2 - (Data::index/microSteps)%256) / scale;
				//    if ((Data::index/microSteps) %256 > 128)
				//      x = COLS-x;
				//    x = (x + ((Data::index / 128) % COLS)) % COLS;
				colour = gfx.getColour(y);
                width = myMap(sin8(beat8(30)+y*4), 0, 256, 0, 16);
				width = min(width, maxWidth);
                for (int8_t w = width; w >= 0; w--) {
                    colour.fadeToBlackBy((width-w) * (160/(width+1)) + y * 2);
                    gfx.blendPixel((x + w + (offset)) % SCREEN_WIDTH, y, colour);
                }
//                colour.fadeToBlackBy((8-i) * 20 + y * 2);
//                gfx.blendPixel((x + i+ (offset)) % SCREEN_WIDTH, y, colour);
			}
			for (int8_t y = SCREEN_HEIGHT-1; y >= 0; y--) {
				x = quadwave8((y + SCREEN_HEIGHT / 2) * 2 + 128 - (index / microSteps) % 256) / scale;
				//    x = (x + ((Data::index / 128) % COLS)) % COLS;
				colour = gfx.getColour(64+y);
                width = myMap(sin8(beat8(30) +y*4), 0, 256, 0, 16);
				width = min(width, maxWidth);
                for (int8_t w = width; w >= 0; w--) {
                    colour.fadeToBlackBy((width-w) * (160/(width+1)) + y * 2);
                    gfx.blendPixel((x + w + (offset)) % SCREEN_WIDTH, y, colour);
                }
//                colour.fadeToBlackBy((8 - i) * 20 + y *2);
//                gfx.blendPixel((x + i + (offset)) % SCREEN_WIDTH, y, colour);
			}
			//colour.fadeToBlackBy(60);
			//colour.fadeToBlackBy(60);

//        }
		
		//offset += 1;
		offset %= SCREEN_WIDTH;
		index += microSteps;

		return returnVal;
	}
	

};


static const struct {
	uint8_t  	 width;
	uint8_t  	 height;
	uint8_t  	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
	uint8_t 	 pixel_data[13 * 14 * 2 + 1];
} fish = {
  13, 14, 2,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\271\000\000\000\000\000\000\000\000?\007\354\347\000\000\000\000\000\000\000\000"
  "\000\000f\271\347\375\347\375\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\347"
  "\375\347\375f\271\000\000\000\000\000\000\000\000f\271\000\000\000\000\000\000\000\000\000\000f\271f\271\347"
  "\373f\271\000\000\000\000\000\000\347\375f\271\000\000\000\000\000\000\347\375f\271f\271f\271\347"
  "\373\347\375\000\000\000\000\000\000f\271f\271\000\000\347\375\347\375\347\375f\271f\271"
  "\347\373\347\373\347\375\000\000\000\000\347\375f\271\347\375\347\375\347\375f\271"
  "f\271f\271\347\373\000\000\347\375\347\375\000\000\347\375f\271\347\375\347\375\347"
  "\375f\271f\271f\271\347\373\347\373\347\375\347\375\000\000f\271f\271\000\000\347"
  "\375\347\375\347\375f\271f\271\347\373\347\373\347\375\000\000\347\375f\271\000"
  "\000\000\000\000\000\347\375f\271f\271f\271\347\373\347\375\000\000\000\000f\271\000\000\000\000\000"
  "\000\000\000\000\000f\271f\271\347\373f\271\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\347"
  "\375\347\375f\271\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000f\271\347\375\347\375"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\347\375\347\375f\271\000\000\000\000\000\000\000\000"
  "\000\000\000\000",
};


class PatternFish : public _Pattern {
public:
	PatternFish() : _Pattern("Fishy") {}

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(64);
			//gfx.blur(177);
		}
		drawFish(0, 0);

		return returnVal;
	}

	void drawFish(uint8_t x, uint8_t y) {
		gfx.drawRGBBitmap(x, y, &fish.pixel_data[0], fish.width, fish.height);


	}


};