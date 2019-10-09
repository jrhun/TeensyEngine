#include <DMAChannel.h>
#include "ADC.h"


typedef struct audio_block_struct {
  uint8_t  ref_count;
  uint8_t  reserved1;
  uint16_t memory_pool_index;
  int16_t  data[AUDIO_BLOCK_SAMPLES];
} audio_block_t;

ADC *adc = new ADC();
DMAChannel dma(false);
DMAMEM static uint16_t adcBuffer[128];
volatile uint16_t outputBlock1[128];
volatile uint16_t outputBlock2[128];
uint16_t* block = NULL;
audio_block_t * block_left = NULL;


IntervalTimer myTimer;

void adc0_isr() {
  __disable_irq();
  Serial.print("ADC1 ISR. Value: ");
  Serial.print(adc->adc0->readSingle());
  Serial.print("\t(ADC1_R0):");
  Serial.println(ADC1_R0);
  __enable_irq();
}
void adc1_isr() {
  //    __disable_irq();
  //    Serial.print("ADC2 ISR. Value: ");
  //    Serial.print(adc->adc1->readSingle());
  //    Serial.print("\t(ADC2_R0):");
  //    Serial.println(ADC2_R0);
  //    __enable_irq();
}
void adcRead() {
  adc->adc1->startSingleRead(A13);
}

void DMA_ISR() {
  uint32_t daddr, offset;
  const uint16_t *src, *end;
  uint16_t *dest;

  daddr = (uint32_t)(dma.TCD->DADDR);

  dma.clearInterrupt();
  //  static unsigned long k = 0;
  //  static unsigned long initial = 0;
  //  k++;
  //  if (k % 44100 == 0 ) {

  if (daddr < (uint32_t)adcBuffer + sizeof(adcBuffer) / 2) {
    // DMA is receiving to the first half of the buffer
    // need to remove data from the second half
    src = (uint16_t *)&adcBuffer[128 / 2];
    end = (uint16_t *)&adcBuffer[128];
    offset = 128 / 2;
  } else {
    // DMA is receiving to the second half of the buffer
    // need to remove data from the first half
    src = (uint16_t *)&adcBuffer[0];
    end = (uint16_t *)&adcBuffer[128 / 2];
    offset = 0;
    //if (update_responsibility) AudioStream::update_all();
  }
  left = block;
  if (left != NULL) {
    
    dest = (uint16_t *) & (outputBlock[offset]);
    do {
      *dest++ = *src++;
    } while (src < end);
  }




}


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Test started!\n");

  adc->adc1->setAveraging(16);
  adc->adc1->setResolution(12);
  adc->adc1->recalibrate();
  adc->adc1->setAveraging(16);
  adc->adc1->setResolution(12);
  //  adc->adc1->enableInterrupts();
  adc->adc1->enableDMA(); //ADC2_GC |= (0x1 << 1); //

  dma.begin(true);
  dma.TCD->SADDR = &ADC2_R0;
  dma.TCD->SOFF = 0;
  dma.TCD->SLAST = 0;
  dma.TCD->DADDR = adcBuffer;
  dma.TCD->DOFF = 2;
  dma.TCD->DLASTSGA = -sizeof(adcBuffer);
  dma.TCD->ATTR = DMA_TCD_ATTR_SSIZE(1) | DMA_TCD_ATTR_DSIZE(1);
  dma.TCD->NBYTES = 2;
  dma.TCD->CITER_ELINKNO  = sizeof(adcBuffer) / 2;
  dma.TCD->BITER_ELINKNO  = sizeof(adcBuffer) / 2;
  dma.TCD->CSR |= DMA_TCD_CSR_INTMAJOR | DMA_TCD_CSR_INTHALF;
  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC2);
  dma.enable();
  dma.attachInterrupt(DMA_ISR);

  //  dma.begin(true);
  //  dma.source(ADC2_R0);
  //  dma.destinationBuffer(adcBuffer, sizeof(adcBuffer));
  //  dma.transferSize(2);
  //  dma.TCD->CSR |= DMA_TCD_CSR_INTMAJOR | DMA_TCD_CSR_INTHALF;
  //  dma.triggerAtHardwareEvent(DMAMUX_SOURCE_ADC2);
  //  dma.enable();
  //  dma.attachInterrupt(DMA_ISR);




  adc->adc1->stopPDB();
  adc->adc1->startSingleRead(A13);
  //  adc->adc0->startSingleRead(A13);
  adc->adc1->startPDB(44100);


  //  myTimer.begin(adcRead, 1000000.0f / 44100.0f);
}


void loop() {
  delay(1000);
  __disable_irq();
  for (int i = 0; i < 128; i++) {
    if (i % 16 == 0) Serial.println();
    Serial.print(outputBlock[i]);
    Serial.print(" ");
  }
  Serial.print("\nLast val of ADC2_R0: ");
  Serial.println(ADC2_R0);
  __enable_irq();
}
