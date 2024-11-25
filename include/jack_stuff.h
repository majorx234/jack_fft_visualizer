#ifndef JACK_STUFF_H_
#define JACK_STUFF_H_

#include <jack/types.h>
#include <jack/ringbuffer.h>
#include <jack/jack.h>

typedef struct JackStuff{
  jack_client_t* client;
  jack_port_t* audio_in_port;
  jack_ringbuffer_t* audio_in_ringbuffer;
  pthread_mutex_t audio_event_thread_lock;
  pthread_cond_t data_ready;
} JackStuff;


JackStuff* create_jack_stuff(char* client_name);
void jack_stuff_clear(JackStuff* jack_stuff);

#endif // JACK_STUFF_H_
