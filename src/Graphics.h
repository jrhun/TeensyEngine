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

#include "Font.h"





class Graphics {
public:
	Graphics()  {}

	virtual void init() {}
	virtual void update() {}
	virtual void show() {}
	virtual void clear() {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				leds[i + j * SCREEN_WIDTH] = CRGB::Black;
			}
		}
	}
	virtual void fill(CRGB c) {
		for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
			leds[i] = c;
		}
	}
	void fade(uint8_t a = 128) {
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				leds[i + j * SCREEN_WIDTH].fadeToBlackBy(a);
			}
		}
	}
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
		//blur2d(leds, SCREEN_WIDTH, SCREEN_HEIGHT, amount);
		blurRows(amount); 
		blurColumns(amount); 
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

	void blur1d(CRGB* leds, uint16_t numLeds, fract8 blur_amount)
	{
		//wrap around 
		uint8_t keep = 255 - blur_amount;
		uint8_t seep = blur_amount >> 1;
		CRGB carryover = CRGB::Black;
		for (uint16_t i = 0; i <= numLeds; i++) {
			CRGB cur;
			if (i == numLeds)
				cur = leds[0];
			else
				cur = leds[i];
			CRGB part = cur;
			part.nscale8(seep);
			cur.nscale8(keep);
			cur += carryover;
			if (i) leds[i - 1] += part;
			if (i == 0) {
				leds[numLeds - 1] += part;
			}
			if (i == numLeds)
				leds[0] = cur;
			else
				leds[i] = cur;
			carryover = part;
		}
	}

	void blurRows(fract8 blur_amount)
	{
		for (uint8_t row = 0; row < SCREEN_HEIGHT; row++) {
			CRGB* rowbase = leds + (row * SCREEN_WIDTH);
			blur1d(rowbase, SCREEN_WIDTH, blur_amount);
		}
	}
	void blurColumns(fract8 blur_amount)
	{
		auto XY = [](uint8_t x, uint8_t y, uint8_t width) {
			return x + y * width;
		};
		// blur columns
		uint8_t keep = 255 - blur_amount;
		uint8_t seep = blur_amount >> 1;
		for (uint8_t col = 0; col < SCREEN_WIDTH; col++) {
			CRGB carryover = CRGB::Black;
			for (uint8_t i = 0; i < SCREEN_HEIGHT; i++) {
				CRGB cur = leds[XY(col, i, SCREEN_WIDTH)];
				CRGB part = cur;
				part.nscale8(seep);
				cur.nscale8(keep);
				cur += carryover;
				if (i) leds[XY(col, i - 1, SCREEN_WIDTH)] += part;
				leds[XY(col, i, SCREEN_WIDTH)] = cur;
				carryover = part;
			}
		}
	}


	virtual void shiftRawX(int8_t amount) {}

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
			else			targetPalette = gGradientPalettes[*paletteIndex_t-1];
		}
	}

	virtual void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {};
	virtual void putPixel(int x, int y, CRGB c) {};
	virtual void putPixel(int x, int y, unsigned char h) {};
	virtual void putPixelDirect(int x, int y, CRGB c, uint8_t opacity = 255) {};
	
	
	//virtual void blendPixel(int x, int y, CRGB c, uint8_t a=128) {};
	void blendPixel(int x, int y, CRGB c, uint8_t a = 128) {
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT) {
			if (leds[(x + y * SCREEN_WIDTH)]) {// only blend if pixel is already lit
				leds[(x + y * SCREEN_WIDTH)] = nblend(leds[(x + y * SCREEN_WIDTH)], c, a);
				//leds[(x + y * SCREEN_WIDTH)] = blendlab(leds[(x + y * SCREEN_WIDTH)], c);
			}
			else
				putPixel(x, y, c);
		}
	}
	

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


	// 16bit 565 encoded image
	void drawRGBBitmap(int16_t x, int16_t y,
		const uint16_t bitmap[], int16_t w, int16_t h) {
		for (int16_t j = 0; j < h; j++, y++) {
			for (int16_t i = 0; i < w; i++) {
				uint16_t color = pgm_read_word(&bitmap[j * w + i]);
				putPixel(x + i, y, CRGB(pgm_read_byte(&gamma5[color >> 11]),
					pgm_read_byte(&gamma6[(color >> 5) & 0x3F]),
					pgm_read_byte(&gamma5[color & 0x1F])));
			}
		}
	}

	void setTextColour(CRGB c) {
		textcolor = c;
	}

	void setCursor(uint8_t x, uint8_t y) {
		cursor_x = x;
		cursor_y = y;
	}

	void setFont(const FXfont *f) {
		if (f) {          // Font struct pointer passed in?
			if (!gfxFont) { // And no current font struct?
			  // Switching from classic to new font behavior.
			  // Move cursor pos down 6 pixels so it's on baseline.
				cursor_y += 6;
			}
		}
		else if (gfxFont) { // NULL passed.  Current font struct defined?
	   // Switching from new to classic font behavior.
	   // Move cursor pos up 6 pixels so it's at top-left of char.
			cursor_y -= 6;
		}
		gfxFont = (FXfont *)f;
	}

	size_t write(const char c) {
		if (!gfxFont) { // 'Classic' built-in font

			if (c == '\n') {
				cursor_y += textsize * 8;
				cursor_x = 0;
			}
			else if (c == '\r') {
				// skip em
			}
			else {
				if (wrap && ((cursor_x + textsize * 6) >= SCREEN_WIDTH * 2)) { // Heading off edge?
					cursor_x = 0;            // Reset x to zero
					cursor_y += textsize * 8; // Advance y one line
				}
				cursor_x %= SCREEN_WIDTH; //wrap around
				drawChar(cursor_x, cursor_y, c, textcolor, textsize);
				cursor_x += textsize * 6;
			}

		}
		else {
			if (c == '\n') {
				cursor_x = 0;
				cursor_y += (int16_t)textsize *
					(uint8_t)pgm_read_byte(&gfxFont->yAdvance);
			}
			else if (c != '\r') {
				uint8_t first = pgm_read_byte(&gfxFont->first);
				if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
					uint8_t   c2 = c - pgm_read_byte(&gfxFont->first);
					FXglyph *glyph = &(((FXglyph *)pgm_read_pointer(&gfxFont->glyph))[c2]);
					uint8_t   w = pgm_read_byte(&glyph->width),
						h = pgm_read_byte(&glyph->height);
					if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
						int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
						if (wrap && ((cursor_x + textsize * (xo + w)) >= SCREEN_WIDTH * 2)) {
							// Drawing character would go off right edge; wrap to new line
							cursor_x = 0;
							cursor_y += (int16_t)textsize *
								(uint8_t)pgm_read_byte(&gfxFont->yAdvance);
						}
						cursor_x %= SCREEN_WIDTH; //wrap around
						drawChar(cursor_x, cursor_y, c, textcolor, textsize);
					}
					cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
				}
			}
		}
		return 1;
	}

	size_t write(const char *buffer, size_t size) {
		size_t n = 0;
		while (size--) {
			size_t ret = write(pgm_read_byte(buffer++));
			if (ret == 0) {
				// Write of last byte didn't complete, abort additional processing
				break;
			}
			n += ret;
		}
		return n;
	}

	void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, CRGB c) {
		for (uint8_t i = 0; i < w; i++) {
			for (uint8_t j = 0; j < h; j++) {
				putPixel(x + i, y + j, c);
			}
		}
	}

	void fillRectDirect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, CRGB c) {
		for (uint8_t i = 0; i < w; i++) {
			for (uint8_t j = 0; j < h; j++) {
				putPixelDirect(x + i, y + j, c);
			}
		}
	}


	void drawChar(int16_t x, int16_t y, unsigned char c,
		CRGB color, uint8_t size) {

		//color = color.scale8(textOpacity);

		if (!gfxFont) { // 'Classic' built-in font

			if (x + 6 * size - 1 < 0) {
				x = x;
			}

			if ((x >= SCREEN_WIDTH) || // Clip right
				(y >= SCREEN_HEIGHT) || // Clip bottom
				((x + 6 * size - 1) < 0) || // Clip left
				((y + 8 * size - 1) < 0))   // Clip top
				return;

			if ((c >= 176)) c++; // Handle 'classic' charset behavior

			for (int8_t i = 0; i < 6; i++) {
				uint8_t line;
				if (i < 5) line = pgm_read_byte(font + (c * 5) + i);
				else      line = 0x0;
				for (int8_t j = 0; j < 8; j++, line >>= 1) {
					if (line & 0x1) {
						if (size == 1) putPixelDirect((x + i) % SCREEN_WIDTH, y + j, color, textOpacity);
						else          fillRectDirect( (x + (i*size)) % SCREEN_WIDTH, y + (j*size), size, size, color);
					}
					//else if (bg != color) {
					//	if (size == 1) putPixel(x + i, y + j, bg);
					//	else          fillRect(x + i * size, y + j * size, size, size, bg);
					//}
				}
			}

		}
		else { // Custom font

	   // Character is assumed previously filtered by write() to eliminate
	   // newlines, returns, non-printable characters, etc.  Calling drawChar()
	   // directly with 'bad' characters of font may cause mayhem!

			c -= pgm_read_byte(&gfxFont->first);
			FXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
			uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

			uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
			uint8_t  w = pgm_read_byte(&glyph->width),
				h = pgm_read_byte(&glyph->height);
			// xa = pgm_read_byte(&glyph->xAdvance);
			int8_t   xo = pgm_read_byte(&glyph->xOffset),
				yo = pgm_read_byte(&glyph->yOffset);
			uint8_t  xx, yy, bits = 0, bit = 0;
			int16_t  xo16 = 0, yo16 = 0;

			if (size > 1) {
				xo16 = xo;
				yo16 = yo;
			}

			for (yy = 0; yy < h; yy++) {
				for (xx = 0; xx < w; xx++) {
					if (!(bit++ & 7)) {
						bits = pgm_read_byte(&bitmap[bo++]);
					}
					if (bits & 0x80) {
						if (size == 1) {
							putPixelDirect( (x + xo + xx) % SCREEN_WIDTH, y + yo + yy, color, textOpacity);
						}
						else {
							fillRectDirect( (x + (xo16 + xx)*size ) % SCREEN_WIDTH, y + (yo16 + yy)*size, size, size, color);
						}
					}
					bits <<= 1;
				}
			}

		} // End classic vs custom font
	}

	size_t print(const String &s) {
		return write(s.c_str(), s.length());
	}

	//rotate RGB colour by hue
	//inline CRGB rotate(const float hue = 60.0) {
	//	//call rotateCalculate first to set matrix properly
	//	CRGB out; 
	//	//Use the rotation matrix to convert the RGB directly
	//	CRGB in = CRGB(r, g, b);
	//	const float cosA = cos(hue * PI / 180.0);
	//	const float sinA = sin(hue * PI / 180.0);
	//	float matrix[3][3] = { {cosA + (1.0f - cosA) / 3.0f,		1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA,	 1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA},
	//		{1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA,	cosA + 1.0f / 3.0f * (1.0f - cosA),		1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA},
	//		{1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA,	1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA,	cosA + 1.0f / 3.0f * (1.0f - cosA)} };

	//	out.r = CLAMP(in.r * matrix[0][0] + in.g * matrix[0][1] + in.b * matrix[0][2], 0, 255);
	//	out.g = CLAMP(in.r * matrix[1][0] + in.g * matrix[1][1] + in.b * matrix[1][2], 0, 255);
	//	out.b = CLAMP(in.r * matrix[2][0] + in.g * matrix[2][1] + in.b * matrix[2][2], 0, 255);
	//	return out;
	//}
	inline CRGB rotate(CRGB in) {
		//call calculate rotate first
		CRGB out;
		out.r = CLAMP(in.r * matrix[0][0] + in.g * matrix[0][1] + in.b * matrix[0][2], 0, 255);
		out.g = CLAMP(in.r * matrix[1][0] + in.g * matrix[1][1] + in.b * matrix[1][2], 0, 255);
		out.b = CLAMP(in.r * matrix[2][0] + in.g * matrix[2][1] + in.b * matrix[2][2], 0, 255);
		return out;
	}
	static inline void rotateCalculate(const float hue) {
		const float cosA = cos(hue * PI / 180.0);
		const float sinA = sin(hue * PI / 180.0);
		//calculate the rotation matrix, only depends on Hue
		//https://stackoverflow.com/questions/8507885/shift-hue-of-an-rgb-color
		matrix[0][0] = cosA + (1.0f - cosA) / 3.0f;
		matrix[0][1] = 1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA;
		matrix[0][2] = 1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA;

		matrix[1][0] = 1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA;
		matrix[1][1] = cosA + 1.0f / 3.0f * (1.0f - cosA);
		matrix[1][2] = 1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA;

		matrix[2][0] = 1.0f / 3.0f * (1.0f - cosA) - sqrtf(1.0f / 3.0f) * sinA;
		matrix[2][1] = 1.0f / 3.0f * (1.0f - cosA) + sqrtf(1.0f / 3.0f) * sinA;
		matrix[2][2] = cosA + 1.0f / 3.0f * (1.0f - cosA);
	}

	// matrix for calculating hue transformation
	//save calculation - only need to calculate once for hue then can use for all pixels!
	static float matrix[3][3];

	CRGB textcolor = CRGB::White;
	uint8_t textOpacity = 128;
	uint8_t textSpeed = 20;
	uint8_t textsize = 1;
	uint8_t cursor_x = 0;
	uint8_t cursor_y = 0;
	bool wrap = true;
	FXfont *gfxFont;// = (FXfont *)FreeSans12pt7b;

	//f;
	
	
	float zBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

	CRGBPalette16	currentPalette;// = RainbowColors_gp;
	CRGBPalette16	targetPalette;// = RainbowColors_p;
	TBlendType		currentBlending = LINEARBLEND;

	CRGBArray < SCREEN_WIDTH * SCREEN_HEIGHT > leds;

};

float Graphics::matrix[3][3] = { {0,0,0},{0,0,0},{0,0,0}, };


