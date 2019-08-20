#ifndef LEDS_H
#define LEDS_H

#include "Data.h"
#include <FastLED.h>
#include <FastLED_GFX.h>
#include <Fonts/PicopixelFL.h>

#define ROWS                32
#define COLS                64
#define NUM_LEDS            ROWS*COLS
#define NUM_STRIPS          8
#define NUM_LEDS_PER_STRIP  240  //we have 30 * 8 leds per strip
#define LAST_VISIBLE_LED    1920  //(NUM_STRIPS * NUM_LEDS_PER_STRIP)
//#define FPS                 60

//#ifndef pgm_read_byte(addr)
//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
//#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
//#endif

CRGBArray < NUM_LEDS > leds;
//CRGB scratchArray[NUM_LEDS]; // have global as ESP doesn't like variables on the stack
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;
uint8_t blendCounter = 0;
bool blendActive = false;



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

	if ((x >= COLS) || (y >= ROWS))
		return LAST_VISIBLE_LED + 1;

	uint16_t j;
	uint16_t i = x * (ROWS - 2); // each column actually has 30 leds
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
		j = (ROWS - 1) - y; //

	}
	return (i + j);
}


class Leds : public FastLED_GFX {
public:
	Leds() : FastLED_GFX(COLS, ROWS) {}

	void init() {
		FastLED.addLeds<WS2812B, 13, GRB>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 12, GRB>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 14, GRB>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 27, GRB>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 26, GRB>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 25, GRB>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 33, GRB>(leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);
		FastLED.addLeds<WS2812B, 32, GRB>(leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection(TypicalSMD5050);

		FastLED.setBrightness(Data::brightness);
		FastLED.setTemperature(OvercastSky);
		//      FastLED.setMaxPowerInVoltsAndMilliamps(5, 5000);

		currentPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;

		setFont(&Picopixel);
		setTextSize(1);
		setTextWrap(true);
		FastLED.setDither(0);
	}

	void drawPixel(int16_t x, int16_t y, CRGB color) {
		leds[XY(x, y)] = color;
	}
	void drawPixel(int16_t x, int16_t y, uint8_t hue) {
		leds[XY(x, y)] = getColour(hue);
	}

	void drawPixel(int16_t x, int16_t y, CRGB color, uint8_t amount) {
		if (leds[XY(x, y)])
			leds[XY(x, y)] = blend(leds[XY(x, y)], color, amount);
		else
			leds[XY(x, y)] = color.nscale8(amount);
	}

	void setRow(uint8_t row, CRGB colour) {
		for (uint8_t x = 0; x < COLS; x++) {
			leds[XY(x, row)] = colour;
		}
	}

	void setRowRainbow(uint8_t row, uint8_t hue) {
		CRGB *rowReference[COLS];

		for (uint8_t x = 0; x < COLS; x++) {
			rowReference[x] = &leds[XY(x, row)];
		}
		fill_rainbow(rowReference[0], COLS, hue);
	}

	void setCol(uint8_t col, CRGB colour) {
		for (uint8_t y = 0; y < ROWS; y++) {
			leds[XY(col, y)] = colour;
		}
	}

	void setColRainbow(uint8_t col, uint8_t hue) {
		CRGB *colReference[ROWS];
		for (uint8_t y = 0; y < ROWS; y++) {
			colReference[y] = &leds[XY(col, y)];
		}
		fill_rainbow(colReference[0], ROWS, hue);
	}

	CRGB getColour(uint8_t offset = 0) {
		CRGB colour = ColorFromPalette(currentPalette, Data::hue + offset);
		if (Data::adjustGamma)
			adjustGamma(colour);
		return colour;
	}

	void drawBlend(uint8_t row, uint8_t col, CRGB colour, uint8_t blendAmount) {
		if (leds[XY(row, col)]) // only blend if pixel is already lit
			nblend(leds[XY(row, col)], colour, blendAmount);
		else
			drawPixel(row, col, colour);
	}

	void fade(uint8_t amount = Data::fadeAmount) {
		leds.fadeToBlackBy(amount);
	}

	void clear() {
		fillScreen(CRGB::Black);
	}

	void fillScreen(CRGB color) {
		leds.fill_solid(color);
	}

	bool blendOnDrawPixel = false;

	// GFX OVERRIDDEN CLASSES BELOW
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color) {
		int16_t steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep) {
			adagfxswap(x0, y0);
			adagfxswap(x1, y1);
		}

		if (x0 > x1) {
			adagfxswap(x0, x1);
			adagfxswap(y0, y1);
		}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if (y0 < y1) {
			ystep = 1;
		}
		else {
			ystep = -1;
		}

		for (; x0 <= x1; x0++) {
			if (steep) {
				drawPixel(y0 % COLS, x0, color);
			}
			else {
				drawPixel(x0 % COLS, y0, color);
			}
			err -= dy;
			if (err < 0) {
				y0 += ystep;
				err += dx;
			}
		}
	}

	size_t write(uint8_t c) {
		if (!gfxFont) { // 'Classic' built-in font

			if (c == '\n') {
				cursor_y += textsize * 8;
				cursor_x = 0;
			}
			else if (c == '\r') {
				// skip em
			}
			else {
				if (wrap && ((cursor_x + textsize * 6) >= _width * 2)) { // Heading off edge?
					cursor_x = 0;            // Reset x to zero
					cursor_y += textsize * 8; // Advance y one line
				}
				cursor_x %= _width; //wrap around
				drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
				cursor_x += textsize * 6;
			}

		}
		else { // Custom font

			if (c == '\n') {
				cursor_x = 0;
				cursor_y += (int16_t)textsize *
					(uint8_t)pgm_read_byte(&gfxFont->yAdvance);
			}
			else if (c != '\r') {
				uint8_t first = pgm_read_byte(&gfxFont->first);
				if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
					uint8_t   c2 = c - pgm_read_byte(&gfxFont->first);
					GFXglyphFL *glyph = &(((GFXglyphFL *)pgm_read_pointer(&gfxFont->glyph))[c2]);
					uint8_t   w = pgm_read_byte(&glyph->width),
						h = pgm_read_byte(&glyph->height);
					if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
						int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
						if (wrap && ((cursor_x + textsize * (xo + w)) >= _width * 2)) {
							// Drawing character would go off right edge; wrap to new line
							cursor_x = 0;

							cursor_y += (int16_t)textsize *
								(uint8_t)pgm_read_byte(&gfxFont->yAdvance);
						}
						cursor_x %= _width; //wrap around
						drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
					}
					cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
				}
			}

		}
		return 1;
	}

	void drawRGBBitmap(int16_t x, int16_t y,
		const uint16_t bitmap[], int16_t w, int16_t h) {
		for (int16_t j = 0; j < h; j++, y++) {
			for (int16_t i = 0; i < w; i++) {
				uint16_t color = pgm_read_word(&bitmap[j * w + i]);
				drawPixel(x + i, y, CRGB(pgm_read_byte(&gamma5[color >> 11]),
					pgm_read_byte(&gamma6[(color >> 5) & 0x3F]),
					pgm_read_byte(&gamma5[color & 0x1F])));
			}
		}
	}

	void drawRGBBitmap(int16_t x, int16_t y,
		const uint8_t bitmap[], int16_t w, int16_t h) {
		uint16_t buffOffset = 0;
		uint8_t r, g, b;
		for (int16_t j = 0; j < h; j++, y++) {
			for (int16_t i = 0; i < w; i++) {
				r = pgm_read_word(&bitmap[buffOffset++]);
				g = pgm_read_word(&bitmap[buffOffset++]);
				b = pgm_read_word(&bitmap[buffOffset++]);
				drawPixel(x + i, y, CRGB(r, g, b));
			}
		}
	}

	// draw gradient scale between two colours fg and bg
	void drawGrayscaleBitmap(int16_t x, int16_t y,
		const uint8_t bitmap[], int16_t w, int16_t h, CRGB fg, CRGB bg) {
		// col = x * cos( uint8_t((y - COLS/2)*6) );
		//          uint16_t iOffset = ((x+i) * cos8( uint8_t(y-ROWS/2)*6 )) / 256;
		// therefore, if x = 0, i = 10, y = 0iOffset =
		// 10 * cos8(-96=160) / 256 = 6. a 4 unit reduction
		// but then we also need to blend with the other 4 pixels... otherwise lastone takes preference
		uint16_t buffidx = 0;
		for (int16_t j = 0; j < h; j++) {
			for (int16_t i = 0; i < w; i++) {
				//          CRGB colour = blend(fg, bg, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
				CRGB colour = blend(bg, fg, pgm_read_byte(bitmap + buffidx));
				buffidx++;
				drawPixel((x + i + COLS) % COLS, j + y, colour);
			}
		}
	}

};

Leds ledControl;


DEFINE_GRADIENT_PALETTE(Black_White_gp) {
	0, 255, 255, 255,
		32, 0, 0, 0,
		64, 0, 0, 0,
		96, 0, 0, 0,
		128, 255, 255, 255,
		160, 0, 0, 0,
		192, 0, 0, 0,
		224, 0, 0, 0,
		255, 255, 255, 255
};

DEFINE_GRADIENT_PALETTE(BlacK_Red_Magenta_Yellow_gp) {
	0, 0, 0, 0,
		42, 42, 0, 0,
		84, 255, 0, 0,
		127, 255, 0, 45,
		170, 255, 0, 255,
		212, 255, 55, 45,
		255, 255, 255, 0
};

DEFINE_GRADIENT_PALETTE(BlacK_Magenta_Red_gp) {
	0, 0, 0, 0,
		63, 42, 0, 45,
		127, 255, 0, 255,
		191, 255, 0, 45,
		255, 255, 0, 0
};

DEFINE_GRADIENT_PALETTE(Blue_Cyan_Yellow_gp) {
	0, 0, 0, 255,
		63, 0, 55, 255,
		127, 0, 255, 255,
		191, 42, 255, 45,
		255, 255, 255, 0
};

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Black_White_gp,
  BlacK_Red_Magenta_Yellow_gp,
  BlacK_Magenta_Red_gp,
  Blue_Cyan_Yellow_gp
};

const uint8_t gGradientPaletteCount =
sizeof(gGradientPalettes) / sizeof(TProgmemRGBGradientPalettePtr);

#endif //LEDS_H