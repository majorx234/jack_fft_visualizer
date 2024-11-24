cmake_minimum_required(VERSION 3.16)
project(jack_fft_visualizer)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Jack REQUIRED)
find_package(raylib REQUIRED)

set(SIMPLE_DFT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_dft.c
)

set(SPECTRUM_GUI_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/spectrum_gui.c
)

set(JACK_FFT_VIZUALIZER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)


add_library(simple_dft)
target_sources(simple_dft
 PRIVATE
  ${SIMPLE_DFT_SOURCES}
)
target_include_directories(simple_dft
 PRIVATE
  include
)
target_link_libraries(simple_dft
 PRIVATE
  m
)

add_library(spectrum_gui)
target_sources(spectrum_gui
 PRIVATE
  ${SPECTRUM_GUI_SOURCES}
)
target_include_directories(spectrum_gui
 PRIVATE
  include
  ${raylib_INCLUDE_DIRS}
)
target_link_libraries(spectrum_gui
 PRIVATE
  ${raylib_LIBRARIES}
)

add_executable(jack_fft_visualizer)
target_sources(jack_fft_visualizer
 PRIVATE
  ${JACK_FFT_VIZUALIZER_SOURCES}
)
target_include_directories(jack_fft_visualizer
 PRIVATE
  ${JACK_INCLUDE_DIRS}
)
target_link_libraries(jack_fft_visualizer
 PRIVATE
  ${JACK_LIBRARIES}
  spectrum_gui
)

