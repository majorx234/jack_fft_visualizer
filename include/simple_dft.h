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
