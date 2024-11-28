///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of jack_fft_visualizer                               //
//    Copyright (C) 2024 <majorx234@googlemail.com>                          //
//                                                                           //
//    jack_fft_visualizer is free software:                                  //
//    you can redistribute it and/or modify                                  //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    jack_fft_visualizer is distributed in the hope that it will be useful, //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with jack_fft_visualizer.                                        //
//    If not, see <http://www.gnu.org/licenses/>.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "simple_fft.h"
#include <assert.h>
#include <math.h>
#include <complex.h>

SimpleFFT* create_simple_fft(size_t size) {
  SimpleFFT* new_fft = (SimpleFFT*)malloc(sizeof(SimpleFFT));
  new_fft->size = size;

  // Hann Window
  new_fft->hann_window = (float*)malloc(new_fft->size * sizeof(float));
  for (size_t k=0;k<size;k++) {
    new_fft->hann_window[k] = (0.5 * (1 - cos(2*M_PI*k/size)));
  }

  return new_fft;
}

void calc_simple_fft(SimpleFFT* simple_fft, float* in, float* out){
  float* windowed_in = (float*)calloc(simple_fft->size, sizeof(float));

  for(size_t h = 0; h<simple_fft->size; ++h){
    float window = simple_fft->hann_window[h];
    float value = in[h] * window;
    windowed_in[h] = value;
  }

}

void free_simple_fft(SimpleFFT* simple_fft){
}
