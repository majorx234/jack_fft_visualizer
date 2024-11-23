#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/jack.h>

#define min(a, b) \
  ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
  })

#define max(a,b) \
  ({ \
      __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; \
  })


typedef struct JackStuff{
  jack_client_t* client;
  jack_port_t* audio_in_port;
  jack_ringbuffer_t* audio_in_ringbuffer;
  pthread_mutex_t audio_event_thread_lock;
  pthread_cond_t data_ready;
} JackStuff;

int process(jack_nframes_t nframes, void* jack_stuff_raw) {
  JackStuff* jack_stuff = (JackStuff*)jack_stuff_raw;
  // midi_in event handling:
  void* jack_audio_in_buffer =  jack_port_get_buffer ( jack_stuff->audio_in_port, nframes);

  size_t space = jack_ringbuffer_write_space(jack_stuff->audio_in_ringbuffer);
  size_t nsamples = min(space / sizeof(float), nframes);
  int written1 = jack_ringbuffer_write(jack_stuff->audio_in_ringbuffer, jack_audio_in_buffer, nsamples);
  if (pthread_mutex_trylock (&jack_stuff->audio_event_thread_lock) == 0) {
    pthread_cond_signal (&jack_stuff->data_ready);
    pthread_mutex_unlock (&jack_stuff->audio_event_thread_lock);
  }
  return 0;
}

JackStuff* create_jack_stuff(char* client_name){
  JackStuff* jack_stuff = (JackStuff*)malloc(sizeof(JackStuff));

  jack_stuff->audio_in_port = NULL;
  jack_stuff->audio_in_ringbuffer = NULL;
  jack_stuff->client = NULL;
  pthread_mutex_init(&jack_stuff->audio_event_thread_lock, NULL);
  pthread_cond_init(&jack_stuff->data_ready, NULL);

  jack_stuff->client = jack_client_open (client_name,
                                         JackNullOption,
                                         0,
                                         0 );
  const size_t ringbuffer_size = 48000 * sizeof(float);
  jack_stuff->audio_in_port  = jack_port_register (jack_stuff->client,
                                                  "audio_input",
                                                  JACK_DEFAULT_AUDIO_TYPE,
                                                  JackPortIsInput, 0);
  jack_stuff->audio_in_ringbuffer = jack_ringbuffer_create(ringbuffer_size);

  jack_set_process_callback(jack_stuff->client, process, (void*)jack_stuff);
  //client.set_sample_rate(48000);
  jack_activate(jack_stuff->client);
  return jack_stuff;
}

void jack_stuff_clear(JackStuff* jack_stuff) {
  jack_deactivate(jack_stuff->client);
  jack_client_close(jack_stuff->client);
  jack_ringbuffer_free(jack_stuff->audio_in_ringbuffer);
  free(jack_stuff);
}

int main(void){
  char client_name[] = "jack_fft_visualizer\n";
  JackStuff* jack_stuff = create_jack_stuff(client_name);
  sleep(1);
  jack_stuff_clear(jack_stuff);
}
