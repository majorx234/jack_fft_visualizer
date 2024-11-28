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

#ifndef SIMPLE_FFT_H_
#define SIMPLE_FFT_H_

#include <stdlib.h>
#include <unistd.h>
#include <complex.h>

typedef struct SimpleFFT{
  size_t size;
  float* hann_window;
  float complex* result;
} SimpleFFT;

SimpleFFT *create_simple_fft(size_t size);
void calc_simple_fft(SimpleFFT *simple_fft, float* in, float* out);
void free_simple_fft(SimpleFFT *simple_fft);

#endif // SIMPLE_FFT_H_
