#include "simple_dft.h"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <complex.h> 

SimpleDFT* create_simple_dft(size_t size) {
  float complex* new_fourier_matrix = (float complex*)malloc(size*size*sizeof(float complex));

  for(size_t i=0;i<size;i++){
    for(size_t j=0;j<size;j++){
      new_fourier_matrix[i * size + j] = cexp(-2*M_PI*i*j/size);
    }
  }

  // Hann Window
  float* hann_window = (float*)malloc(size*sizeof(float));
  for (size_t k;k<size;k++) {
    hann_window[k] = (0.5 * (1 - cos(2*M_PI*k/size)));
  }

  SimpleDFT* new_dft = (SimpleDFT*)malloc(sizeof(SimpleDFT));
  new_dft->size = size;
  new_dft->fourier_matrix = new_fourier_matrix;
  new_dft->hann_window = hann_window;
  return new_dft;
}

void calc_simple_dft(SimpleDFT* simple_dft, float* in, float* out){
  float* windowed_in = (float*)calloc(simple_dft->size, sizeof(float*));
  for(size_t h = 0; h<simple_dft->size; h++){
    windowed_in[h] = in[h] * simple_dft->hann_window[h];
  }
  for(size_t i = 0; i<simple_dft->size; i++){
    float complex freq = 0;
    for(size_t j = 0; j<simple_dft->size; j++){
      freq+= simple_dft->fourier_matrix[i * simple_dft->size + j] * windowed_in[j];
    }
    float freq_real = crealf(freq);
    float freq_img = cimagf(freq);
    out[i] = logf(freq_real*freq_real + freq_img*freq_img);
  }
}

void* free_simple_dft(SimpleDFT* simple_dft){
  free(simple_dft->fourier_matrix);
  free(simple_dft);
}
