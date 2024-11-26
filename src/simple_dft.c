#include "simple_dft.h"
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <stdio.h>

SimpleDFT* create_simple_dft(size_t size) {
  SimpleDFT* new_dft = (SimpleDFT*)malloc(sizeof(SimpleDFT));
  new_dft->size = size;

  // Hann Window
  new_dft->hann_window = (float*)malloc(new_dft->size * sizeof(float));
  for (size_t k=0;k<size;k++) {
    new_dft->hann_window[k] = (0.5 * (1 - cos(2*M_PI*k/size)));
  }
  new_dft->fourier_matrix = (float complex*)malloc(size*size*sizeof(float complex));

  for(size_t i=0;i<size;i++){
    for(size_t j=0;j<size;j++){
      new_dft->fourier_matrix[i * size + j] = cexp(-2*M_PI*i*j/size);
    }
  }

  return new_dft;
}

void calc_simple_dft(SimpleDFT* simple_dft, float* in, float* out){
  float* windowed_in = (float*)calloc(simple_dft->size, sizeof(float));

  for(size_t h = 0; h<simple_dft->size; ++h){
    float window = simple_dft->hann_window[h];
    float value = in[h] * window;
    windowed_in[h] = value;
  }

  for(size_t i = 0; i<simple_dft->size; ++i){
    float complex freq = 0;
    for(size_t j = 0; j<simple_dft->size; j++){
      freq+= simple_dft->fourier_matrix[i * simple_dft->size + j] * windowed_in[j];
    }
    float freq_real = crealf(freq);
    float freq_img = cimagf(freq);
    // TODO: adjust logarithmic scale
    out[i] = logf(100* (freq_real*freq_real + freq_img*freq_img));
  }
}

void free_simple_dft(SimpleDFT* simple_dft){
  if (simple_dft){
    free(simple_dft->hann_window);
    free(simple_dft->fourier_matrix);
    free(simple_dft);
  }
}
