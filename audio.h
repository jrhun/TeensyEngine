#pragma once 

#include "data.h"

//#include "ADC.h"
//ADC *adc = new ADC();
//audioIn (pin27, A13) is on ADC2

// references
//noise processing code https://github.com/atuline/FastLED-Demos/blob/master/soundmems_noise/soundmems_noise.ino
//example algorithim https://www.parallelcube.com/2018/03/30/beat-detection-algorithm/
//article on the principles of beat detection https://www.badlogicgames.com/wordpress/?p=99
// ended up just doing a VU like meter rather than beat detection 

IntervalTimer myTimer;

#define SAMPLEPERIODUS 200

int spectrumValue[7];

void getSpectrum () { // enables the MSQEQ7 and gets the data
  const uint16_t filterValue = 80;

  // Set reset pin low to enable strobe
  digitalWrite(EQ_RESET_PIN, HIGH);
  digitalWrite(EQ_RESET_PIN, LOW);


  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(EQ_STROBE_PIN, LOW);

    delayMicroseconds(30); // Allow output to settle

    uint16_t val = analogRead(EQ_AUDIO_PIN);

    // Constrain any value above 1023 or below filterValue
    val = constrain(spectrumValue[i], filterValue, 4096);

    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(val, filterValue, 4096, 0, 255);

    digitalWrite(EQ_STROBE_PIN, HIGH);

  }
}


float bassFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1];
  xv[1] = xv[2];
  xv[2] = (sample) /
          3.f; // change here to values close to 2, to adapt for stronger or weeker sources of line level audio

  yv[0] = yv[1];
  yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
  return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float envelopeFilter(float sample) { //10hz low pass
  static float xv[2] = {0, 0}, yv[2] = {0, 0};
  xv[0] = xv[1];
  xv[1] = sample / 50.f;
  yv[0] = yv[1];
  yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
  return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float beatFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1];
  xv[1] = xv[2];
  xv[2] = sample / 2.7f;
  yv[0] = yv[1];
  yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
  return yv[2];
}

void adcRead() {
//    adc->adc1->startSingleRead(A13);
//  adc->startSingleRead(A13, 1);

  static uint8_t i = 0;
  static float average = 0;
  const float smoothing = 0.02;
  float sample = (float) analogRead(MIC_IN_PIN) - 504*3;
  //  int16_t d = (int16_t)adc->readSingle(1) - 504*3;
  float value, envelope, beat, thresh;
  
  value = bassFilter(sample);
  if (value < 0)
    value = -value;
  average = average * (1-smoothing) + value * smoothing;
  envelope = envelopeFilter(value);
  
  if (i == 200) { //25hz
    i = 0;
//    Serial.print("average: ");
//    Serial.println(average);
    if (average > 100) {
      analogWrite(TFT_BACKLIGHT, map(average, 0, 1000, 0, 1024));
    } else {
      analogWrite(TFT_BACKLIGHT, 0);
    }
    
    beat = beatFilter(envelope);
    if (beat > Data::audioThreshold) {
      // beat detected
      Serial.print("beat! val: ");
      Serial.println(beat);
      Data::beat = 255;
      
    } else {
      if (Data::beat > 0)
        Data::beat--;
    }
  } else {
    i++;
  }
}

void audioSetup() {
  pinMode(MIC_IN_PIN, INPUT);
  adc->adc1->setReference(ADC_REFERENCE::REF_3V3);
  adc->adc1->setAveraging(8); // set number of averages
  adc->adc1->setResolution(16); // set bits of resolution
  adc->adc1->recalibrate();
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::MED_SPEED ); // change the conversion speed
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED  ); // change the sampling speed
    

//    adc->adc1->setAveraging(8);
//    adc->adc1->setResolution(10);
//    adc->adc1->recalibrate();
//    adc->adc1->setAveraging(8);
//    adc->adc1->setResolution(10);
  
//    adc->adc1->enableInterrupts();

//  adc->setAveraging(8, 1);
//  adc->setResolution(10, 1);
//  adc->analogRead(A13, 1);
//  adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED, 1);
//  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED, 1);
//  adc->enableInterrupts(1);

//    adc->adc1->stopPDB();
//    adc->adc1->startSingleRead(MIC_IN_PIN);
//    adc->adc1->startPDB(5000);


//  myTimer.begin(adcRead, 1000000.0f / 5000.0f);
}
void adc0_isr() {}

void adc1_isr() {
//  static uint8_t i = 0;
//  float sample = (float) adc->adc1->readSingle();
//  //  int16_t d = (int16_t)adc->readSingle(1) - 504*3;
//  float value, envelope, beat, thresh;
//  value = bassFilter(sample);
//  if (value < 0)
//    value = -value;
//  envelope = envelopeFilter(value);
//  
//  if (i == 200) { //25hz
//    i = 0;
//    Serial.print("sample: ");
//    Serial.println(sample);
//    
//    beat = beatFilter(envelope);
//    if (beat > Data::threshold) {
//      // beat detected
//      Serial.print("beat! val: ");
//      Serial.println(envelope);
//    }
//  } else {
//    i++;
//  }
}

  //settings for a 160 led rhombus - pulsating and spinning settings

  int stripNumOfLeds = 64;                                 // the total number of leds
  int stripsOn2Pins = false;                                // set to true if the LED strips or rings are connected to 2 input pins
//  uint32_t stripColor[81];                                  // half of the number of leds + 1
  int displayMiddleLed = true;                              // display the middle led (blue). set to true for one strip, false for two strips or rings
  int splitStrip = true;                                    // set to true when using 2 strips or rings, false for one strip
  int middleOffset = 0;                                     // offset for the middle led when using one strip
  int startupAnimationDelay = 0;                            // delay for the startup animation
  int orangeLimitAmount = 0;                                // limit the amount of green of the middle LEDs to make them more orange
  int swapLeftRight = false;                                // swap the left and right input values or not
  
  int dropDelay = 4;                                        // hold time before dropping the leds
  float dropFactor = .92;                                   // value for dropping the leds
  
  int peakTimeNoDropDelay = 150;                            // peak hold time when not dropping the peaks (when droppingPeak is false)
  int peakTimeFirstDropDelay = 70;                          // peak hold time when dropping the first peak
  int peakTimeDropDelay = 7;                                // peak hold time when dropping the rest of the peaks
  float peakDropFactor = .94;                               // value for dropping the peaks
  int droppingPeakFade = false;                             // display the dropping peak fading to black or not
  
  int bouncingPeaksNumOfLeds = 10;                          // how many leds to bounce up (max)
  int bouncingPeaksNumOfLedsMin = 5;                        // how many leds to bounce up (min) when using dynamicBouncingPeaks
  int bouncingPeakDelay = 4;                                // delay between peak bounce updates
  int bouncingPeakCounterInc = 6;       

  // basic settings
int pulsing = true;                                       // pulsing will display from the middle of each strip or ring  @EB

int spinCircle = true;                                    // spin the animation. will not work with stripsOn2Pins  @EB

int animType = 0;                                         // startup animation selection (1 looks nice for 1 ring)  @EB
int colorScheme = 10;                                     // 0: green-red, 1: blue-green, 2: blue-red, 3: red-blue, 4: green-blue, 5: red-green, 6: blue-white-red
                                                          // 7: red-white-blue, 8: green-white-red, 9: green-white-blue, 10: color wheel, 11: spinning color wheel,
                                            // 12: as 11 but spread with factor colorScheme12Factor  @EB
int maxColorScheme = 12;                                  // used for looping through the color schemes with the switch button
int colorScheme11SpinDelay = stripNumOfLeds / 4 ;         // delay for spinning scheme 11
int colorScheme12Factor = 3;                              // wheel spread factor for scheme 12 @EB

int minValue = 10;                                        // min analog input value
int sensitivityValue = 110;                               // 0 - 255, initial value (value read from the potentiometer if useSensorValues = true)

void audioLoop() {



  
  unsigned long now = micros(); // Used to track rate
  static unsigned long lastUpdate = 0;
  float sample, value, envelope, beat, thresh;
  static unsigned char i;

  

  // Read ADC and center
  int16_t raw = adc->adc1->analogRead(MIC_IN_PIN) - 1512;

  sample = (float) raw;
  
  raw = abs(raw);
//  Serial.print(raw);
  raw = (raw <= Data::noiseFloor) ? 0 : (raw - Data::noiseFloor);
  raw = ((Data::sampleArray[Data::sampleCount] * 7) + raw) >> 3;
//  Serial.print(" ");
//  Serial.println(raw);
  Data::sampleSum += raw - Data::sampleArray[Data::sampleCount];
  Data::sampleAvg = Data::sampleSum / Data::numSamples;
  Data::sampleArray[Data::sampleCount] = raw;
  Data::sampleCount = (Data::sampleCount +1) % Data::numSamples;

  if ((raw > (Data::sampleAvg + Data::maxVol)) and
              (millis() > (Data::peakTime + 50)) and
              (Data::samplePeak == 0)) {
        Data::samplePeak = 1; 
        Data::peakTime = millis();
  } else {
//    Data::samplePeak = 0;  //reset by patterns
  }


  // filterd sound
  

  // Filter only bass component
  value = bassFilter(sample);

  // Take signal amplitude and filter
  if (value < 0)
    value = -value;
  envelope = envelopeFilter(value);

  // Every 200 samples (25hz) filter the envelope = 200 samples every 40msec = 1 sample every 200micros
  if (i == 200) {
    // Filter out repeating bass sounds 100 - 180bpm
    beat = beatFilter(envelope);

    //                  Serial.println(beat);
    // Threshold it based on potentiometer on AN1
    //TODO(aphexcx) make software-settable
    thresh = 200; // 0.02f * (float)analogRead(1);

    // If we are above threshold, light up LED
    if (beat > thresh)  {
      Serial.println(beat);
      Data::beat = 255;
    } else {
      Data::beat--;
    }

    //Reset sample counter
    i = 0;
  }
  
}
