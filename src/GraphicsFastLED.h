#pragma once

#include "Graphics.h"
#include "..\ledControl.h"
// #include "..\data.h"

class GraphicsFastLED : public Graphics {
public:
	GraphicsFastLED(Leds *ledControl) :
		ledRef(ledControl) {}

	void clear() {
		ledRef->clear();
	}

	virtual void fill(CRGB c) {
		ledRef->fillScreen(c);
	}

	void fade(unsigned char a = 128) {
		ledRef->fade(a);
	}

	void show() {
		FastLED.show();
	}

	CRGB getColour(uint8_t offset = 0) {
		return ledRef->getColour(offset);
	}

	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		ledRef->drawPixel(x, y, CRGB(r, g, b));
	}

	void putPixel(int x, int y, CRGB c) {
		ledRef->drawPixel(x, y, c);
	}

	void putPixel(int x, int y, unsigned char h) {
		ledRef->drawPixel(x, y, h);
	}

	void blendPixel(int x, int y, CRGB c, uint8_t a=128) {
		ledRef->drawBlend(x, y, c, a);
	}

	// crgb getcolor(unsigned char offset = 0) {

	// }


	Leds *ledRef;
};