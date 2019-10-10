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

	void fill(CRGB c) {
		for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
			leds[i] = c;
		}
	}

	void blendPixel(int x, int y, CRGB c, uint8_t a=128) {
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT) {
			if (leds[(x + y * SCREEN_WIDTH)]) {// only blend if pixel is already lit
				//leds[(x + y * SCREEN_WIDTH)] = nblend(leds[(x + y * SCREEN_WIDTH)], c, a);
				leds[(x + y * SCREEN_WIDTH)] = blendlab(leds[(x + y * SCREEN_WIDTH)], c);
			}
			else
				putPixel(x, y, c);
		}
	}

	void show() {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				CRGB c0, c1, c2, c3;// c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15;
				c0 = leds[(i * 1 + 0) + (j * 1 + 0) * SCREEN_WIDTH];
				//c1 = leds[(i * 2 + 1) + (j * 2 + 0) * SCREEN_WIDTH];
				//c2 = leds[(i * 2 + 0) + (j * 2 + 1) * SCREEN_WIDTH];
				//c3 = leds[(i * 2 + 1) + (j * 2 + 1) * SCREEN_WIDTH];

				//c4 = leds[(i * 4 + 2) + (j * 4 + 0) * SCREEN_WIDTH];
				//c5 = leds[(i * 4 + 2) + (j * 4 + 1) * SCREEN_WIDTH];
				//c6 = leds[(i * 4 + 2) + (j * 4 + 2) * SCREEN_WIDTH];
				//c7 = leds[(i * 4 + 0) + (j * 4 + 2) * SCREEN_WIDTH];
				//c8 = leds[(i * 4 + 1) + (j * 4 + 2) * SCREEN_WIDTH];

				//c9 = leds[(i * 4 + 3) + (j * 4 + 0) * SCREEN_WIDTH];
				//c10 = leds[(i * 4 + 3) + (j * 4 + 1) * SCREEN_WIDTH];
				//c11 = leds[(i * 4 + 3) + (j * 4 + 2) * SCREEN_WIDTH];
				//c12 = leds[(i * 4 + 3) + (j * 4 + 3) * SCREEN_WIDTH];
				//c13 = leds[(i * 4 + 0) + (j * 4 + 3) * SCREEN_WIDTH];
				//c14 = leds[(i * 4 + 1) + (j * 4 + 3) * SCREEN_WIDTH];
				//c15 = leds[(i * 4 + 2) + (j * 4 + 3) * SCREEN_WIDTH];

				//uint16_t r, g, b;
				//r = c0.r + c1.r + c2.r + c3.r;
				//g = c0.g + c1.g + c2.g + c3.g;
				//b = c0.b + c1.b + c2.b + c3.b;
				//r /= 4;
				//g /= 4;
				//b /= 4;
				//r = c0.r + c1.r + c2.r + c3.r + c4.r + c5.r + c6.r + c7.r + c8.r + c9.r + c10.r + c11.r + c12.r + c13.r + c14.r + c15.r;
				//g = c0.g + c1.g + c2.g + c3.g + c4.g + c5.g + c6.g + c7.g + c8.g + c9.g + c10.g + c11.g + c12.g + c13.g + c14.g + c15.g;
				//b = c0.b + c1.b + c2.b + c3.b + c4.b + c5.b + c6.b + c7.b + c8.b + c9.b + c10.b + c11.b + c12.b + c13.b + c14.b + c15.b;
				//r /= 16;
				//g /= 16;
				//b /= 16;
				ofColor c;
				c.set(c0.r, c0.g, c0.b);
				ofSetColor(c);
				ofDrawRectangle(i*scale, j*scale, scale, scale);

				/*int index = i + j * SCREEN_WIDTH;
				CRGB buf = leds[index];
				ofColor c;
				c.set(buf.r, buf.g, buf.b);
				ofSetColor(c);
				ofDrawRectangle(i*scale, j*scale, scale, scale);*/
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

	//CRGB getColour(uint8_t offset = 0) {
	//	uint8_t h = (Data::getHue() + offset) % 256;

	//	return CHSV(h, 255, 255);
	//}


	int scale = 8;
	uint8_t hue = 0;
	/*CRGB leds[SCREEN_WIDTH * SCREEN_HEIGHT];*/
};

