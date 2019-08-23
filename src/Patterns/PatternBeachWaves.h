#pragma once


#include "Pattern.h"

class PatternBeachWaves : public Pattern {
public:
	PatternBeachWaves()
		: Pattern("Beach Waves") {}

	uint8_t drawFrame() {
		//      unsigned long now = millis();

		//      CRGB colour1 = ledControl.getColour();
		//      CRGB colour2 = ledControl.getColour(64);
		gfx.fade(64);// ledControl.fade();

		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
			uint8_t x1 = beat8(20);
			uint8_t x2 = beat8(23);
			x1 = myMap(x1, 0, 255, 0, SCREEN_WIDTH) + y;
			x2 = myMap(x2, 0, 255, SCREEN_WIDTH, 0) + SCREEN_WIDTH - y;
			x1 %= SCREEN_WIDTH;
			x2 %= SCREEN_WIDTH;
			uint8_t h1 = Data::getHue() + y * 2 + random8(10);
			uint8_t h2 = Data::getHue() + 64+ y * 2 + random8(15);
			CRGB c1 = CHSV(h1, 255 - y * 2, 255);
			CRGB c2 = CHSV(h2, 255 - y * 2, 255);
			gfx.putPixel(x1, y, c1);
			gfx.putPixel(x2, y, c2);
		}
		blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, 120);
		

		//      uint16_t offset = millis() - now;
		return (1000 / Data::FPS);
	}
};