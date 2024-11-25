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
