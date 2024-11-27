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

#ifndef SPECTRUM_GUI_H_
#define SPECTRUM_GUI_H_

#include <stdbool.h>
#include <unistd.h>
#include <raylib.h>
#include "simple_dft.h"

typedef struct SpectrumGui{
  Shader circle;
  Shader smear;
  float* spectrum_data;
  float* spectrum_log_data;
  float* spectrum_smooth;
  float* spectrum_smear;
  size_t size;
  size_t size_logbins;
  SimpleDFT* dft;
} SpectrumGui;


SpectrumGui *init_spectrum_gui(size_t size);
bool update_gui(SpectrumGui* spectrum_gui, float* data, size_t data_size, bool new_data);
void free_spectrum_gui(SpectrumGui* spectrum_gui);

#endif // SPECTRUM_GUI_H_
