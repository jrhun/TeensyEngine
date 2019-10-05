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
  static uint16_t k = 0;
  static unsigned long initial = 0;
  k++;
  if (k % 44100 == 0) {
    unsigned long now = millis();
    Serial.println("ADC0_ISR");
    Serial.print("Time for 44100 samples = ");
    Serial.print(now - initial);
    Serial.println("msec");
    initial = now;
  }
  audioBuffer[activeBuffer].data[bufferIndex] = adc->readSingle();
  bufferIndex++;
  if (bufferIndex >= 128) {
    bufferIndex = 0;
    activeBuffer = (activeBuffer + 1) % numBuffers;
  }
}


#include <DMAChannel.h>

//DMAMUX_SOURCE_ADC1 is adc0
DMAChannel dma(false);
DMAMEM static uint16_t adcBuffer[128];

void adc1_isr() {}

ISR(TIMER0_OVF_vect)
{
    /* Timer 0 overflow */
    
}


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
  }
  dma.clearInterrupt();
}

IntervalTimer myTimer;



void readAdc() {
  adc->startSingleRead(A13);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(readPin, INPUT);

  Serial.begin(9600);
//  delay(10000);
  while(!Serial);

  Serial.print("ADC Test");

  adc->setAveraging(16);
  adc->setResolution(12);
  adc->analogRead(A13);
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED);
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_LOW_SPEED);
  
//  adc->enableInterrupts();
  adc->enableDMA();

//  ADC1_CFG |= ADC_CFG_ADTRG;
//  ADC1_GC |= ADC_GC_DMAEN;


  dma.begin(true);
  dma.TCD->SADDR = &ADC1_R0;
  dma.TCD->SOFF = 0;
  dma.TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  dma.TCD->NBYTES_MLNO = 2;
  dma.TCD->SLAST = 0;
  dma.TCD->DADDR = adcBuffer;
  dma.TCD->DOFF = 2;
  dma.TCD->CITER_ELINKNO = sizeof(adcBuffer) / 2;
  dma.TCD->DLASTSGA = -sizeof(adcBuffer);
  dma.TCD->BITER_ELINKNO = sizeof(adcBuffer) / 2;
  dma.TCD->CSR = DMA_TCD_CSR_INTHALF | DMA_TCD_CSR_INTMAJOR;
  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC1);
  dma.enable();
  dma.attachInterrupt(DMA_ISR);

//  adc->startContinuous(A13);

  myTimer.begin(readAdc, 22.675737f);


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
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
