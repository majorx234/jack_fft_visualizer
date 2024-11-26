#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include <jack/ringbuffer.h>

#include <raylib.h>

#include "spectrum_gui.h"
#include "jack_stuff.h"

typedef struct ThreadStuff {
  bool running;
  jack_ringbuffer_t* audio_in_ringbuffer;
  pthread_mutex_t* audio_event_thread_lock;
  pthread_cond_t* data_ready;
  // to notify closing
  pthread_mutex_t close_thread_lock;
  pthread_cond_t close_ready;
} ThreadStuff;

typedef struct ThreadResult {
  int status;
} ThreadResult;

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
      // todo check received bytes
      close = update_gui(spectrum_gui, data, size, true);
    }
    // TODO audio visualization
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

int main(void){
  char client_name[] = "jack_fft_visualizer";
  JackStuff* jack_stuff = create_jack_stuff(client_name);

  pthread_t audio_visualizer_thread;

  ThreadStuff thread_stuff = {
    .running = true,
    .audio_in_ringbuffer = jack_stuff->audio_in_ringbuffer,
    .audio_event_thread_lock = &jack_stuff->audio_event_thread_lock,
    .data_ready = &jack_stuff->data_ready
  };

  // to signal closing notification
  pthread_mutex_init(&thread_stuff.close_thread_lock, NULL);
  pthread_cond_init(&thread_stuff.close_ready, NULL);

  pthread_create(&audio_visualizer_thread, NULL, audio_visualizer_thread_fct,(void *) &thread_stuff);

  // wait to finish programm:
  pthread_cond_wait (&thread_stuff.close_ready, &thread_stuff.close_thread_lock);
  // TODO: still need signal handler to stop programm

  ThreadResult* result = NULL;
  thread_stuff.running = false;
  if (pthread_mutex_trylock (&jack_stuff->audio_event_thread_lock) == 0) {
    pthread_cond_signal (&jack_stuff->data_ready);
    pthread_mutex_unlock (&jack_stuff->audio_event_thread_lock);
  }

  pthread_join(audio_visualizer_thread, (void**)&result);
  printf("received status: %d\n",result->status);
  jack_stuff_clear(jack_stuff);
  free(result);
}
