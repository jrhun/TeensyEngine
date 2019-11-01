#pragma once


#include "Pattern.h"

class PatternBeachWaves : public _Pattern {
public:
	PatternBeachWaves()
		: _Pattern("Beach Waves") {}

	uint8_t drawFrame() {
		//      unsigned long now = millis();

		//      CRGB colour1 = ledControl.getColour();
		//      CRGB colour2 = ledControl.getColour(64);

		if (_Pattern::useDefaultEffect) {
			gfx.fade(2);// ledControl.fade();
			blur2d(gfx.leds, SCREEN_WIDTH, SCREEN_HEIGHT, 176);
		}
		//
		static uint16_t offset = 0;
		offset += 64;
		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
			uint8_t x1 = beat8(20);
			uint8_t x2 = beat8(23);
			x1 = myMap(x1, 0, 255, 0, SCREEN_WIDTH) + (SCREEN_HEIGHT - y) * (inoise16(offset + (y*y / (64)) * 256) / (beatsin8(120, 160, 160)*256.0));
			x2 = myMap(x2, 0, 255, SCREEN_WIDTH, 0) + SCREEN_WIDTH - (SCREEN_HEIGHT - y) * (inoise16(offset + (y*y / 64) * 256) / (beatsin8(120, 160, 160) * 256.0));
			x1 %= SCREEN_WIDTH;
			x2 %= SCREEN_WIDTH;
			uint8_t h1 = Data::getHue() + y * 2 + random8(10);
			uint8_t h2 = Data::getHue() + 64 + y * 2 + random8(15);
			CRGB c1 = CHSV(h1, 255 - y * 2, 255);
			CRGB c2 = CHSV(h2, 255 - y * 2, 255);
			gfx.putPixel(x1, y, c1);
			gfx.putPixel(x2, y, c2);
		}
		//blur2d(gfx.leds, SCREEN_WIDTH, SCREEN_HEIGHT, 120);


		//      uint16_t offset = millis() - now;
		return (1000 / Data::FPS);
	}
};

class PatternBeachWaves2 : public _Pattern {
public:
	PatternBeachWaves2()
		: _Pattern("Beach Waves 2") {}

	uint8_t drawFrame() {
		//      unsigned long now = millis();

		//      CRGB colour1 = ledControl.getColour();
		//      CRGB colour2 = ledControl.getColour(64);

		if (_Pattern::useDefaultEffect) {
			gfx.fade(2);// ledControl.fade();
			blur2d(gfx.leds, SCREEN_WIDTH, SCREEN_HEIGHT, 176);
		}
		//
		static uint16_t offset = 0;
		offset += 8;
		//for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
		uint8_t x1 = myMap(beat8(20), 0, 255, 0, SCREEN_WIDTH);
		uint8_t x2 = myMap(beat8(23), 0, 255, 0, SCREEN_WIDTH);
		uint8_t y1 = 0;
		if (offset % 4096 < 1024) {
			y1 = 0;
		}
		else if (offset % 4096 < 2048) {
			y1 = (sin8((offset % 1024) / 16) - 127) / 4;
		}
		else if (offset % 4096 < 3096) {
			y1 = SCREEN_HEIGHT - 1;
		}
		else {
			y1 = (sin8((1024 - offset % 1024) / 16) - 127) / 4;
		}

		//uint8_t h1 = Data::getHue() + random8(10);
		//uint8_t h2 = Data::getHue() + 64 + random8(15);
		gfx.drawLine(x1, y1, x1 + SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT - y1 - 1, random8(16), 32 + random8(16));
		gfx.drawLine(SCREEN_WIDTH - x2, SCREEN_HEIGHT - y1 - 1, SCREEN_WIDTH - x2 + SCREEN_WIDTH / 2 - 1, y1, random8(16) + 32, 64+ random8(16));
		//gfx.drawLine(SCREEN_WIDTH - x2, y1, x2, SCREEN_HEIGHT - y1, h2, h1);
		//	x1 = myMap(x1, 0, 255, 0, SCREEN_WIDTH) + (SCREEN_HEIGHT - y) * (inoise16(offset + (y*y / (64)) * 256) / (beatsin8(120, 160, 160)*256.0));
		//x2 = myMap(x2, 0, 255, SCREEN_WIDTH, 0) + SCREEN_WIDTH - (SCREEN_HEIGHT - y) * (inoise16(offset + (y*y / 64) * 256) / (beatsin8(120, 160, 160) * 256.0));
		//x1 %= SCREEN_WIDTH;
		//x2 %= SCREEN_WIDTH;
		//uint8_t h1 = Data::getHue() + y * 2 + random8(10);
		//uint8_t h2 = Data::getHue() + 64 + y * 2 + random8(15);
		//CRGB c1 = CHSV(h1, 255 - y * 2, 255);
		//CRGB c2 = CHSV(h2, 255 - y * 2, 255);
		//gfx.putPixel(x1, y, c1);
		//gfx.putPixel(x2, y, c2);

	//blur2d(gfx.leds, SCREEN_WIDTH, SCREEN_HEIGHT, 120);


	//      uint16_t offset = millis() - now;
		return (1000 / Data::FPS);
	}
};
