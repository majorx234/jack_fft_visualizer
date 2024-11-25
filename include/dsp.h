#include <unistd.h>

void smoother(float* out_log,
              float* out_smooth,
              float* out_smear,
              float size,
              float smoothness,
              float dt,
              float smearness);
float sqash_logarithmic(float* in, float* outlog, size_t size);
