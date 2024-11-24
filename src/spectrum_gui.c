#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <unistd.h>

#include "spectrum_gui.h"


SpectrumGui *init_spectrum_gui(){
  SpectrumGui* spectrum_gui = (SpectrumGui*)malloc(sizeof(SpectrumGui));
  return spectrum_gui;
}

void run_gui(SpectrumGui* spectrum_gui){
  InitWindow(800, 600, "SpectrumGui");
  SetTargetFPS(60);
  while(!WindowShouldClose()){
    if (IsKeyPressed(KEY_SPACE)) {
      break;
    }
  }
}

void free_spectrum_gui(SpectrumGui* spectrum_gui){
  if(spectrum_gui){
    free(spectrum_gui);
  }
}
