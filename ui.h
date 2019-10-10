#include <SPI.h>
#include <Encoder.h> // paul strofen library

#include "data.h"
#include "ledControl.h"
#include "src\Patterns\PatternController.h"

// Use hardware SPI
#if defined(ESP32)
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sText = TFT_eSprite(&tft);

#define H1Font    &FreeSansBold18pt7b
#define H2Font    &FreeSans18pt7b
#define bodyFont  &FreeSans12pt7b
// for changing values, use font 4
// digital clock like font is 7
#define TFT_BACKGROUND  0x2967
#define TFT_TEXT        0xE719

#else //teensy 

#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"
#define TFT_BACKGROUND  0x2967
#define TFT_TEXT        0xE719
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);
#endif


#define TFT_FPS         5
bool updateDisplay = true;
bool displayFPS = true;

#include "src\Menu.h"

// ENCODER & Buttons
Encoder encoder(EN_A_PIN, EN_B_PIN);

const uint8_t debounceDelay = 20;
const uint16_t  holdDelay = 800;
const uint8_t numButtons = 5;
unsigned long lastDebounceTime[numButtons] = {0, 0, 0, 0, 0};
bool buttonState[numButtons] = {true, true, true, true, true};
bool lastPinState[numButtons] = {true, true, true, true, true}; // input pullup
enum {JOY_CENTRE, JOY_UP, JOY_LEFT, JOY_DOWN, JOY_RIGHT};
uint8_t buttonPins[numButtons] = {JOY_CENTRE_PIN, JOY_UP_PIN, JOY_LEFT_PIN, JOY_DOWN_PIN, JOY_RIGHT_PIN};

void inc() {
  //  patterns.inc();
  menu.inc();
  updateDisplay = true;
}

void dec() {
  //  patterns.dec();
  menu.dec();
  updateDisplay = true;
}
void centre(bool p) {
  menu.press();
}
void up(bool p) {
  menu.up();
}
void left(bool p) {
  menu.left();
}
void down(bool p) {
  menu.down();
}
void right(bool p) {
  menu.right();
}

typedef void (*buttonFn) (bool p);
buttonFn buttonActions[numButtons] = {
  &centre, &up, &left, &down, &right
};


// Main UI Functions
void tftDisplayFPS();

void uiLoop() {
  unsigned long now = millis();

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

  for (uint8_t i = 0; i < numButtons; i++) {
    uint8_t reading = digitalRead(buttonPins[i]);
    if (reading != lastPinState[i]) {
      lastDebounceTime[i] = now; //debounce or input detected
    }
    lastPinState[i] = reading;
    if ( now - lastDebounceTime[i] > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (reading == LOW) {// input pull up
          buttonActions[i](false);//shortPress();
          updateDisplay = true;
          Serial.print("Press: ");
          Serial.println(i);
        }
      }
      if ( (reading == LOW) and (now - lastDebounceTime[i]) > holdDelay) {
        buttonActions[i](true);// true for longPress();
        lastDebounceTime[i] = now;
      }
    }
  }



  // display handle
  static unsigned long lastTFTUpdate = 0;
  if (now - lastTFTUpdate > (1000 / 60)) {
    lastTFTUpdate = now;
    // fast text updates only!
    //FPS
    if (displayFPS) {
      tftDisplayFPS();
    }
  }

  if (updateDisplay) {
    updateDisplay = false;

//    tft.fillScreen(ILI9341_BLACK);
    // print title
    const uint8_t headingOffset = 2;
    tft.setCursor(0, headingOffset);
    tft.setTextSize(3);
    tft.setTextColor(TFT_TEXT);//TFT_BACKGROUND);
    tft.println("Radience");
    const uint8_t headingBottom = tft.fontCapHeight() + headingOffset;
    tft.drawFastHLine(0, headingBottom + 2, tft.width(), TFT_TEXT);

    //print page name
    tft.setCursor(2, headingBottom + 6);
    tft.setTextSize(2);
    tft.fillRect(0, headingBottom + 4, tft.width(), tft.fontCapHeight()+2, ILI9341_BLACK);
    //    tft.print("Current pattern\n> ");
    tft.println(menu.currentPage()->getName());    
    for (uint8_t i = 0; i < menu.numPages; i++) {
      const uint8_t xPos = tft.width() - 52;
      const uint8_t yPos = headingBottom + 2 + 2 + 2;
      if (i == menu.currentPageIndex) {
        tft.fillRoundRect(xPos + i * 10, yPos, 8, tft.fontCapHeight(), 2, TFT_TEXT);
      }
      else {
        tft.drawRoundRect(xPos + i * 10, yPos, 8, tft.fontCapHeight(), 2, TFT_TEXT - 0x0841);
      }
    }
    tft.drawFastHLine(0, headingBottom + tft.fontCapHeight() + 9, tft.width(), TFT_TEXT);
    

    //Print body
    const uint8_t bodyY = headingBottom + tft.fontCapHeight() + 12;
    tft.setCursor(0, bodyY);
    
    tft.fillRect(0, bodyY, tft.width(), tft.height() - bodyY, ILI9341_BLACK);
    tft.println(menu.currentPage()->getPageData());
    //    String body = menu.currentPage()->getPageData();
    //    uint8_t index = 0;
    //    String temp = "";
    //    for (int i = 0; i < body.length(); i++) {
    //      char c = body.charAt(i);
    //      if (c == '\n') {
    //        tft.println(temp);
    //        temp = "";
    //      } else {
    //        if (c == '\t'){
    //          temp += " ";
    //          }
    //        else {
    //          temp += c;
    //        }
    //      }
    //    }



  }
}


// Display functions
void drawBattery() {

}

void drawPalette(uint8_t x, uint8_t y, CRGBPalette16 &palette) {
  //draws a rectangle filled with current palette with arrow highlighting current hue
  // TL = top left
  // BR = bottom right
  // 0,0 is top left
  //  const uint8_t h = 30;
  const uint8_t w = 220; // 240 - w / 2
  const uint8_t xStart = (240 - w) / 2;
  for (uint8_t i = xStart; i < xStart + w; i++) {
    //center current hue in middle
    CRGB c = ColorFromPalette(palette, i + 100);
    //    tft.drawFastVLine(i, y, h, tft.color565(c.r, c.g, c.b));
  }
  //  tft.drawRoundRect(x - 10, y, 20, h, 5, TFT_WHITE);
}

void tftDisplayFPS() {
  tft.setCursor(180, 10);
  tft.setTextSize(1);
  tft.setTextColor(TFT_TEXT, ILI9341_BLACK);
  char p[10];
  sprintf(p, "FPS: %3d", FastLED.getFPS());
  tft.print(p);
}


// SAVING AND STORING SETTINGS
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) 
#include <EEPROM.h>

void saveSettings() {
  uint8_t address = 0;
  EEPROM.write(address++, *Data::brightness_t);
  EEPROM.write(address++, *Data::triggerType_t);
  EEPROM.write(address++, *Data::backlight_t);
}

void loadSettings() {
  uint8_t address = 0;
  *Data::brightness_t = EEPROM.read(address++);
  *Data::triggerType_t = EEPROM.read(address++); 
  *Data::backlight_t = EEPROM.read(address++);
}

#else 
void saveSettings();
void loadSettings();
#endif

void updateSettings() {
  FastLED.setBrightness(*Data::brightness_t);
  analogWrite( TFT_BACKLIGHT, gamma6[*Data::backlight_t] * 4);
}

void uiSetup() {

  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);// grey fill

  //buttons
  for (uint8_t i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(TFT_BACKLIGHT, OUTPUT);
  analogWrite( TFT_BACKLIGHT, gamma6[*Data::backlight_t] * 4 );

  // setup callbacks
  currentPattern_t.setCallback([]() {
    patterns.set(*currentPattern_t);
  });

  paletteIndex_t.setCallback([]() {
    gfx.setPalette(*paletteIndex_t);
  });
  
  gfx.currentPalette = RainbowColors_p;
  gfx.targetPalette = RainbowColors_p;

  Data::brightness_t.setCallback([]() {
    FastLED.setBrightness(*Data::brightness_t);
  });
  Data::backlight_t.setCallback([]()  {
    analogWrite( TFT_BACKLIGHT, gamma6[*Data::backlight_t] * 4);
  });

  menu.Settings.Save.setCallback([]() {
    Serial.print("Saved settings. Brightness: ");
    Serial.println(*Data::brightness_t);
    saveSettings();  
    updateSettings();
  });

  menu.Settings.Load.setCallback([]() {
    loadSettings();  
    Serial.print("Loaded settings. Brightness: ");
    Serial.print(*Data::brightness_t);
    updateSettings();
  });

  //  xTaskCreatePinnedToCore(uiTask, "UITask", 4096, NULL, 2, &uiTaskHandle, 1);

  //backlight PWM setup (TODO: use high resolution and map to give better control at lower brightness
  //  ledcSetup(OLED_LED_PIN, 5000, 8);  // pin, freq, res 8/10/12/15
  //  ledcAttachPin(OLED_LED_PIN, 0);  // attach PWM channel to OLED pin
}
