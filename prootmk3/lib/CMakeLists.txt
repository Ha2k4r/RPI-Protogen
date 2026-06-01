cmake_minimum_required(VERSION 3.16)
project(rgbmatrix C CXX)

set(RGBMATRIX_SUBMODULE ${CMAKE_CURRENT_SOURCE_DIR}/rpi-rgb-led-matrix)

set(CXX_SOURCES
  ${RGBMATRIX_SUBMODULE}/lib/bdf-font.cc
  ${RGBMATRIX_SUBMODULE}/lib/content-streamer.cc
  ${RGBMATRIX_SUBMODULE}/lib/content-streamer-c.cc
  ${RGBMATRIX_SUBMODULE}/lib/framebuffer.cc
  ${RGBMATRIX_SUBMODULE}/lib/gpio.cc
  ${RGBMATRIX_SUBMODULE}/lib/graphics.cc
  ${RGBMATRIX_SUBMODULE}/lib/led-matrix.cc
  ${RGBMATRIX_SUBMODULE}/lib/led-matrix-c.cc
  ${RGBMATRIX_SUBMODULE}/lib/multiplex-mappers.cc
  ${RGBMATRIX_SUBMODULE}/lib/options-initialize.cc
  ${RGBMATRIX_SUBMODULE}/lib/pixel-mapper.cc
  ${RGBMATRIX_SUBMODULE}/lib/thread.cc
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_backend.cc
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_rio_backend.cc
)

set(C_SOURCES
  ${RGBMATRIX_SUBMODULE}/lib/hardware-mapping.c
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_support.c
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_vendor/piolib/piolib.c
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_vendor/piolib/pio_rp1.c
)

add_library(rgbmatrix STATIC ${CXX_SOURCES} ${C_SOURCES})

target_include_directories(rgbmatrix PUBLIC SYSTEM
  ${RGBMATRIX_SUBMODULE}/include
)
target_include_directories(rgbmatrix PRIVATE
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_vendor/piolib/include
  ${RGBMATRIX_SUBMODULE}/lib/rp1/rp1_pio_vendor/include
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
