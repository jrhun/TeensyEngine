#pragma once

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH	64
#endif
#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT	32
#endif

#include "Vec3.h"
#include "Geometry.h"
#include <limits>
#include "Color.h"
#include "Palettes.h"

#if defined(ESP32) || defined(CORE_TEENSY)
#include "FastLED.h"
#else
#include "FastLED_PC.h"
#endif

class Graphics {
public:
	Graphics()  {}

	virtual void init() {}
	virtual void show() {}
	virtual void clear() {}
	virtual void fill(CRGB c) {}
	virtual void fade(uint8_t a = 128) {}
	virtual CRGB getColour(uint8_t offset = 0, uint8_t bri = 255) {
		uint8_t h = (Data::getHue() + offset) % 256;
		return ColorFromPalette(currentPalette, h, bri, currentBlending);
		//return CRGB::Black;
	}

	virtual void setColorTemp(uint32_t i) {}

	uint8_t IncAmount = 24;

	void updatePalette() {
		nblendPaletteTowardPalette(currentPalette, targetPalette, IncAmount);
	}

	void blur(uint8_t amount) {
		blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, amount);
	}

	uint16_t myXY(int16_t x, int16_t y) {
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT)
			return x + y * SCREEN_WIDTH;
		return 0;
	}

	void spiral(int x, int y, int r, byte dimm) {
		for (int d = r; d >= 0; d--) { // from the outside to the inside
			for (int i = x - d; i <= x + d; i++) {
				leds[myXY(i, y - d)] += leds[myXY(i + 1, y - d)]; // lowest row to the right
				leds[myXY(i, y - d)].nscale8(dimm);
			}
			for (int i = y - d; i <= y + d; i++) {
				leds[myXY(x + d, i)] += leds[myXY(x + d, i + 1)]; // right colum up
				leds[myXY(x + d, i)].nscale8(dimm);
			}
			for (int i = x + d; i >= x - d; i--) {
				leds[myXY(i, y + d)] += leds[myXY(i - 1, y + d)]; // upper row to the left
				leds[myXY(i, y + d)].nscale8(dimm);
			}
			for (int i = y + d; i >= y - d; i--) {
				leds[myXY(x - d, i)] += leds[myXY(x - d, i - 1)]; // left colum down
				leds[myXY(x - d, i)].nscale8(dimm);
			}
		}
	}

	//void incPalette() {
	//	*paletteIndex_t = (*paletteIndex_t + 1) % gGradientPaletteCount;
	//	setPalette(*paletteIndex_t);
	//}

	//void decPalette() {
	//	*paletteIndex_t = (*paletteIndex_t - 1 + gGradientPaletteCount) % gGradientPaletteCount;
	//	setPalette(*paletteIndex_t);
	//}
	void setPalette(uint8_t i) {
		if (i < gGradientPaletteCount) {
			if (i == 0)		targetPalette = RainbowColors_p;
			else			targetPalette = gGradientPalettes[*paletteIndex_t+1];
		}
	}

	virtual void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {};
	virtual void putPixel(int x, int y, CRGB c) {};
	virtual void putPixel(int x, int y, unsigned char h) {};
	virtual void blendPixel(int x, int y, CRGB c, uint8_t a=128) {};


	void drawPointDepth(int16_t x, int16_t y, int16_t z, unsigned char r, unsigned char g, unsigned char b) {
		drawPointDepth(x, y, z, CRGB(r, g, b));
	}

	void drawPointDepth(Vec3 &v, unsigned char r, unsigned char g, unsigned char b) {
		drawPointDepth(v, CRGB(r, g, b));
	}

	void drawPointDepth(Vec3 &v, CRGB c) {
		if (testAndSetZ(int16_t(v.x), v.y, v.z)) {
			c.nscale8_video(v.z);
			putPixel(int16_t(v.x) % SCREEN_WIDTH, v.y, c);
		}
	}
	void drawPointDepth(int16_t x, int16_t y, int16_t z, CRGB c) {
		if (testAndSetZ(x, y, z)) {
			c.nscale8_video(z);
			putPixel(x% SCREEN_WIDTH, y, c);
		}
	}


	//void drawLineDepth(Vec3 &v0, Vec3 &v1, unsigned char r, unsigned char g, unsigned char b) {
	//	drawLineDepth(v0, v1, CRGB(r, g, b));
	//}

	void drawLineDepth(Vec3 &v0, Vec3 &v1, CRGB c) {
		int16_t x0 = v0.x;

		int16_t y0 = v0.y;
		int16_t z0 = v0.z;
		int16_t x1 = v1.x;
		if (x0 > x1) {
			if (x0 - x1 > 32)
				x1 += SCREEN_WIDTH;
		}
		else {
			if (x1 - x0 > 32)
				x0 += SCREEN_WIDTH;
		}
		int16_t y1 = v1.y;
		int16_t z1 = v1.z;

		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dz = abs(z1 - z0);
		int16_t sz = z0 < z1 ? 1 : -1;
		int16_t dm = max(max(dx, dy), dz);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;
		z1 = dm / 2; /* error offset */

		for (;;) { /* loop */
			//drawPointDepth((x0+SCREEN_WIDTH) % SCREEN_WIDTH,y0,z0, r, g, b);
			drawPointDepth((x0), y0, z0, c);
			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
			z1 -= dz;
			if (z1 < 0) {
				z1 += dm;
				z0 += sz;
			}
		}
	}
	void drawLineDepth(Vec3 &v0, Vec3 &v1, uint16_t hue1, uint16_t hue2, uint8_t fade=255) {
		int16_t x0 = v0.x;
		int16_t y0 = v0.y;
		int16_t z0 = v0.z;
		int16_t x1 = v1.x;
		if (x0 > x1) {
			if (x0 - x1 > 32)
				x1 += SCREEN_WIDTH;
		}
		else {
			if (x1 - x0 > 32)
				x0 += SCREEN_WIDTH;
		}
		int16_t y1 = v1.y;
		int16_t z1 = v1.z;

		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dz = abs(z1 - z0);
		int16_t sz = z0 < z1 ? 1 : -1;
		int16_t dm = max(max(dx, dy), dz);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;
		z1 = dm / 2; /* error offset */

		CRGB c;
		float delta = hue2 - hue1;
		float hue = hue1;
		float step = delta / dm;
		for (;;) { /* loop */
			//c.setHSV(hue1 + step, myMap(z0, 0, 255, 150, 255), 255);
			//c = getColour(hue);
			//c.nscale8_video(fade);
			uint8_t h = Data::getHue() + hue;
			c = getColour(h, fade);
			//c = CHSV(h, myMap(z0, 0, 255, 128, 255), fade);
			hue += step;
			if (hue < 0)
				hue += 255;
			if (hue > 255)
				hue -= 255;
			drawPointDepth((x0), y0, z0, c);
			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
			z1 -= dz;
			if (z1 < 0) {
				z1 += dm;
				z0 += sz;
			}
		}
	}

	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t hue1, uint16_t hue2) {
		//if (x0 > x1) {
		//	if (x0 - x1 > 32)
		//		x1 += SCREEN_WIDTH;
		//}
		//else {
		//	if (x1 - x0 > 32)
		//		x0 += SCREEN_WIDTH;
		//}

		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dm = max(dx, dy);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;

		CRGB c;
		float delta = hue2 - hue1;
		float hue = hue1;
		float step = delta / dm;
		for (;;) { /* loop */
			c = getColour(hue);
//            c.nscale8(fade);
			hue += step;
			if (hue < 0)
				hue += 255;
			if (hue > 255)
				hue -= 255;
			putPixel((x0 + SCREEN_WIDTH) % SCREEN_WIDTH, (y0 + SCREEN_HEIGHT) % SCREEN_HEIGHT, c);
			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
		}
	}
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB c) {
		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dm = max(dx, dy);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;

		for (;;) { /* loop */
			
			putPixel(x0, y0, c);
			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
		}
	}

	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB c1, CRGB c2) {
		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t dm = max(dx, dy);
		int16_t i = dm; /* maximum difference */
		x1 = dm / 2;
		y1 = dm / 2;
		const float interpolateSteps = 1.0f / dm;
		Vec3 v1 = toVec(c1); 
		Vec3 v2 = toVec(c2);

		for (;;) { /* loop */
			
			putPixel(x0, y0, toCRGB(interpolate(v1, v2, i * interpolateSteps)) );
			if (i-- == 0) break;
			x1 -= dx;
			if (x1 < 0) {
				x1 += dm;
				x0 += sx;
			}
			y1 -= dy;
			if (y1 < 0) {
				y1 += dm;
				y0 += sy;
			}
		}
	}
	//void drawTriangle()
	void drawTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c) {
		const Vec3* pv0 = &v0;
		const Vec3* pv1 = &v1;
		const Vec3* pv2 = &v2;

		// sorting vertices by y
		if (pv1->y < pv0->y) std::swap(pv0, pv1);
		if (pv2->y < pv1->y) std::swap(pv1, pv2);
		if (pv1->y < pv0->y) std::swap(pv0, pv1);

		if (pv0->y == pv1->y) // natural flat top
		{
			// sorting top vertices by x
			if (pv1->x < pv0->x) std::swap(pv0, pv1);
			drawFlatTopTriangle(*pv0, *pv1, *pv2, c);
		}
		else if (pv1->y == pv2->y) // natural flat bottom
		{
			// sorting bottom vertices by x
			if (pv2->x < pv1->x) std::swap(pv1, pv2);
			drawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
		}
		else // general triangle
		{
			// find splitting vertex
			const float alphaSplit =
				(pv1->y - pv0->y) /
				(pv2->y - pv0->y);
			const Vec3 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

			if (pv1->x < vi.x) // major right
			{
				drawFlatBottomTriangle(*pv0, *pv1, vi, c);
				drawFlatTopTriangle(*pv1, vi, *pv2, c);
			}
			else // major left
			{
				drawFlatBottomTriangle(*pv0, vi, *pv1, c);
				drawFlatTopTriangle(vi, *pv1, *pv2, c);
			}
		}
	}



	void drawFlatTopTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c)
	{
		
		// calulcate slopes in screen space
		float m0 = (v2.x - v0.x) / (v2.y - v0.y);
		float m1 = (v2.x - v1.x) / (v2.y - v1.y);

		// calculate start and end scanlines
		const int yStart = (int)ceil(v0.y - 0.5f);
		const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

		float dzdy1 = ((v2.z - v0.z) / (v2.y - v0.y));
		float dzdy2 = ((v2.z - v1.z) / (v2.y - v1.y));

		for (int y = yStart; y < yEnd; y++)
		{
			// caluclate start and end points (x-coords)
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

			// calculate start and end pixels
			const int xStart = (int)ceil(px0 - 0.5f);
			const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

			float pz1 = ((y - yStart) * dzdy1 + v0.z);
			float pz2 = ((y - yStart) * dzdy2 + v1.z);
			float zStep = (pz2 - pz1) / (px1 - px0);

			for (int x = xStart; x < xEnd; x++)	{
				float z = zStep * (x - xStart) + pz1;
				drawPointDepth(x,y,z, c.x, c.y, c.z);
			}
		}
	}

	void drawFlatBottomTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Vec3 &c)
	{
		// calulcate slopes in screen space
		float m0 = (v1.x - v0.x) / (v1.y - v0.y);
		float m1 = (v2.x - v0.x) / (v2.y - v0.y);

		// calculate start and end scanlines
		const int yStart = (int)ceil(v0.y - 0.5f);
		const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

		float zxm1 = ((v1.z - v0.z) / (v1.y - v0.y));
		float zxm2 = ((v2.z - v0.z) / (v2.y - v0.y));

		for (int y = yStart; y < yEnd; y++)
		{
			// caluclate start and end points
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

			// calculate start and end pixels
			const int xStart = (int)ceil(px0 - 0.5f);
			const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

			float pz1 = ((y - yStart) * zxm1 + v0.z);
			float pz2 = ((y - yStart) * zxm2 + v0.z);
			float zStep = (pz2 - pz1) / (px1 - px0);

			for (int x = xStart; x < xEnd; x++)	{
				float z = zStep * (x - xStart) + pz1;
				drawPointDepth(x, y, z, c.x, c.y, c.z);
			}
		}
	}

	void resetZ() {
		for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
			zBuffer[i] = 0;// std::numeric_limits<float>::infinity();
		}
	}
	

	bool testAndSetZ(int16_t x, int16_t y, float depth) {
		x %= SCREEN_WIDTH;
		if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
			float &val = zBuffer[x + y * SCREEN_WIDTH];
			if (depth > val) {
				val = depth;
				return true;
			}
		}
		return false;
	}



	float zBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	
	CRGBPalette16	currentPalette;// = RainbowColors_gp;
	CRGBPalette16	targetPalette;// = RainbowColors_p;
	TBlendType		currentBlending = LINEARBLEND;
	
	CRGBArray < SCREEN_WIDTH * SCREEN_HEIGHT > leds;

};
