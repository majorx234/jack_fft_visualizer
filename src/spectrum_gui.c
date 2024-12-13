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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include <raylib.h>
#include <rlgl.h>

#include "spectrum_gui.h"
#include "dsp.h"
#include "simple_dft.h"

SpectrumGui* init_spectrum_gui(size_t size){
  SpectrumGui* spectrum_gui = (SpectrumGui*)malloc(sizeof(SpectrumGui));

  //todo: better calculation of size_logbins
  size_t m = 0;
  float step = 1.06;
  float lowf = 1.0f;

  for(float f = lowf; (size_t) f < size/2; f = ceilf(f*step)) {
    m++;
  }
  m = m-1;
  spectrum_gui->size = size;
  spectrum_gui->size_logbins = m;
  spectrum_gui->spectrum_data = (float*)malloc(size*sizeof(float));
  spectrum_gui->spectrum_log_data = (float*)malloc(spectrum_gui->size_logbins*sizeof(float));
  spectrum_gui->spectrum_smooth = (float*)malloc(spectrum_gui->size_logbins*sizeof(float));
  spectrum_gui->spectrum_smooth_tmp = spectrum_gui->spectrum_smooth;
  spectrum_gui->spectrum_smear = (float*)malloc(spectrum_gui->size_logbins*sizeof(float));
  spectrum_gui->dft = create_simple_dft(size);

  InitWindow(800, 600, "SpectrumGui");
  SetTargetFPS(60);
  spectrum_gui->circle = LoadShader(NULL, "../shader/circle.fs");
  spectrum_gui->smear = LoadShader(NULL, "../shader/smear.fs");
  spectrum_gui->use_smoothener = true;
  spectrum_gui->smoothnes = 8;
  spectrum_gui->smearnes = 6;

  return spectrum_gui;
}

bool update_gui(SpectrumGui* spectrum_gui, float* data, size_t data_size, bool new_data){
  int w = GetRenderWidth();
  int h = GetRenderHeight();
  float dt = GetFrameTime();

  bool close = WindowShouldClose();
  BeginDrawing();
  if (IsKeyPressed(KEY_SPACE)) {
    printf("space pressed\n");
    close = true;
  }

  if(!new_data){
    EndDrawing();
    return close;
  }
  // only redraw if new data is avaible
  ClearBackground(BLACK);


  calc_simple_dft(spectrum_gui->dft, data, spectrum_gui->spectrum_data);
  squash_logarithmic(spectrum_gui->spectrum_data, spectrum_gui->spectrum_log_data, spectrum_gui->size);

  if(spectrum_gui->use_smoothener){
    spectrum_gui->spectrum_smooth = spectrum_gui->spectrum_smooth_tmp;
    smoother(spectrum_gui->spectrum_log_data,
             spectrum_gui->spectrum_smooth,
             spectrum_gui->spectrum_smear,
             spectrum_gui->size_logbins,
             spectrum_gui->smoothnes,
             dt,
             spectrum_gui->smearnes);
  } else {
    spectrum_gui->spectrum_smooth = spectrum_gui->spectrum_log_data;
  }

  float cell_width = (float)w/spectrum_gui->size_logbins;

  float saturation = 0.75f;
  float value = 1.0f;

  for (size_t i = 0; i < spectrum_gui->size_logbins; ++i) {
    float hue = (float)i/spectrum_gui->size_logbins;
    float t = spectrum_gui->spectrum_smooth[i];
    Color color = ColorFromHSV(hue*360, saturation, value);
    Vector2 start_pos = {
      i*cell_width + cell_width/2,
      h - h*2/3*t,
    };

    Vector2 end_pos = {
      i*cell_width + cell_width/2,
      h,
    };
    float thick = cell_width/2*sqrtf(t);
    DrawLineEx(start_pos, end_pos, thick, color);
  }


  Texture2D texture = {rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  BeginShaderMode(spectrum_gui->smear);
  for (size_t i = 0; i < spectrum_gui->size_logbins; ++i) {
    float start = spectrum_gui->spectrum_smear[i];
    float end = spectrum_gui->spectrum_smooth[i];
    float hue = (float)i/spectrum_gui->size_logbins;
    Color color = ColorFromHSV(hue*360, saturation, value);
    Vector2 start_pos = {
      i*cell_width + cell_width/2,
      h - h*2/3*start,
    };
    Vector2 end_pos = {
      i*cell_width + cell_width/2,
      h - h*2/3*end,
    };
    float radius = cell_width*sqrtf(end);
    Vector2 origin = {0};
    if( end_pos.y >= start_pos.y) {
      Rectangle dest = {
      .x = start_pos.x - radius,
        .y = start_pos.y,
        .width = 2*radius,
        .height = end_pos.y - start_pos.y
      };
      Rectangle source = {0, 0, 1, 0.5};
      DrawTexturePro(texture, source, dest, origin, 0, color);
    } else {
      Rectangle dest = {
      .x = end_pos.x - radius,
        .y = end_pos.y,
        .width = 2*radius,
        .height = start_pos.y - end_pos.y
      };
      Rectangle source = {0, 0.5, 1, 0.5};
      DrawTexturePro(texture, source, dest, origin, 0, color);
    }
  }
  EndShaderMode();

  //Display circles
  BeginShaderMode(spectrum_gui->circle);
  for (size_t i = 0; i < spectrum_gui->size_logbins; ++i) {
    float hue = (float)i/spectrum_gui->size_logbins;
    float t = spectrum_gui->spectrum_smooth[i];
    Color color = ColorFromHSV(hue*360, saturation, value);
    Vector2 center = {
      i*cell_width + cell_width/2,
      h - h*2/3*t,
    };
    float radius = cell_width*5*sqrtf(t);
    Vector2 position = {
      .x = center.x - radius,
      .y = center.y - radius,
    };
    DrawTextureEx(texture, position, 0, 2*radius, color);
  }
  EndShaderMode();
  EndDrawing();

  return close;
}

void free_spectrum_gui(SpectrumGui* spectrum_gui){
  if(spectrum_gui){
    free(spectrum_gui->spectrum_data);
    free(spectrum_gui->spectrum_log_data);
    free(spectrum_gui->spectrum_smooth);
    free(spectrum_gui->spectrum_smear);
    free_simple_dft(spectrum_gui->dft);
    free(spectrum_gui);
  }
}
