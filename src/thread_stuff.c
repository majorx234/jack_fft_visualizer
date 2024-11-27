#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include <stdio.h>
#include <jack/ringbuffer.h>
#include "thread_stuff.h"
#include "spectrum_gui.h"

void* audio_visualizer_thread_fct(void* thread_stuff_raw) {
  ThreadStuff* thread_stuff = (ThreadStuff*) thread_stuff_raw;
  ThreadResult* result = (ThreadResult*)malloc(sizeof( ThreadResult));
  result->status = 0;
  const size_t size = 1024;
  float* data = calloc(size, sizeof(float));
  SpectrumGui* spectrum_gui = init_spectrum_gui(size);

  while(thread_stuff->running){
    // TODO check for data in buffer
    size_t num_bytes = jack_ringbuffer_read_space (thread_stuff->audio_in_ringbuffer);
    bool close = false;
    if(num_bytes < 1024 * sizeof(float)){
      close = update_gui(spectrum_gui, NULL, 0, false);
    } else {
      size_t received_bytes = jack_ringbuffer_read(thread_stuff->audio_in_ringbuffer, (char*)data, size*sizeof(float));
      // TODO check received bytes
      close = update_gui(spectrum_gui, data, size, true);
    }
    if(close) {
      if (pthread_mutex_trylock (&thread_stuff->close_thread_lock) == 0) {
          pthread_cond_signal (&thread_stuff->close_ready);
          pthread_mutex_unlock (&thread_stuff->close_thread_lock);
        }
      break;
    }
  }
  free_spectrum_gui(spectrum_gui);
  printf("finish audio vissualizer thread\n");
  return result;
}
