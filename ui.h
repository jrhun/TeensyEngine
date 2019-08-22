#include <SPI.h>
#include <TFT_eSPI.h>
#include <Encoder.h> // paul strofen library

#include "data.h"
#include "ledControl.h"
#include "src\Patterns\PatternController.h"

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sText = TFT_eSprite(&tft);

#define H1Font    &FreeSansBold18pt7b
#define H2Font    &FreeSans18pt7b
#define bodyFont  &FreeSans12pt7b
// for changing values, use font 4
// digital clock like font is 7

#define TFT_BACKGROUND  0x2967
#define TFT_TEXT        0xE719
#define TFT_FPS         5

bool updateDisplay = true;
bool displayFPS = true;

// ENCODER & Buttons
Encoder encoder(EN_A_PIN, EN_B_PIN);

unsigned long lastDebounceTime = 0;
const uint8_t debounceDelay = 20;
const uint16_t  holdDelay = 800;
bool lastPinState = true; // input pullup
bool buttonState = true;

void inc() {
  patterns.inc();
  updateDisplay = true;
}

void dec() {
  patterns.dec();
  updateDisplay = true;
}

void shortPress() {

}

void longPress() {

}

// Display functions
void drawBattery() {

}

void drawPalette(uint8_t x, uint8_t y, CRGBPalette16 &palette) {
  //draws a rectangle filled with current palette with arrow highlighting current hue
  // TL = top left
  // BR = bottom right
  // 0,0 is top left
  const uint8_t h = 30;
  const uint8_t w = 220; // 240 - w / 2
  const uint8_t xStart = (240 - w) / 2;
  for (uint8_t i = xStart; i < xStart + w; i++) {
    //center current hue in middle
    CRGB c = ColorFromPalette(palette, i + 100);
    tft.drawFastVLine(i, y, h, tft.color565(c.r, c.g, c.b));
  }
  tft.drawRoundRect(x - 10, y, 20, h, 5, TFT_WHITE);
}

void tftDisplayFPS() {
  tft.setCursor(180, 300, 1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_TEXT, TFT_BACKGROUND);
  char p[10];
  sprintf(p, "FPS: %3d", FastLED.getFPS());
  tft.print(p);
}







// Main UI Functions

void uiLoop() {

  //encoder handle
  int8_t newPos = encoder.read();
  if (newPos >= + 4) {
    while (newPos >= 4) {
      inc();
      newPos -= 4;
    }
    encoder.write(newPos);
  } else if (newPos <= -4) {
    while (newPos <= -4) {
      dec();
      newPos += 4;
    }
    encoder.write(newPos);
  }

  // button press
  uint8_t reading = digitalRead(JOY_CENTRE_PIN);
  if (reading != lastPinState) {
    lastDebounceTime = millis(); //debounce or input detected
  }
  lastPinState = reading;
  if ( millis() - lastDebounceTime > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (reading == LOW) // input pull up
        shortPress();
    }
    if ( (reading == LOW) and (millis() - lastDebounceTime) > holdDelay) {
      longPress();
      lastDebounceTime = millis();
    }
  }

  // display handle
  static unsigned long lastTFTUpdate = 0;
  if (millis() - lastTFTUpdate > (1000 / TFT_FPS)) {
    lastTFTUpdate = millis();
    // fast text updates only!
  }

  if (updateDisplay) {
    updateDisplay = false;

    sText.setColorDepth(1);
    sText.createSprite(240, 320);
    sText.fillSprite(TFT_BLACK);// grey fill

    // print title
    sText.setCursor(2, 0, 1);
    sText.setTextSize(3);
    sText.setTextColor(TFT_TEXT, TFT_BLACK);
    sText.println("Radience");
    sText.drawFastHLine(0, 30, tft.width(), TFT_TEXT);

    //print body
    sText.setCursor(2, 40, 1);
    sText.setTextSize(2);
    sText.print("Current pattern\n> ");
    sText.println(patterns.getCurrentPatternName());



    sText.setBitmapColor(TFT_TEXT, TFT_BACKGROUND);
    sText.pushSprite(0, 0);
    sText.deleteSprite();

        //FPS
    if (displayFPS) {
      tftDisplayFPS();
    }
  }

  //backlight
  //  ledcWrite(0, backlightLevel); // chanel 0
}


// TODO: put uiLoop onto low priority thread on core 1 while main runs on core 0
TaskHandle_t uiTaskHandle;

void uiTask(void * parameter) {
  for (;;) {
    uiLoop();
  }
}

void uiSetup() {
  tft.begin();
  tft.setRotation(2);

  tft.fillScreen(TFT_BACKGROUND);// grey fill



  //buttons
  pinMode(JOY_CENTRE_PIN, INPUT_PULLUP);
  pinMode(JOY_UP_PIN,     INPUT_PULLUP);
  pinMode(JOY_LEFT_PIN,   INPUT_PULLUP);
  pinMode(JOY_DOWN_PIN,   INPUT_PULLUP);
  pinMode(JOY_RIGHT_PIN,  INPUT_PULLUP);

  //  xTaskCreatePinnedToCore(uiTask, "UITask", 4096, NULL, 2, &uiTaskHandle, 1);

  //backlight PWM setup (TODO: use high resolution and map to give better control at lower brightness
  //  ledcSetup(OLED_LED_PIN, 5000, 8);  // pin, freq, res 8/10/12/15
  //  ledcAttachPin(OLED_LED_PIN, 0);  // attach PWM channel to OLED pin
}
