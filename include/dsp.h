#ifndef DSP_H_
#define DSP_H_

#include <unistd.h>

void smoother(float* out_log,
              float* out_smooth,
              float* out_smear,
              float size,
              float smoothness,
              float dt,
              float smearness);
float squash_logarithmic(float* in, float* outlog, size_t size);

#endif // DSP_H_
