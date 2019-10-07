#include "fft.h"
#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"
#define TFT_DC      32
#define TFT_CS      30
#define TFT_RST     31
#define TFT_LED     33

const uint8_t BACKLIGHT_PIN = 33;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);

FFT fft1024;
float level[16];
float scale = 400.0;
int   shown[16];

void setup() {
  Serial.begin(9600);
  pinMode(A13, INPUT);
  pinMode( BACKLIGHT_PIN, OUTPUT );
  analogWrite( BACKLIGHT_PIN, 1023 );

  tft.begin();
  tft.setRotation( 3 );
  tft.fillScreen(ILI9341_BLACK);

  tft.print("waiting for");
  tft.setFont(Arial_24);
  tft.setCursor(100, 80);
  tft.print("Arduino");
  tft.setCursor(60, 120);
  tft.print("Serial Monitor");
  tft.setTextColor(ILI9341_GREEN);
  tft.setFont(Arial_18);
  while (!Serial && millis() < 8000) { // wait for Arduino Serial Monitor
    tft.fillRect(118, 182, 42, 18, ILI9341_BLACK);
    tft.setCursor(118, 182);
    tft.print((8000.0 - (float)millis()) / 1000.0, 1);
    tft.print(" sec");
    delay(50);
  }
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setFont(Arial_18);
  tft.setCursor(40, 42);
  tft.print("Spectrum analyzer");

  fft1024.init();
}

void loop() {
  // put your main code here, to run repeatedly:

  
  fft1024.test();
//  delay(1000/60);
//  fft1024.update();
  if (fft1024.available() and (1)) {
    // read the 512 FFT frequencies into 16 levels
    // music is heard in octaves, but the FFT data
    // is linear, so for the higher octaves, read
    // many FFT bins together.
    level[0] =  fft1024.read(0);
    level[1] =  fft1024.read(1);
    level[2] =  fft1024.read(2, 3);
    level[3] =  fft1024.read(4, 6);
    level[4] =  fft1024.read(7, 10);
    level[5] =  fft1024.read(11, 15);
    level[6] =  fft1024.read(16, 22);
    level[7] =  fft1024.read(23, 32);
    level[8] =  fft1024.read(33, 46);
    level[9] =  fft1024.read(47, 66);
    level[10] = fft1024.read(67, 93);
    level[11] = fft1024.read(94, 131);
    level[12] = fft1024.read(132, 184);
    level[13] = fft1024.read(185, 257);
    level[14] = fft1024.read(258, 359);
    level[15] = fft1024.read(360, 511);
    // See this conversation to change this to more or less than 16 log-scaled bands?
    // https://forum.pjrc.com/threads/32677-Is-there-a-logarithmic-function-for-FFT-bin-selection-for-any-given-of-bands

    // if you have the volume pot soldered to your audio shield
    // uncomment this line to make it adjust the full scale signal
    //scale = 8.0 + analogRead(A1) / 5.0;

    //clear draw area first

    const int xOffset = 40;
    const int yOffset = 200;
    const int pixStep = 15;
    tft.fillScreen(ILI9341_BLACK);
//    tft.fillRect(xOffset- pixStep * 16, yOffset - pixStep*9, pixStep * 16, pixStep*9, ILI9341_BLACK);
 
    for (int i=1; i<16; i++) {
//      Serial.print(level[i]);
//      Serial.print("-");

      // TODO: conversion from FFT data to display bars should be
      // exponentially scaled.  But how keep it a simple example?
      int val = map(level[i] * scale,0, 100, 0, 200);
      if (val > 120) val = 120;

//      if (val >= shown[i]) {
//        shown[i] = val;
//      } else {
//        if (shown[i] > 0) shown[i] = shown[i] - 1;
//        val = shown[i];
//      }

//      Serial.print(val);
//      Serial.print(" ");

      // print each custom digit
      tft.fillRect(xOffset+i*pixStep, yOffset - (val+1), pixStep, (val+1), ILI9341_BLUE);
    }
//    Serial.println();
//    Serial.println(AudioProcessorUsageMax());
  }
  
}
