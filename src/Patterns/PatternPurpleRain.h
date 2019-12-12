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
		hue = random8(20);
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
	uint8_t hue;

	void fall() {
		this->y = this->y + this->ySpeed;
		ySpeed += 0.005;
		if (y > SCREEN_HEIGHT) {
			init();
		}
	}

	static CRGB background;

	 void show(bool purple = true) {
		 CRGB c1, c2;
		 uint8_t scale = myMap(z, 0, 20, 160, 255);
		 if (purple) {
			 c1 = CRGB(138, 43, 226);
			 c1.nscale8(scale);
		 }
		 else {
			 uint8_t offset = x;
			 if (x < SCREEN_WIDTH / 2)
				 offset = SCREEN_WIDTH/2 - x;
			 if (Data::paletteIndex)
				 c1 = gfx.getColour(offset + hue, scale);
			 else {
				 //uint8_t sat = myMap(y, 0, SCREEN_HEIGHT, 255, 150, true);
				 c1 = CHSV(offset + Data::getHue() + hue, 255, scale);
			 }
		 }
		c2 = background;
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


class PatternRain : public _Pattern {
public:
	PatternRain() : _Pattern("Rain") {}

	uint8_t drawFrame() {
		//Drop::background = CRGB(230, 230, 250).nscale8(GuiVars1 * 128);
		//Drop::background = CHSV(Data::getHue(), 255, 255);
		//Drop::background.nscale8(GuiVars1 * 128);

		gfx.fill(Drop::background);

		for (Drop &d : drops) {
			d.fall();
			d.show(false);
		}

		return returnVal;
	}
};