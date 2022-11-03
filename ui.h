#include <SPI.h>
#include <Encoder.h> // paul strofen library

#include "headbands.h"

#include "data.h"
//#include "ledControl.h"
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
//#include "ILI9341_t3.h"
//#include "font_Arial.h"

//#include "SPI.h"
#include "ILI9341_t3n.h"
#include "ili9341_t3n_font_Arial.h"
#include "ili9341_t3n_font_ArialBold.h"
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

#define TFT_MOSI    11
#define TFT_SCLK    13
#define TFT_MISO    12


#define TFT_BACKGROUND  0x2967
#define TFT_TEXT        0xE719
//ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);





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
// Display functions
void drawBattery() {

}

//#include "src\palettes.h"

//void drawPalette(uint8_t x, uint8_t y, CRGBPalette16 &palette) {
void drawPalette(uint8_t paletteIndex) {
  //draws a rectangle filled with current palette with arrow highlighting current hue
  // TL = top left
  // BR = bottom right
  // 0,0 is top left
  const uint8_t h = 16;
  const uint8_t w = 255; // 240 - w / 2
//  const uint8_t xStart = (240 - w) / 2;

//  uint8_t currentHue = (Data::getHue() + 128) % 255; // current hue centered in middle
  uint8_t currentHue = 0;
  CRGBPalette16 p;
  if (paletteIndex == 0)
    p = RainbowColors_p;
  else 
    p = gGradientPalettes[paletteIndex-1];
  for (uint8_t i = 0; i < w; i++) {
    //center current hue in middle
    
    CRGB c = ColorFromPalette(p, currentHue);
    currentHue++;
    tft.drawFastHLine(224, 60+i, 16, tft.color565(c.r, c.g, c.b));
  }
  //  tft.drawRoundRect(x - 10, y, 20, h, 5, TFT_WHITE);
}

void tftDisplayFPS() {
  tft.setCursor(180, 310);
  tft.setTextSize(1);
  tft.setTextColor(TFT_TEXT, ILI9341_BLACK);
  char p[10];
  sprintf(p, "FPS:%3d", FastLED.getFPS());
  tft.print(p);
}

void tftDisplayVoltsAmps() {
  static uint8_t lastValue = 0;
  const uint8_t numVals = 10;
  static float volts[numVals] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  static float voltsSum = 0.0f;
  static float amps[numVals] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  static float ampsSum = 0.0f;



  float v = analogRead(VOLTS_IN_PIN);
//  Serial.print("volt raw: ");
//  Serial.print(v);
  const float VREF = 3.292f;
  const uint16_t VMAX = 4095;
  v = (v * VREF) / VMAX;
//  Serial.print("\tcalc: ");
//  Serial.print(v);
  // I think R1 is 508k, R2 96k...
  v = map(v, 0.22f, 0.85f, 4.32f, 16.47f); // but lets just use measured values for rough estimate
//  Serial.print("\tmap: ");
//  Serial.println(v);
  
  int16_t ampsRaw = analogRead(AMP_IN_PIN); //  ampsRaw -= 3107; // 0 amp center is 3107 measured (3110 calculated 2.5V)
  //  amps = (ampsRaw * VREF) / VMAX; // amps measured in volts where 100mv = 1a
//  Serial.print("amp raw: ");
//  Serial.print(ampsRaw);
  float a = map(ampsRaw, 3169, 3347, 580, 2050);
//  Serial.print("amp map: ");
//  Serial.println(a);
  a /= 1000.0f;


  voltsSum -= volts[lastValue];
  voltsSum += v;
  volts[lastValue] = v;

  ampsSum -= amps[lastValue];
  ampsSum += a;
  amps[lastValue] = a;

  lastValue = (lastValue + 1) % numVals;

  float voltsAvg = voltsSum / numVals;
  float ampsAvg = ampsSum / numVals;

  tft.setTextSize(1);
  tft.setCursor(180, 8);
  tft.setTextColor(TFT_TEXT, ILI9341_BLACK);
  char t[12];
  sprintf(t, "V: %.2f", voltsAvg);
  tft.print(t);

  

  tft.setCursor(180, 16);
  sprintf(t, "A: %.2fA", ampsAvg);
  tft.print(t);
}

void uiLoop() {
  unsigned long now = millis();

  //encoder handle
  int8_t newPos = encoder.read();
  cli();
  if (newPos > 0) {

    while (newPos >= 4) {
      inc();
      newPos -= 4;
    }
    //    newPos = 0;
    encoder.write(newPos);
  } else if (newPos < 0) {
    while (newPos <= -4) {
      dec();
      newPos += 4;
    }
    //    newPos = 0;
    encoder.write(newPos);
  }
  sei();

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
          //          Serial.print("Press: ");
          //          Serial.println(i);
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
  if (now - lastTFTUpdate > (1000 / 10)) {
    lastTFTUpdate = now;
    // fast text updates only!
    tft.useFrameBuffer(false);
    tftDisplayVoltsAmps();
    //FPS

    if (displayFPS) {
      tftDisplayFPS();

    }

#if 1
    if (updateDisplay) {
      updateDisplay = false;

      //    tft.fillScreen(ILI9341_BLACK);
      tft.useFrameBuffer(true);
      // print title
      const uint8_t headingOffset = 2;
      tft.setCursor(0, headingOffset);
      tft.setTextSize(3);
      tft.setTextColor(TFT_TEXT);//TFT_BACKGROUND);
      tft.println("Radience");
      //const uint8_t headingBottom = tft.fontCapHeight() + headingOffset;
      const uint8_t headingBottom = tft.getTextSizeY() + headingOffset;
      tft.drawFastHLine(0, headingBottom + 2, tft.width(), TFT_TEXT);

      //print page name
      tft.setCursor(2, headingBottom + 6);
      tft.setTextSize(2);
      //tft.fillRect(0, headingBottom + 4, tft.width(), tft.fontCapHeight() + 2, ILI9341_BLACK);
      tft.fillRect(0, headingBottom + 4, tft.width(), tft.getTextSizeY() + 2, ILI9341_BLACK);      
      //    tft.print("Current pattern\n> ");
      tft.println(menu.currentPage()->getName());
      for (uint8_t i = 0; i < menu.numPages; i++) {
        const uint8_t xPos = tft.width() - 52;
        const uint8_t yPos = headingBottom + 2 + 2 + 2;
        if (i == menu.currentPageIndex) {
          //tft.fillRoundRect(xPos + i * 10, yPos, 8, tft.fontCapHeight(), 2, TFT_TEXT);
          tft.fillRoundRect(xPos + i * 10, yPos, 8, tft.getTextSizeY(), 2, TFT_TEXT);
        }
        else {
          //tft.drawRoundRect(xPos + i * 10, yPos, 8, tft.fontCapHeight(), 2, TFT_TEXT - 0x0841);
          tft.drawRoundRect(xPos + i * 10, yPos, 8, tft.getTextSizeY(), 2, TFT_TEXT - 0x0841);
        }
      }
      //tft.drawFastHLine(0, headingBottom + tft.fontCapHeight() + 9, tft.width(), TFT_TEXT);
      tft.drawFastHLine(0, headingBottom + tft.getTextSizeY() + 9, tft.width(), TFT_TEXT);


      //Print body
      //const uint8_t bodyY = headingBottom + tft.fontCapHeight() + 12;
      const uint8_t bodyY = headingBottom + tft.getTextSizeY() + 12;
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

      
//      if (menu.currentPageIndex == 1) {
        //colour, display palette
//        drawPalette(0, 300, gfx.currentPalette);
//      }

      tftDisplayVoltsAmps();
      if (displayFPS) tftDisplayFPS();
      tft.updateScreenAsync(false);
    }
#endif
  }
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

  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, LOW);

  tft.begin();
  //  tft.setFont(Arial_24_Bold);
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);// grey fill

  // turn up screen brightness
  uint16_t i = 0;
  while (i <= *Data::backlight_t) {
    analogWrite( TFT_BACKLIGHT, gamma6[i] * 4 );
    i += 4;
    delay(5);
  }
  analogWrite( TFT_BACKLIGHT, gamma6[*Data::backlight_t] * 4 );

  //buttons
  for (uint8_t i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  encoder.write(0);




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

  menu.Colour.PaletteList.setCallback([](uint8_t i) {
    if (i <= paletteIndex_t.max)
      drawPalette(i);
  });

  menu.Settings.Dither.setCallback([]() {
    FastLED.setDither(Data::dither);
  });
  
  menu.Settings.Save.setCallback([]() {
//    Serial.print("Saved settings. Brightness: ");
//    Serial.println(*Data::brightness_t);
    saveSettings();
    updateSettings();
  });

  menu.Settings.Load.setCallback([]() {
    loadSettings();
//    Serial.print("Loaded settings. Brightness: ");
//    Serial.print(*Data::brightness_t);
    updateSettings();
  });

  setupRadio();
  menu.Headbands.Palette.setCallback([]() {
    sendClientUpdate(PALETTE);
  });

  menu.Headbands.Pattern.setCallback([]() {
    sendClientUpdate(SETTINGS);
  });

  //  Data::brightnessHb_t.setCallback([]() {
  //    sendClientUpdate(SETTINGS);
  //  });
  //
  //  Data::fadeHb_t.setCallback([]() {
  //    sendClientUpdate(SETTINGS);
  //  });
  //
  //  Data::speedHb_t.setCallback([]() {
  //    sendClientUpdate(SETTINGS);
  //  });
  //
  //  Data::hueDelayHb_t.setCallback([]() {
  //    sendClientUpdate(SETTINGS);
  //  });

  menu.Headbands.Sync.setCallback([]() {
    sendClientUpdate(UPDATE);
  });

  menu.Headbands.Settings.setCallback([]() {
    sendClientUpdate(SETTINGS);
  });

  //  xTaskCreatePinnedToCore(uiTask, "UITask", 4096, NULL, 2, &uiTaskHandle, 1);

  //backlight PWM setup (TODO: use high resolution and map to give better control at lower brightness
  //  ledcSetup(OLED_LED_PIN, 5000, 8);  // pin, freq, res 8/10/12/15
  //  ledcAttachPin(OLED_LED_PIN, 0);  // attach PWM channel to OLED pin
}
