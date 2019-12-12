#pragma once

#include "Graphics.h"
#include "..\ledControl.h"
// #include "..\data.h"


//? upgrade https://www.alibaba.com/product-detail/New-arrival-5mm-width-200LEDs-m_62099875217.html?spm=a2700.7724857.normalList.30.65266342e6nPWa

class GraphicsFastLED : public Graphics {
public:
	GraphicsFastLED(Leds *ledControl) :
		ledRef(ledControl) {}

	//void clear() {
	//	ledRef->clear();
	//}

	//virtual void fill(CRGB c) {
	//	ledRef->fillScreen(c);
	//}

	//void fade(unsigned char a = 128) {
	//	ledRef->fade(a);
	//}

	void setColorTemp(uint32_t i) {
		FastLED.setTemperature(i);
	}

	void update() {
		// change to use gfx.leds and copy to fastled buffer for double buffering
		//ledRef->fillScreen(CRGB::Black);
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				uint8_t x = i * 1 + 0, y = (j * 1 + 0) * SCREEN_WIDTH;
				ledsRaw[XY(x, y)] = leds[(x)+(y)* SCREEN_WIDTH];
			}
		}
	}
	void show() {
		FastLED.setBrightness(Data::brightness);
		FastLED.show();
	}

	//CRGB getColour(uint8_t offset = 0) {
	//	return ledRef->getColour(offset);
	//}

	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		leds[myXY(x, y)] = CRGB(r, g, b);
		//ledRef->drawPixel(x, y, CRGB(r, g, b));
	}

	void putPixel(int x, int y, CRGB c) {
		leds[myXY(x, y)] = c;
		//ledRef->drawPixel(x, y, c);
	}

	void putPixelDirect(int x, int y, CRGB c0) {
		CRGB c1 = leds[x + y * SCREEN_WIDTH];
		CRGB out;
		int16_t dr, dg, db;
		dr = c0.r - c1.r;
		dg = c0.g - c1.g;
		db = c0.b - c1.b;
		ledsRaw[XY(x, y)].r = c1.r + (dr * textOpacity) / 255;
		ledsRaw[XY(x, y)].g = c1.g + (dg * textOpacity) / 255;
		ledsRaw[XY(x, y)].b = c1.b + (db * textOpacity) / 255;

		//ledsRaw[XY(x, y)] = c;
		//ledRef->drawPixel(x, y, c);
	}

	void shiftRawX(int8_t amount) {
	
	}

	void putPixel(int x, int y, unsigned char h) {
		CRGB c = getColour(h);
		putPixel(x, y, c);
		//ledRef->drawPixel(x, y, c);
	}

	//void blendPixel(int x, int y, CRGB c, uint8_t a=128) {
	//	ledRef->drawBlend(x, y, c, a);
	//}

	// crgb getcolor(unsigned char offset = 0) {

	// }

	static uint16_t XY(uint8_t x, uint8_t y) {
		/*//leds are mapped serpentine with 2 offset with 30 leds in each row e.g.
		  0       60
		  1       61
		  2   59  62  119
		  3   ..  63  ..
		  ..  33  ..  93
		  29  32  89  92
			  31      91
			  30      90
		*/
		const uint16_t _LAST_VISIBLE_LED = 1920;

		if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
			return _LAST_VISIBLE_LED + 1;

		uint16_t j;
		uint16_t i = x * (SCREEN_HEIGHT - 2); // each column actually has 30 leds
		if (x % 2 == 0) {// even row
			if (y == 31 and x >= 16 and x < 48) {
				return (_LAST_VISIBLE_LED + (x * 2) + (y - 30));
			}
			if ((y == 30 or y == 31) and (x < 16 or x >= 48)) {
				//      return LAST_VISIBLE_LED + 1;
				return (_LAST_VISIBLE_LED + (x * 2) + (y - 30));
			}
			if (x >= 16 and x < 48)
				j = y - 1; // hardware error: accidently have the even rows of the middle half of the globe only offset from each other by 1 instead of 2
			else j = y;
		}
		else {// if odd row flip the row
			if (y == 0 or y == 1) {
				//      return LAST_VISIBLE_LED + 1;
				return (_LAST_VISIBLE_LED + (x * 2) + y);
			}
			j = (SCREEN_HEIGHT - 1) - y; //

		}
		return (i + j);
	}


	Leds *ledRef;
};