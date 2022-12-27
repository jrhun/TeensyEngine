#pragma once

#include "Graphics.h"
//#include "..\ledControl.h"
// #include "..\data.h"

#include <FastLED.h>


//? upgrade https://www.alibaba.com/product-detail/New-arrival-5mm-width-200LEDs-m_62099875217.html?spm=a2700.7724857.normalList.30.65266342e6nPWa
//fastLED specific defines
static const uint8_t NUM_LEDS = SCREEN_WIDTH * SCREEN_HEIGHT;
const uint8_t NUM_STRIPS = 8;
const uint8_t NUM_LEDS_PER_STRIP = 240;
const uint16_t LAST_VISIBLE_LED = 1920;  //(NUM_STRIPS * NUM_LEDS_PER_STRIP)

CRGBArray < SCREEN_WIDTH * SCREEN_HEIGHT > ledsRaw;

uint16_t XY(uint8_t x, uint8_t y) {
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
	return 0;

	if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
		return LAST_VISIBLE_LED + 1;

	uint16_t j;
	uint16_t i = x * (SCREEN_HEIGHT - 2); // each column actually has 30 leds
	if (x % 2 == 0) {// even row
		if (y == 31 and x >= 16 and x < 48) {
			return (LAST_VISIBLE_LED + (x * 2) + (y - 30));
		}
		if ((y == 30 or y == 31) and (x < 16 or x >= 48)) {
			//      return LAST_VISIBLE_LED + 1;
			return (LAST_VISIBLE_LED + (x * 2) + (y - 30));
		}
		if (x >= 16 and x < 48)
			j = y - 1;
		else j = y;
	}
	else {// if odd row flip the row
		if (y == 0 or y == 1) {
			//      return LAST_VISIBLE_LED + 1;
			return (LAST_VISIBLE_LED + (x * 2) + y);
		}
		j = (SCREEN_HEIGHT - 1) - y; //

	}
	return (i + j);
}


class GraphicsFastLED : public Graphics {
public:
	//GraphicsFastLED(Leds *ledControl) :
	//	ledRef(ledControl) {}

	GraphicsFastLED() {}

	//void clear() {
	//	ledRef->clear();
	//}

	//virtual void fill(CRGB c) {
	//	ledRef->fillScreen(c);
	//}

	//void fade(unsigned char a = 128) {
	//	ledRef->fade(a);
	//}



	


	void init() {
#if defined(ESP32)
		FastLED.addLeds<WS2812B, 13, GRB>(ledsRaw, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 12, GRB>(ledsRaw, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 14, GRB>(ledsRaw, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 27, GRB>(ledsRaw, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 26, GRB>(ledsRaw, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 25, GRB>(ledsRaw, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 33, GRB>(ledsRaw, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 32, GRB>(ledsRaw, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		// teensy 4.x, allows DMA output, uses predefined pins though 
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__) 
		FastLED.addLeds<NUM_STRIPS, WS2812B, 19, GRB>(ledsRaw, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
#endif 

		FastLED.setBrightness(*Data::brightness_t);

		FastLED.setTemperature(WarmFluorescent);
		FastLED.setDither(0);
	}

	void setColorTemp(uint32_t i) {
		FastLED.setTemperature(i);
	}

	void update() {
		// change to use gfx.leds and copy to fastled buffer for double buffering
		//ledRef->fillScreen(CRGB::Black);
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				uint8_t x = i * 1 + 0, y = (j * 1 + 0) * SCREEN_WIDTH;
				ledsRaw[this->XY(i, j)] = leds[(i)+(j)* SCREEN_WIDTH];
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

	void putPixelDirect(int x, int y, CRGB c0, uint8_t opacity = 255) {
		CRGB c1 = leds[myXY(x, y)];
		CRGB out;
		int16_t dr, dg, db;
		dr = c0.r - c1.r;
		dg = c0.g - c1.g;
		db = c0.b - c1.b;
		ledsRaw[this->XY(x, y)].r = c1.r + (dr * opacity) / 255;
		ledsRaw[this->XY(x, y)].g = c1.g + (dg * opacity) / 255;
		ledsRaw[this->XY(x, y)].b = c1.b + (db * opacity) / 255;

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

	uint16_t XY(uint8_t x, uint8_t y) {
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


	//Leds *ledRef;
};