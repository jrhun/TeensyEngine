#include "arm_math.h"


extern "C" {
extern const int16_t AudioWindowHanning1024[];
}


class FFT {
public:
  FFT() : {}  


private:
  arm_cfft_radix4_instance_q15 fft_inst;
};
