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

#include <stdlib.h>

#include <jack/jack.h>
#include <jack/types.h>
#include <jack/ringbuffer.h>
#include "jack_stuff.h"
#include "helper.h"

int process(jack_nframes_t nframes, void* jack_stuff_raw) {
  JackStuff* jack_stuff = (JackStuff*)jack_stuff_raw;

  // open audioport
  void* jack_audio_in_buffer =  jack_port_get_buffer ( jack_stuff->audio_in_port, nframes);

  // copy samples from audioport to ringbuffer
  size_t space = jack_ringbuffer_write_space(jack_stuff->audio_in_ringbuffer);
  size_t nsamples = min(space / sizeof(float), nframes);
  int written1 = jack_ringbuffer_write(jack_stuff->audio_in_ringbuffer, jack_audio_in_buffer, nsamples * sizeof(float));

  // notify data avaible
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

  // notification mutex + conditional variable
  pthread_mutex_init(&jack_stuff->audio_event_thread_lock, NULL);
  pthread_cond_init(&jack_stuff->data_ready, NULL);

  jack_stuff->client = jack_client_open (client_name,
                                         JackNullOption,
                                         0,
                                         0 );
  jack_stuff->audio_in_port  = jack_port_register (jack_stuff->client,
                                                  "audio_input",
                                                  JACK_DEFAULT_AUDIO_TYPE,
                                                  JackPortIsInput, 0);

  const size_t ringbuffer_size = 48000 * sizeof(float);
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

