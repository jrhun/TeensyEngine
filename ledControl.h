#ifndef LEDS_H
#define LEDS_H

#include "data.h"

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

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))

CRGBArray < NUM_LEDS > leds;
uint8_t zBuffer[ROWS * COLS];
//CRGB scratchArray[NUM_LEDS]; // have global as ESP doesn't like variables on the stack
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;
uint8_t blendCounter = 0;
bool blendActive = false;

DEFINE_GRADIENT_PALETTE( Black_White_gp) {
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

DEFINE_GRADIENT_PALETTE( BlacK_Red_Magenta_Yellow_gp ) {
  0,   0,  0,  0,
  42,  42,  0,  0,
  84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0, 255,
  212, 255, 55, 45,
  255, 255, 255,  0
};

DEFINE_GRADIENT_PALETTE( BlacK_Magenta_Red_gp ) {
  0,   0,  0,  0,
  63,  42,  0, 45,
  127, 255,  0, 255,
  191, 255,  0, 45,
  255, 255,  0,  0
};

DEFINE_GRADIENT_PALETTE( Blue_Cyan_Yellow_gp ) {
  0,   0,  0, 255,
  63,   0, 55, 255,
  127,   0, 255, 255,
  191,  42, 255, 45,
  255, 255, 255,  0
};

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Black_White_gp,
  BlacK_Red_Magenta_Yellow_gp,
  BlacK_Magenta_Red_gp,
  Blue_Cyan_Yellow_gp
};

const uint8_t gGradientPaletteCount =
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );

const uint8_t PROGMEM gamma8[] = {
  0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,   1,   1,   1,   1,   1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,   3,   3,   3,   3,   3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,   6,   6,   6,   6,   7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10,  10,  11,  11,  11,  12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17,  17,  18,  18,  19,  19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25,  26,  27,  27,  28,  29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37,  38,  39,  39,  40,  41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51,  52,  54,  55,  56,  57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69,  70,  72,  73,  74,  75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90,  92,  93,  95,  96,  98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void adjustGamma(CRGB &colour) {
  colour.r = gamma8[colour.r];
  colour.g = gamma8[colour.g];
  colour.b = gamma8[colour.b];

  //method 2
  //  colour.r = dim8_video(colour.r);
  //  colour.g = dim8_video(colour.g);
  //  colour.b = dim8_video(colour.b);
}

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

  if ( (x >= COLS) || (y >= ROWS) )
    return LAST_VISIBLE_LED + 1;

  uint16_t j;
  uint16_t i = x * (ROWS - 2); // each column actually has 30 leds
  if (x % 2 == 0) {// even row
    if (y == 31  and x >= 16 and x < 48) {
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
      FastLED.addLeds<WS2812B, 13, GRB>(leds, 0 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 12, GRB>(leds, 1 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 14, GRB>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 27, GRB>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 26, GRB>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 25, GRB>(leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 33, GRB>(leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );
      FastLED.addLeds<WS2812B, 32, GRB>(leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalSMD5050 );

      FastLED.setBrightness( Data::brightness );
      FastLED.setTemperature( OvercastSky );
      //      FastLED.setMaxPowerInVoltsAndMilliamps(5, 5000);

      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;

      setFont(&Picopixel);
      setTextSize(1);
      setTextWrap(true);
      FastLED.setDither( 0 );
    }

    void show() {
       FastLED.show();
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
      CRGB colour = ColorFromPalette( currentPalette, Data::hue + offset);
      if (adjustGamma)
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
      } else {
        ystep = -1;
      }

      for (; x0 <= x1; x0++) {
        if (steep) {
          drawPixel(y0 % COLS, x0, color);
        } else {
          drawPixel(x0 % COLS, y0, color);
        }
        err -= dy;
        if (err < 0) {
          y0 += ystep;
          err += dx;
        }
      }
    }

    void resetZbuffer() {
      for (uint16_t i = 0; i < ROWS * COLS; i++) {
        zBuffer[i] = 0;
      }
    }
    static const int8_t zMax = 32;

    static void changeToScreenSpace(int8_t &x, int8_t &y, int8_t z) {
      // maps x/y in place to perspective
      // X/Y/Z should be centered around 0/0/0 to work correctly
      const float cameraDistance = 2;
      float p = (z + zMax) / (zMax * 2.0);// scale 0 to 1
      p = 1 / (cameraDistance - p);
      // perspective and offset to screen space mid point
      x = int8_t(x * p) + COLS / 2;
      y = int8_t(y * p) + ROWS / 2;
    }
    static void changeToScreenSpace(int8_t &x, int8_t z, bool isX = true) {
      const float cameraDistance = 2;
      float p = (z + zMax) / (zMax * 2.0);// scale 0 to 1
      p = 1 / (cameraDistance - p);
      x = int8_t(x * p) + isX ? COLS / 2 : ROWS / 2;
    }
    void drawPointZ(int8_t x, int8_t y, int8_t z, uint8_t hue) {
      uint8_t iz = map(z, -zMax, zMax, 0, 255);
      if (y >= 0 and y < ROWS) {
        uint16_t index = (x + COLS) % COLS + y * COLS;
        if (iz > zBuffer[index]) {
          zBuffer[index] = iz;
          CRGB col = CHSV((Data::getHue() + hue) % 256, map(z, -zMax, zMax, 64, 255), iz);
          drawPixel((x + COLS) % COLS, y, col);
        }
      }
    }
    void drawLineZ(int16_t x1, int16_t y1, int16_t z1, int16_t x2, int16_t y2, int16_t z2, uint8_t hue) {
      // z axis affects brightness and saturation
      // uses Z buffer
      uint8_t dx =  abs(x2 - x1);
      uint8_t dy =  abs(y2 - y1);
      uint8_t dz =  abs(z2 - z1);
      int8_t sx = x1 < x2 ? 1 : -1;
      int8_t sy = y1 < y2 ? 1 : -1;
      int8_t sz = z1 < z2 ? 1 : -1;
      uint8_t dm = max(max(dx, dy), dz);
      uint8_t i = dm;
      x2 = y2 = z2 = dm / 2;
      for (;;) {
        uint8_t z = map(z1, -zMax, zMax, 0, 255);
        if (y1 >= 0 and y1 < ROWS) {
          uint16_t index = (x1 + COLS) % COLS + y1 * COLS;
          if (z > zBuffer[index]) {
            zBuffer[index] = z;
            CRGB col = CHSV((Data::getHue() + hue + abs(dm - i)) % 256, map(z1, -zMax, zMax, 64, 255), z);
            drawPixel((x1 + COLS) % COLS, y1, col);
          }
          hue++;
        }
        if (i-- == 0) break;
        x2 -= dx;
        if (x2 < 0) {
          x2 += dm;
          x1 += sx;
        }
        y2 -= dy;
        if (y2 < 0) {
          y2 += dm;
          y1 += sy;
        }
        z2 -= dz;
        if (z2 < 0) {
          z2 += dm;
          z1 += sz;
        }
      }
    }

    size_t write(uint8_t c) {
      if (!gfxFont) { // 'Classic' built-in font

        if (c == '\n') {
          cursor_y += textsize * 8;
          cursor_x  = 0;
        } else if (c == '\r') {
          // skip em
        } else {
          if (wrap && ((cursor_x + textsize * 6) >= _width * 2)) { // Heading off edge?
            cursor_x  = 0;            // Reset x to zero
            cursor_y += textsize * 8; // Advance y one line
          }
          cursor_x %= _width; //wrap around
          drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
          cursor_x += textsize * 6;
        }

      } else { // Custom font

        if (c == '\n') {
          cursor_x  = 0;
          cursor_y += (int16_t)textsize *
                      (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if (c != '\r') {
          uint8_t first = pgm_read_byte(&gfxFont->first);
          if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
            uint8_t   c2    = c - pgm_read_byte(&gfxFont->first);
            GFXglyphFL *glyph = &(((GFXglyphFL *)pgm_read_pointer(&gfxFont->glyph))[c2]);
            uint8_t   w     = pgm_read_byte(&glyph->width),
                      h     = pgm_read_byte(&glyph->height);
            if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
              int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
              if (wrap && ((cursor_x + textsize * (xo + w)) >= _width * 2)) {
                // Drawing character would go off right edge; wrap to new line
                cursor_x  = 0;

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
        for (int16_t i = 0; i < w; i++ ) {
          uint16_t color = pgm_read_word(&bitmap[j * w + i]);
          drawPixel(x + i, y, CRGB( pgm_read_byte(&gamma5[ color >> 11       ]),
                                    pgm_read_byte(&gamma6[(color >> 5) & 0x3F]),
                                    pgm_read_byte(&gamma5[ color       & 0x1F])));
        }
      }
    }

    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint8_t bitmap[], int16_t w, int16_t h) {
      uint16_t buffOffset = 0;
      uint8_t r, g, b;
      for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++ ) {
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
        for (int16_t i = 0; i < w; i++ ) {
          //          CRGB colour = blend(fg, bg, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
          CRGB colour = blend(bg, fg, pgm_read_byte(bitmap + buffidx));
          buffidx++;
          drawPixel((x + i + COLS) % COLS, j + y, colour);
        }
      }
    }

};

Leds ledControl;

// PATTERNS

/*
  // use a x y z array
  // with a 4 sided cube (no top/bottom
  // each side represents the 32x64 led matrix
  // one side is 64/ 4 = 16 wide, and 32 tall
  // coordinates would be
  // if we have a 32*32*32 grid though that we generate once
  // then move the cube through it and interpolate between values


  split the matrix into 4 sections
  16x32, 16x32, 16x32, 16x32
  1st section FRONT
  so we would take col = x 0 to 15, row = y = 0 to 31, z = 0

  2nd section RIGHT
  col = z = 0 to 15, row = y = 0 to 31, x = 15
  3rd section BACK
  col = x = 15 to 0, row = y = 0 to 31, z = 15
  4th section LEFT
  col = z = 15 to 0, row = y = 0 to 31, x = 0

  then we also offset the whole thing by
  xOffset = 0 to 16, then zOffset = 0 to 16, then xoffset 16 to 0, then zoffset 16 to 0


*/






// -- The core to run FastLED.show()
#define FASTLED_SHOW_CORE 0

// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle = 0;

void FastLEDshowESP32()
{
  if (userTaskHandle == 0) {
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
    // -- Store the handle of the current task, so that the show task can
    //    notify it when it's done
    userTaskHandle = xTaskGetCurrentTaskHandle();

    // -- Trigger the show task
    xTaskNotifyGive(FastLEDshowTaskHandle);

    // -- Wait to be notified that it's done
    ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    userTaskHandle = 0;
  }
}

void FastLEDshowTask(void *pvParameters)
{
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
  // -- Run forever...
  for (;;) {
    // -- Wait for the trigger
    ulTaskNotifyTake(pdTRUE, xMaxBlockTime);

    // -- Do the show (synchronously)
    FastLED.show();

    // -- Notify the calling task
    xTaskNotifyGive(userTaskHandle);
  }
}




#endif //LEDS_H
