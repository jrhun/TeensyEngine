#include <avr/io.h>
#include <avr/interrupt.h>


#include <ADC.h>
ADC *adc = new ADC();

const int readPin = A13; // ADC1

// set up continuous reads
// set up ISR every 25microsec (40khz) to copy value to buffer
// ISR triggered every 25microsec (40khz)
// does a single read

struct AudioBuffer {
  volatile uint16_t data[128];
//  volatile uint8_t index = 0;
//  volatile uint8_t activeBuffer = 0;
//  volatile uint16_t data0[128];
//  volatile uint16_t data1[128];
//  volatile uint16_t data2[128];
//  volatile uint16_t data3[128];
//  volatile uint16_t data4[128];
//  volatile uint16_t data5[128];
//  volatile uint16_t data6[128];
//  volatile uint16_t data7[128];
};
volatile uint8_t bufferIndex = 0;
volatile uint8_t activeBuffer = 0;
const uint8_t  numBuffers = 8;
volatile AudioBuffer audioBuffer[numBuffers];


void adc0_isr() {
  Serial.print(adc->readSingle(0));
//  audioBuffer[activeBuffer].data[bufferIndex] = adc->readSingle();
//  bufferIndex++;
//  if (bufferIndex >= 128) {
//    bufferIndex = 0;
//    activeBuffer = (activeBuffer + 1) % numBuffers;
//  }
//  static unsigned long k = 0;
//  k++;
//  if (k%441000 == 0) {
//  Serial.print("Value1adc: ");
//  Serial.println(adc->readSingle(1));
//  Serial.print("Value1adc: ");
//  Serial.println(analogRead(A13));
//  }
    static uint16_t k = 0;
  static unsigned long initial = 0;
  k++;
//  if (k % 44100 == 0) {
    __disable_irq();
    unsigned long now = millis();
    Serial.println("ADC2_ISR");
    Serial.print("Time for 44100 samples = ");
    Serial.print(now - initial);
    Serial.println("msec");
    Serial.print("Reading: ");
    Serial.println(ADC1_R0);
    initial = now;
    __enable_irq();
//  }
}

void adc1_isr() {
//  Serial.print("Value1adc: ");
//  Serial.println(adc->readSingle(1));
//  Serial.print("Value1adc: ");
//  Serial.println(analogRead(A13));
//    static uint16_t k = 0;
//  static unsigned long initial = 0;
//  k++;
//  if (k % 44100 == 0) {
//    unsigned long now = millis();
//    Serial.println("ADC2_ISR");
//    Serial.print("Time for 44100 samples = ");
//    Serial.print(now - initial);
//    Serial.println("msec");
//    Serial.print("Reading: ");
//    Serial.println(ADC2_R0);
//    initial = now;
//  }
}


#include <DMAChannel.h>

//DMAMUX_SOURCE_ADC1 is adc0
DMAChannel dma(false);
DMAMEM static uint16_t adcBuffer[128];


void DMA_ISR() {
  static unsigned long k = 0;
  static unsigned long initial = 0;
  k++;
  
  if (k % (44100/128) == 0) {
     unsigned long now = millis();
     Serial.println("DMA Completed 44100 reads total");
     Serial.print("Time for 44100 samples = ");
     Serial.print(now - initial);
     Serial.println("msec");
     initial = now;
     Serial.print("Reading: ");
     Serial.println(ADC1_R0);
  }
  dma.clearInterrupt();
}

IntervalTimer myTimer;



void readAdc() {
//  adc->startSingleRead(A13,0);
  __disable_irq();
  ADC1_HC0 = 4;
  __enable_irq();
//    ADC1_HC0 = 4; //A13
//  Serial.print("Value1adc: ");
//  Serial.println(adc->analogRead(A13, 1));
//  Serial.print("Value2adc: ");
//  Serial.println(analogRead(A13));
}



void setup() {
  // put your setup code here, to run once:
  pinMode(A13, INPUT);

  Serial.begin(9600);
//  delay(10000);
  while(!Serial);

  Serial.print("ADC Test");

//  adc->setAveraging(16,0);
//  adc->setResolution(12,0);
//  adc->analogRead(A13,0);
//  adc->setConversionSpeed(ADC_CONVERSION_SPEED::LOW_SPEED,0);
//  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::LOW_SPEED,0);
  
  adc->enableInterrupts(0);
//  adc->enableDMA();

//  ADC1_CFG |= ADC_CFG_ADTRG;

  //use ADC1
//  ADC1_GC |= ADC_GC_DMAEN; //enable DMA trigger on conversion 

//  dma.begin(true);
//  dma.TCD->SADDR = &ADC1_R0;
//  dma.TCD->SOFF = 0;
//  dma.TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
//  dma.TCD->NBYTES_MLNO = 2;
//  dma.TCD->SLAST = 0;
//  dma.TCD->DADDR = adcBuffer;
//  dma.TCD->DOFF = 2;
//  dma.TCD->CITER_ELINKNO = sizeof(adcBuffer) / 2;
//  dma.TCD->DLASTSGA = -sizeof(adcBuffer);
//  dma.TCD->BITER_ELINKNO = sizeof(adcBuffer) / 2;
//  dma.TCD->CSR = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;
//  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);
//  dma.enable();
//  dma.attachInterrupt(DMA_ISR);

//  adc->startContinuous(A13,1);

//  myTimer.begin(readAdc, 4*1000000.0f/44100.0f);//4*22.675737f);
//  startPDB();


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
//  analogReadRes(10); // max is 1024
//  analogReadAveraging(16);
//  Serial.println(analogRead(A13));
//    Serial.println((int16_t)adc->analogRead(A13,1));
    adc->startSingleRead(A13,1);
//  int8_t lastBuffer = (activeBuffer + numBuffers - 1) % numBuffers;
//  Serial.print("Buffer num: \n");
//  Serial.print(lastBuffer);
//  Serial.println();
//  for (int i = 0; i < 128; i++) {
//    
//    Serial.print(audioBuffer[lastBuffer].data[i]);
//    Serial.print("\t");
//    if (i % 8 == 0)
//      Serial.print("\n");
//  }
  Serial.print(".");
}
