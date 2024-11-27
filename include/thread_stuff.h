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

#ifndef THREAD_STUFF_H_
#define THREAD_STUFF_H_

#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include <jack/ringbuffer.h>


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

void* audio_visualizer_thread_fct(void* thread_stuff_raw);

#endif
