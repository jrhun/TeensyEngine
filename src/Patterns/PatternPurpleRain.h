#pragma once

#include "Pattern.h"

//#include <vector>

class Drop {
public:
	Drop() { 
		init();
		y = random8(SCREEN_WIDTH*2) - SCREEN_WIDTH*2;
		ySpeed += myMap(y, -SCREEN_WIDTH*2, 0, 0, 4.0) / 6.0f;
		//ySpeed = 0.05;
	}

	void init() {
		x = random8(SCREEN_WIDTH);
		y = random8(SCREEN_WIDTH) - SCREEN_WIDTH*2;
		z = random8(0, 20);
		len = myMap(z, 0, 20, 10, 20);
		ySpeed = myMap(z, 0, 20, 4 / (2*10.0f), 10 / (2*10.0f)) / 4.0f;
	}

	float x;
	float y;
	float z;
	float ySpeed;
	float len;

	void fall() {
		this->y = this->y + this->ySpeed;
		ySpeed += 0.005;
		if (y > SCREEN_HEIGHT) {
			init();
		}
	}

	static CRGB background;

	void show() {
		CRGB c1 = CRGB(138, 43, 226);
		CRGB c2 = background;
		uint8_t scale = myMap(z, 0, 20, 160, 255);
		c1.nscale8(scale);
		gfx.drawLine(x, y, x, y + len, c1, c2);
	}
};

CRGB Drop::background = CRGB::Black;// CRGB(230, 230, 250).nscale8(16);// GuiVars1 * 128);

Drop drops[200];

class PatternPurpleRain : public _Pattern {
public:
	PatternPurpleRain() : _Pattern("Purple Rain") {}

	uint8_t drawFrame() {
		//Drop::background = CRGB(230, 230, 250).nscale8(GuiVars1 * 128);
		//Drop::background = CHSV(Data::getHue(), 255, 255);
		//Drop::background.nscale8(GuiVars1 * 128);

		gfx.fill(Drop::background);

		for (Drop &d : drops) {
			d.fall();
			d.show();
		}

		return returnVal;
	}
};