#include <unistd.h>
#include <math.h>

float sqash_logarithmic(float* in, float* outlog, size_t size){
  float step = 1.06;
  float lowf = 1.0f;
  size_t m = 0;
  float max_amp = 1.0f;

  // "Squash" into the Logarithmic Scale
  for(float f = lowf; (size_t) f < size/2; f = ceilf(f*step)) {
    float f1 = ceilf(f*step);
    float a = 0.0f;
    for(size_t q = (size_t) f; q < size/2 && q < (size_t) f1; ++q) {
      float b = in[q];
      if (b > a) a = b;
    }
    if(max_amp < a) max_amp = a;
    outlog[m] = a;
    m++;
  }
  return max_amp;
}

void smoother(float* out_log,
              float* out_smooth,
              float* out_smear,
              float size,
              float smoothness,
              float dt,
              float smearness){
  for(size_t i = 0; i < size; ++i) {
    out_smooth[i] += (out_log[i] - out_smooth[i])*smoothness*dt;
    out_smear[i] += (out_smooth[i] - out_smear[i])*smearness*dt;
  }
}
