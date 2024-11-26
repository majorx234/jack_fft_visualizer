#ifndef SIMPLE_DFT_H_
#define SIMPLE_DFT_H_

#include <stdlib.h>
#include <unistd.h>
#include <complex.h>

typedef struct SimpleDFT{
  size_t size;
  float* hann_window;
  float complex* fourier_matrix;
} SimpleDFT;

SimpleDFT *create_simple_dft(size_t size);
void calc_simple_dft(SimpleDFT *simple_dft, float* in, float* out);
void free_simple_dft(SimpleDFT *simple_dft);

#endif // SIMPLE_DFT_H_
