#include <ADC.h>
ADC *adc = new ADC();

IntervalTimer myTimer;

volatile uint8_t bufferIndex = 0;
volatile int16_t dataBuffer[128];

void readAdc() {
  adc->startSingleRead(A13,1);
}

void adc0_isr() {}

void adc1_isr() {
  static unsigned long counter = 0;

  int16_t d = (int16_t)adc->readSingle(1);
  dataBuffer[bufferIndex] = d;
  counter++;
  if (counter % 44100 == 0){
    unsigned long sum = 0;
    Serial.print("Raw:\n");
    for (int i = 0; i < 128; i++) {
      if (i%16==0) Serial.println();
      Serial.print(dataBuffer[i]);
      Serial.print(" ");
      sum += dataBuffer[i];
    }
    sum /= 128;
    Serial.print("\nAverage: ");
    Serial.print(sum);
    Serial.print("\tLast: ");
    Serial.println(d);
  }
  bufferIndex++;
  bufferIndex %= 128;
  
}


void setup() {
  // put your setup code here, to run once:
  adc->setAveraging(2,1);
  adc->setResolution(12,1);
  adc->analogRead(A13,1);
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED,1);
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED,1);
  adc->enableInterrupts(1);
  
  Serial.begin(9600);
  while (!Serial);
  Serial.println("ADC Test");
  
  myTimer.begin(readAdc, 1000000.0f/44100.0f);//4*22.675737f);
}

void loop() {
  // put your main code here, to run repeatedly:

}
