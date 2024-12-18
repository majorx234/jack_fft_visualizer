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
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include <jack/ringbuffer.h>

#include <raylib.h>

#include "jack_stuff.h"
#include "thread_stuff.h"

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
