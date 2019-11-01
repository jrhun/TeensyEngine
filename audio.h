
#include "data.h"

#include "ADC.h"
//ADC *adc = new ADC();

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
    if (beat > Data::threshold) {
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


  myTimer.begin(adcRead, 1000000.0f / 5000.0f);
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

void audioLoop() {
  unsigned long now = micros(); // Used to track rate
  static unsigned long lastUpdate = 0;
  float sample, value, envelope, beat, thresh;
  static unsigned char i;

  // Read ADC and center so +-512
  sample = (float) analogRead(MIC_IN_PIN) - 1512.f;

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

  // Consume excess clock cycles, to keep at 5000 hz
  //    for (unsigned long up = time + SAMPLEPERIODUS; time > 20 && time < up; time = micros());

}
