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

#include "stdio.h"
#include <stdlib.h>
#include "math.h"
#include "simple_dft.h"
#include "simple_fft.h"

void signal_gen(float* signal, size_t size, float freq){
  for (size_t t = 0; t < size; ++t)
  {
    signal[t] = sinf(2.0f*M_PI * t * freq);
  }
}

int main(void){
  const size_t size = 4096;

  float signal[size];
  signal_gen(signal,size,50.0);

  SimpleDFT* dft = create_simple_dft(size);
  SimpleFFT* fft = create_simple_fft(size);

  // TODO calc stuff
  //calc_simple_fft(SimpleFFT* simple_fft, float* in, float* out);
  //calc_simple_fft(SimpleDFT* simple_fft, float* in, float* out);

  // TODO print results
  free_simple_dft(dft);
  free_simple_fft(fft);
}
