#include "simple_dft.h"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h> 

SimpleDFT* create_simple_dft(size_t size) {
  float complex* new_fourier_matrix = (float complex*)malloc(size*size*sizeof(float complex));
  SimpleDFT* new_dft = (SimpleDFT*)malloc(sizeof(SimpleDFT));

  for(size_t i=0;i<size;i++){
    for(size_t j=0;j<size;j++){
      new_fourier_matrix[i * size + j] = cexp(-2*M_PI*i*j/size);
    }
  }
  new_dft->size = size;
  new_dft->fourier_matrix = new_fourier_matrix;
  return new_dft;
}

void calc_simple_dft(SimpleDFT* simple_dft, float* in, float* out){
  for(size_t i = 0; i<simple_dft->size; i++){
    float complex freq = 0;
    for(size_t j = 0; j<simple_dft->size; j++){
      freq+= simple_dft->fourier_matrix[i * simple_dft->size + j] * in[j];
    }
    out[i]+= cabsf(freq);
  }
}

void* free_simple_dft(SimpleDFT* simple_dft){
  free(simple_dft->fourier_matrix);
  free(simple_dft);
}
