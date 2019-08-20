#pragma once

#include "Graphics.h"
#include "..\ledControl.h"

class GraphicsFastLED : public Graphics {
public:
	GraphicsFastLED(Leds *ledControl) : ledRef(ledControl) {}

	void clear() {
		ledRef->clear();
	}
	void fade(unsigned char a = 128) {
		ledRef->fade(a);
	}

	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		ledRef->drawPixel(x, y, CRGB(r, g, b));
	}

	//void putPixel(int x, int y, mRGB c) {
	//	putPixel(x, y, c.r, c.g, c.b);
	//}

	void putPixel(int x, int y, unsigned char h) {
		ledRef->drawPixel(x, y, h);
	}


	Leds *ledRef;
};