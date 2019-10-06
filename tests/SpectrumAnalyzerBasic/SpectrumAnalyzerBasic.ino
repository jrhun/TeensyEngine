#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=448,174
AudioSynthWaveformSine   sine1;          //xy=417,318
AudioSynthNoisePink      pink1;          //xy=408,369
AudioMixer4              mixer1;         //xy=592,311
AudioAnalyzeFFT1024      fft1024;      //xy=626,207
AudioConnection          patchCord1(adc1, 0, mixer1, 0);
AudioConnection          patchCord2(sine1, 0, mixer1, 1);
AudioConnection          patchCord3(pink1, 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, fft1024);
// GUItool: end automatically generated code


// The scale sets how much sound is needed in each frequency range to
// show all 8 bars.  Higher numbers are more sensitive.
float scale = 60.0;

// An array to hold the 16 frequency bands
float level[16];

// This array holds the on-screen levels.  When the signal drops quickly,
// these are used to lower the on-screen level 1 bar per update, which
// looks more pleasing to corresponds to human sound perception.
int   shown[16];



// Use the TFT library to display the spectrum
//

#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"
#define TFT_DC      32
#define TFT_CS      30
#define TFT_RST     31
#define TFT_LED     33
const uint8_t MIC_PIN = 27;
const uint8_t BACKLIGHT_PIN = 33;

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST);
//
//#include "ADC.h"
//ADC *adc = new ADC();
//
//IntervalTimer myTimer;
//
//void readAdc2() {  
//  adc->startSingleRead(A13, 1);
//}




void setup() {
  // Audio requires memory to work.
  AudioMemory(16);

  pinMode(A13, INPUT);
  
  
  
//  adc->setAveraging(16);
//  adc->setResolution(12);
//  adc->setConversionSpeed(ADC_CONVERSION_SPEED::LOW_SPEED);
//  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED);
//  myTimer.begin(readAdc, 4*1000000.0f/44100.0f);


  // turn on the LCD and define the custom characters
    // Backlight
  pinMode( BACKLIGHT_PIN, OUTPUT );
  analogWrite( BACKLIGHT_PIN, 1023 );
  
  Serial.begin(9600);
  
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
//  tft.fillRect(80, 180, 80,80, ILI9341_GREEN);
//  delay(500);

  Serial.print("Serial started");
  
  // configure the mixer to equally add left & right
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.34);
  mixer1.gain(1, 0.33);

  sine1.frequency(500);
  sine1.amplitude(0.75);
  pink1.amplitude(0.25);

  // pin 5 will select rapid vs animated display
  pinMode(5, INPUT_PULLUP); 

  // set the ISR to trigger the ADC2 at 44.1 kHz = 22microsec
//  myTimer.begin(readAdc2, 1000000.0f/44100.0f);

  adc1.setup(A13);
  
}


void loop() {
  /*
  if (fft1024.available()) {
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
    tft.fillRect(xOffset, yOffset, pixStep * 16, pixStep*9, ILI9341_BLACK);
 
    for (int i=0; i<16; i++) {
      Serial.print(level[i]);
      Serial.print("-");

      // TODO: conversion from FFT data to display bars should be
      // exponentially scaled.  But how keep it a simple example?
      int val = map(level[i] * scale,0, 100, 0, 200);
      if (val > 120) val = 120;

      if (val >= shown[i]) {
        shown[i] = val;
      } else {
        if (shown[i] > 0) shown[i] = shown[i] - 1;
        val = shown[i];
      }

      Serial.print(shown[i]);
      Serial.print(" ");

      // print each custom digit
      tft.fillRect(xOffset+i*pixStep, yOffset - (shown[i]+1), pixStep, (shown[i]+1), ILI9341_BLUE);
    }
    Serial.print(" cpu:");
    Serial.println(AudioProcessorUsageMax());
  }
  */
}
