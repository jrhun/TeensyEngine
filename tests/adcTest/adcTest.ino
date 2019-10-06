#include <DMAChannel.h>
#include "ADC.h"


ADC *adc = new ADC();
DMAChannel dma(false);
DMAMEM static uint16_t adcBuffer[128];
IntervalTimer myTimer;

void adc0_isr() {
  __disable_irq();
  Serial.print("ADC1 ISR. Value: ");
  Serial.print(adc->readSingle(0));
  Serial.print("\t(ADC1_R0):");
  Serial.println(ADC1_R0);
  __enable_irq();
}
void adc1_isr() {
  __disable_irq();
  Serial.print("ADC2 ISR. Value: ");
  Serial.print(adc->readSingle(1));
  Serial.print("\t(ADC2_R0):");
  Serial.println(ADC2_R0);
  __enable_irq();
}
void readAdc() {
  static unsigned long k = 0;
  static unsigned long initial = 0;
  k++;
  if (k > 44100) {
    k = 0;
    unsigned long now = millis();
    Serial.println("");
    Serial.print("Timer 44100 calls = ");
    Serial.print(now - initial);
    Serial.println("msec");
    initial = now;
//    Serial.print("Last value: ");
    myTimer.disable();
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);


}


void loop() {
  Serial.print("Analog read: ");
  Serial.println(analogRead(A13));
  
  delay(200);
  Serial.print("ADC1 Analog read: ");
  Serial.println(adc->analogRead(A13, 0));

  delay(200);
  Serial.print("ADC2 Analog read: ");
  Serial.println(adc->analogRead(A13, 1));

  delay(200);
  Serial.println("ADC1 interrupt test");
  adc->enableInterrupts(0);
  Serial.print("Single read: ");
  adc->startSingleRead(A13,0);
  delay(100);
  Serial.print("Default analogRead: ");
  analogRead(A13);
  delay(100);
  Serial.print("Register edit: ");
  __disable_irq();
  ADC1_HC0 = 4;
  __enable_irq();
  delay(100);
  
  // end ADC1 interrupt tests
  adc->disableInterrupts(0);
  
  delay(200);
  Serial.println("ADC1 interrupt test");
  adc->enableInterrupts(1);
  Serial.print("Single read: ");
  adc->startSingleRead(A13,1);
  delay(100);
  Serial.print("Default analogRead: ");
  analogRead(A13);
  delay(100);
  Serial.print("Register edit: ");
  __disable_irq();
  ADC2_HC0 = 4;
  __enable_irq();
  delay(100);

  // end ADC2 interrupt tests
  adc->disableInterrupts(1);

  delay(200);
  Serial.println("Set pinmode");
  pinMode(A13, INPUT);
  Serial.print("Analog read: ");
  Serial.println(analogRead(A13));
  
  delay(200);
  Serial.print("ADC1 Analog read: ");
  Serial.println(adc->analogRead(A13, 0));

  delay(200);
  Serial.print("ADC2 Analog read: ");
  Serial.println(adc->analogRead(A13, 1));

  delay(200);
  Serial.println("ADC1 interrupt test");
  adc->enableInterrupts(0);
  Serial.print("Single read: ");
  adc->startSingleRead(A13,0);
  delay(100);
  Serial.print("Default analogRead: ");
  analogRead(A13);
  delay(100);
  Serial.print("Register edit: ");
  __disable_irq();
  ADC1_HC0 = 4;
  __enable_irq();
  delay(100);
  
  // end ADC1 interrupt tests
  adc->disableInterrupts(0);
  
  delay(200);
  Serial.println("ADC1 interrupt test");
  adc->enableInterrupts(1);
  Serial.print("Single read: ");
  adc->startSingleRead(A13,1);
  delay(100);
  Serial.print("Default analogRead: ");
  analogRead(A13);
  delay(100);
  Serial.print("Register edit: ");
  __disable_irq();
  ADC2_HC0 = 4;
  __enable_irq();
  delay(100);

  // end ADC2 interrupt tests
  adc->disableInterrupts(1);
  
  delay(200);
  Serial.println("\nTimer test");
  myTimer.begin(readAdc, 4*1000000.0f/44100.0f);//4*22.675737f);

  delay(5000);
  Serial.println("DMA ADC1 test");
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

  Serial.print("DMA setup\n default read test");
  for (int i = 0; i < 1024; i++) {
    
  }
  
}

