#include "Arduino.h"
#include "arm_math.h"

#include "dspinst.h"
#include "ADC.h"
ADC *adc = new ADC();
volatile bool dataReady = false;
volatile bool blockOneActive = true;

typedef struct audio_block_struct {
  int16_t  data[128];
} audio_block_t;
audio_block_t block1;
audio_block_t block2;

#define COEF_HPF_DCBLOCK    (1048300<<10) // DC Removal filter coefficient in S1.30

void adc0_isr() {

}
void adc1_isr() {
  static uint8_t blockOffset = 0;
  if (blockOneActive) {
    block1.data[blockOffset] = adc->adc1->readSingle();
  } else {
    block2.data[blockOffset] = adc->adc1->readSingle();
  }
  blockOffset++;
  if (blockOffset == 128) {
    blockOffset = 0;
    blockOneActive = !blockOneActive;
    dataReady = true;
  }
}

audio_block_t * getBlock() {
  __disable_irq();
  bool b = blockOneActive;
  __enable_irq();
  if (b)
    return &block2;
  else
    return &block1;
}

extern "C" {
  extern const int16_t AudioWindowHanning1024[];
}

static void copy_to_fft_buffer(void *destination, const void *source)
{
  const uint16_t *src = (const uint16_t *)source;
  uint32_t *dst = (uint32_t *)destination;

  for (int i = 0; i < 128; i++) {
    *dst++ = *src++;  // real sample plus a zero for imaginary
  }
}

static void apply_window_to_fft_buffer(void *buffer, const void *window)
{
  int16_t *buf = (int16_t *)buffer;
  const int16_t *win = (int16_t *)window;;

  for (int i = 0; i < 1024; i++) {
    int32_t val = *buf * *win++;
    //*buf = signed_saturate_rshift(val, 16, 15);
    *buf = val >> 15;
    buf += 2;
  }

}



class FFT {
  public:
    FFT() : window(AudioWindowHanning1024), state(0), outputflag(false) {
//      arm_cfft_radix4_init_q15(&fft_inst, 1024, 0, 1);
      //      arm_cfft_q15(&fft_inst, 1024, 0, 1);
      arm_rfft_fast_init_f32(&fft_inst, 1024);
    }

    void init() {
      adc->adc1->setAveraging(8);
      adc->adc1->setResolution(10);
      adc->adc1->recalibrate();
      adc->adc1->setAveraging(8);
      adc->adc1->setResolution(10);

      int32_t tmp = (uint16_t) adc->adc1->analogRead(A13);
      tmp = ( ((int32_t) tmp) << 14);
      hpf_x1 = tmp;   // With constant DC level x1 would be x0
      hpf_y1 = 0;     // Output will settle here when stable

      adc->adc1->enableInterrupts();

      adc->adc1->stopPDB();
      adc->adc1->startSingleRead(A13);
      adc->adc1->startPDB(44100);
    }

    bool available() {
      if (outputflag == true) {
        outputflag = false;
        return true;
      }
      return false;
    }
    float read(unsigned int binNumber) {
      if (binNumber > 511) return 0.0;
      return (float)(output[binNumber]) * (1.0 / 16384.0);
    }
    float read(unsigned int binFirst, unsigned int binLast) {
      if (binFirst > binLast) {
        unsigned int tmp = binLast;
        binLast = binFirst;
        binFirst = tmp;
      }
      if (binFirst > 511) return 0.0;
      if (binLast > 511) binLast = 511;
      uint32_t sum = 0;
      do {
        sum += output[binFirst++];
      } while (binFirst <= binLast);
      return (float)sum * (1.0 / 16384.0);
    }
    void windowFunction(const int16_t *w) {
      window = w;
    }

    void removeDCoffset(uint8_t blockID) {
      int16_t s, *p, *end;
      int32_t tmp;
      p = blocklist[blockID]->data;
      end = p + 128;
      do {
        
        tmp = (int16_t)(*p);
//        tmp -= hpf_x1;
//        tmp *= 32;
//        *p++ = tmp;
        tmp = ( ((int32_t) tmp) << 14);
        int32_t acc = hpf_y1 - hpf_x1;
        acc += tmp;
        hpf_y1 = FRACMUL_SHL(acc, COEF_HPF_DCBLOCK, 1);
        hpf_x1 = tmp;
        s = signed_saturate_rshift(hpf_y1, 16, 14);
        s *= 32;
        *p++ = s;
      } while (p < end);
    }
    void test() {
      double khz_period = 1.0 / 10000;
      double seconds_16k = 1.0 / 44100;
      float b[2048];
      float out[1024];
      for (int i = 0; i < 1024; i++)
      {
        // I need to figure out the encoding for q15_t!
        uint16_t input = (int16_t)( 4096 * cosf( TWO_PI * ((i * seconds_16k) / khz_period) ) );
        b[i] = ( 1 * cosf( TWO_PI * ((i * seconds_16k) / khz_period) ) );
        //        buffer[i] = input;
        //        buffer[i+1] = 0;
      }
//      copy_to_fft_buffer(buffer + 0x000, &b[0]);
//      copy_to_fft_buffer(buffer + 0x100, &b[128 * 1]);
//      copy_to_fft_buffer(buffer + 0x200, &b[128 * 2]);
//      copy_to_fft_buffer(buffer + 0x300, &b[128 * 3]);
//      copy_to_fft_buffer(buffer + 0x400, &b[128 * 4]);
//      copy_to_fft_buffer(buffer + 0x500, &b[128 * 5]);
//      copy_to_fft_buffer(buffer + 0x600, &b[128 * 6]);
//      copy_to_fft_buffer(buffer + 0x700, &b[128 * 7]);
      arm_rfft_fast_f32(&fft_inst, b, out, 0);
//      arm_cfft_radix4_q15(&fft_inst, buffer);
      for (int i = 0; i < 512; i++) {
//        uint32_t tmp = *((uint32_t *)buffer + i); // real & imag
//        uint32_t magsq = multiply_16tx16t_add_16bx16b(tmp, tmp);
//        output[i] = sqrt_uint32_approx(magsq);
        output[i] = out[i];
      }
      outputflag = true;
    }
    void update() {
      if (!dataReady) {
        //         Serial.println("Data not ready");
        return;
      }
      //      Serial.println("Data ready");
      // read 256 samples
      audio_block_t *block;

      __disable_irq();
      block = getBlock();
      if (!block) return;

      switch (state) {
        case 0:
          blocklist[0] = block;
          __enable_irq();
          removeDCoffset(0);
          state = 1;
          break;
        case 1:
          blocklist[1] = block;
          __enable_irq();
          removeDCoffset(1);
          state = 2;
          break;
        case 2:
          blocklist[2] = block;
          __enable_irq();
          removeDCoffset(2);
          state = 3;
          break;
        case 3:
          blocklist[3] = block;
          __enable_irq();
          removeDCoffset(3);
          state = 4;
          break;
        case 4:
          blocklist[4] = block;
          __enable_irq();
          removeDCoffset(4);
          state = 5;
          break;
        case 5:
          blocklist[5] = block;
          __enable_irq();
          removeDCoffset(5);
          state = 6;
          break;
        case 6:
          blocklist[6] = block;
          __enable_irq();
          removeDCoffset(6);
          state = 7;
          break;
        case 7:
          blocklist[7] = block;
          __enable_irq();
          removeDCoffset(7);
          // TODO: perhaps distribute the work over multiple update() ??
          //       github pull requsts welcome......

          copy_to_fft_buffer(buffer + 0x000, blocklist[0]->data);
          copy_to_fft_buffer(buffer + 0x100, blocklist[1]->data);
          copy_to_fft_buffer(buffer + 0x200, blocklist[2]->data);
          copy_to_fft_buffer(buffer + 0x300, blocklist[3]->data);
          copy_to_fft_buffer(buffer + 0x400, blocklist[4]->data);
          copy_to_fft_buffer(buffer + 0x500, blocklist[5]->data);
          copy_to_fft_buffer(buffer + 0x600, blocklist[6]->data);
          copy_to_fft_buffer(buffer + 0x700, blocklist[7]->data);

          //          Serial.print("raw data: ");
          //          for (int i = 0; i < 1024; i++) {
          //            if (i % 16 == 0) Serial.println();
          //            Serial.print(buffer[i*2]);
          //            Serial.print(" ");
          //          }

          if (window) apply_window_to_fft_buffer(buffer, window);
//          arm_cfft_radix4_q15(&fft_inst, buffer);
          //          arm_cfft_q15(&fft_inst, buffer, 0, 0);

          for (int i = 0; i < 512; i++) {
            uint32_t tmp = *((uint32_t *)buffer + i); // real & imag
            uint32_t magsq = multiply_16tx16t_add_16bx16b(tmp, tmp);
            output[i] = sqrt_uint32_approx(magsq);
          }
          //          Serial.println("Output ready");
          outputflag = true;
      }
      dataReady = false;
    }
    uint16_t output[512] __attribute__ ((aligned (4)));
  private:
    const int16_t *window;
    volatile bool outputflag;
    uint8_t state;
    int16_t buffer[2048] __attribute__ ((aligned (4)));
//    arm_cfft_radix4_instance_q15 fft_inst;
    //    arm_cfft_instance_q15 fft_inst;
    arm_rfft_fast_instance_f32 fft_inst;
    audio_block_t *blocklist[8];
    int32_t hpf_y1 = 0;
    int32_t hpf_x1 = 0;
};
