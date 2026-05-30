cmake_minimum_required(VERSION 3.16)
project(rgbmatrix C CXX)

set(CXX_SOURCES
  bdf-font.cc
  content-streamer.cc
  content-streamer-c.cc
  framebuffer.cc
  gpio.cc
  graphics.cc
  led-matrix.cc
  led-matrix-c.cc
  multiplex-mappers.cc
  options-initialize.cc
  pixel-mapper.cc
  thread.cc
  rp1/rp1_pio_backend.cc
  rp1/rp1_rio_backend.cc
)

set(C_SOURCES
  hardware-mapping.c
  rp1/rp1_pio_support.c
  rp1/rp1_pio_vendor/piolib/piolib.c
  rp1/rp1_pio_vendor/piolib/pio_rp1.c
)

add_library(rgbmatrix STATIC ${CXX_SOURCES} ${C_SOURCES})

target_include_directories(rgbmatrix PUBLIC SYSTEM
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)
target_include_directories(rgbmatrix PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/rp1/rp1_pio_vendor/piolib/include
  ${CMAKE_CURRENT_SOURCE_DIR}/rp1/rp1_pio_vendor/include
)

target_compile_options(rgbmatrix PRIVATE
  -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -O3 -fPIC
  -ffunction-sections -fdata-sections
)

set_target_properties(rgbmatrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
)

foreach(src ${CXX_SOURCES})
  set_source_files_properties(${src} PROPERTIES COMPILE_FLAGS "-fno-exceptions")
endforeach()

target_link_libraries(rgbmatrix PUBLIC rt pthread m)
