#pragma once

#include "Graphics.h"
#include "ofMain.h"

#include "Color.h"

#include "../data.h"
CRGB leds[SCREEN_HEIGHT * SCREEN_WIDTH];

float rgb2luma(Vec3 rgb) {
	return sqrt(rgb * Vec3(0.299, 0.587, 0.114)); // dot
}
float rgb2luma(CRGB rgb) {
	return sqrt(Vec3(rgb.r / 255.0, rgb.g / 255.0, rgb.b / 255.0) * Vec3(0.299, 0.587, 0.114)); // dot
}

class GraphicsPC : public Graphics {
public:
	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		CRGB col(r, g, b);
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT) {
			leds[(x + y * SCREEN_WIDTH)] = col;
		}
	}


	void putPixel(int x, int y, unsigned char h) {
		CRGB col;
		col.setHSV(h, 255, 255);
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT) {
			leds[x + y * SCREEN_WIDTH] = col;
		}
	}

	void putPixel(int x, int y, CRGB c) {
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT) {
			leds[(x + y * SCREEN_WIDTH)] = c;
		}
	}

	void show() {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				int index = i + j * SCREEN_WIDTH;
				CRGB buf = leds[index];
				ofColor c;
				c.set(buf.r, buf.g, buf.b);
				ofSetColor(c);
				ofDrawRectangle(i*scale, j*scale, scale, scale);
			}
		}
		hue++;
	}
	void clear() {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				leds[i + j * SCREEN_WIDTH] = CRGB::Black;
			}
		}
	}
	void fade(uint8_t a = 128) {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				leds[i + j * SCREEN_WIDTH].fadeToBlackBy(a);
			}
		}
	}

	CRGB getColor(uint8_t offset = 0) {
		uint8_t h = (Data::getHue() + offset) % 256;
		return CHSV(h, 255, 255);
	}


	int scale = 8;
	uint8_t hue = 0;
	CRGB leds[SCREEN_WIDTH * SCREEN_HEIGHT];
};

