#ifndef SIMPLE_DFT_H_
#define SIMPLE_DFT_H_

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <complex.h> 

typedef struct SimpleDFT{
  size_t size;
  float complex* fourier_matrix;
  float* hann_window;
} SimpleDFT;

SimpleDFT *create_simple_dft(size_t size);
void* free_simple_dft(SimpleDFT *simple_dft);

#endif // SIMPLE_DFT_H_
