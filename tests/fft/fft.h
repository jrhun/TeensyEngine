
#include "arm_math.h"


extern "C" {
  extern const int16_t AudioWindowHanning1024[];
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

struct block {
  uint16_t data[128];
}

class FFT {
  public:
    FFT() : window(AudioWindowHanning1024), state(0), outputflag(false) {
      arm_cfft_radix4_init_q15(&fft_inst, 1024, 0, 1);
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
    void update() {

      // read 256 samples
      uint16_t sample = analogRead(

      if (window) apply_window_to_fft_buffer(buffer, window);
      arm_cfft_radix4_q15(&fft_inst, buffer);
      break;
      for (int i = 0; i < 512; i++) {
        uint32_t tmp = *((uint32_t *)buffer + i); // real & imag
        uint32_t magsq = multiply_16tx16t_add_16bx16b(tmp, tmp);
        output[i] = sqrt_uint32_approx(magsq);
      }
      outputflag = true;

    }
    uint16_t output[512] __attribute__ ((aligned (4)));
  private:
    const int16_t *window;
    volatile bool outputflag;
    uint8_t state;
    int16_t buffer[2048] __attribute__ ((aligned (4)));
    arm_cfft_radix4_instance_q15 fft_inst;
};
