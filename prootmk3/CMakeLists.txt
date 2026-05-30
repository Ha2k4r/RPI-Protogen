cmake_minimum_required(VERSION 3.16)
project(prootmk3 C CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "\n  In-source builds are not allowed.\n  Run from the repo root:\n    cmake -S prootmk3 -B prootmk3/build\n    cmake --build prootmk3/build\n")
endif()

# === Options ===

option(TESTING_ENVIRONMENT "ON = desktop OpenCV display, OFF = real RGB matrix output" OFF)
option(VERBOSE_LOGGING "Enable verbose debug output" OFF)

set(SYSROOT "" CACHE PATH "Path to aarch64 sysroot for cross-compilation (set when using rpi presets)")

# === Cross-compilation guard ===

if(CMAKE_CROSSCOMPILING)
  if(NOT SYSROOT AND NOT DEFINED ENV{SYSROOT})
    message(WARNING "\n"
      "  Cross-compiling without library paths set.\n"
      "  Options:\n"
      "    - Set -DSYSROOT=/path/to/debian-bookworm-arm64-rootfs\n"
      "    - Or: export SYSROOT=/path/to/debian-bookworm-arm64-rootfs\n"
      "    - NixOS: enter nix-shell which sets SYSROOT automatically\n"
    )
  endif()
endif()

# === Dependencies ===

find_package(PkgConfig QUIET)

# === GStreamer GIF runtime check ===
# Animated GIF colormaps require a GIF decoder in GStreamer at runtime.
# Either gifdec (gstreamer1.0-plugins-good) or avdec_gif
# (gstreamer1.0-libav) works. If missing, cv::VideoCapture cannot load
# GIFs and sprites will render without colorization.
#
# Install on Debian/Raspbian (either is sufficient):
#   sudo apt install gstreamer1.0-plugins-good
#   sudo apt install gstreamer1.0-libav

if(CMAKE_CROSSCOMPILING)
  # Can't run gst-inspect on the build machine during cross-compilation.
  message(STATUS
    "Cross-compiling: GIF decoder check skipped (cannot run gst-inspect on target).\n"
    "  GIFs require gstreamer1.0-plugins-good or gstreamer1.0-libav on the Pi.")
else()
  find_program(GST_INSPECT gst-inspect-1.0)
  if(GST_INSPECT)
    execute_process(COMMAND ${GST_INSPECT} --exists gifdec
      RESULT_VARIABLE GST_GIF_EXISTS
      OUTPUT_QUIET ERROR_QUIET)
    if(NOT GST_GIF_EXISTS EQUAL 0)
      set(GST_GIF_MISSING TRUE)
    endif()
  else()
    # gst-inspect-1.0 not on PATH — try finding libgstgif.so directly.
    # Note: gstreamer1.0-libav (avdec_gif) also works but won't be
    # detected here since it's named libavcodec_gst.so or similar.
    message(STATUS "gst-inspect-1.0 not found — checking for libgstgif.so on disk")
    find_file(GST_GIF_SO libgstgif.so
      PATHS /usr/lib /usr/lib/aarch64-linux-gnu /usr/lib/arm-linux-gnueabihf
      PATH_SUFFIXES gstreamer-1.0
      NO_DEFAULT_PATH)
    if(NOT GST_GIF_SO)
      set(GST_GIF_MISSING TRUE)
    endif()
  endif()
  if(GST_GIF_MISSING)
    message(WARNING
      "GStreamer GIF decoder not found (neither gifdec nor avdec_gif).\n"
      "  Install (either is sufficient):\n"
      "    sudo apt install gstreamer1.0-plugins-good\n"
      "    sudo apt install gstreamer1.0-libav\n"
      "  GIF animated colormaps will NOT load at runtime without a GIF decoder.")
  endif()
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgproc videoio highgui)

# Boost.System is header-only since 1.75, so no COMPONENTS needed.
set(Boost_NO_BOOST_CMAKE ON)
cmake_policy(PUSH)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()
find_package(Boost REQUIRED)
cmake_policy(POP)

# === Speed up rebuilds with ccache ===

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  message(STATUS "ccache found — enabled")
endif()

# === Bundled hzeller library ===

add_subdirectory(lib)

# === Main executable ===

set(SOURCES
  src/Config.cpp
  src/expression-vectors.cpp
  src/globals.cpp
  src/main.cpp
  src/Matrix_Drw.cpp
  src/serialport.cpp
  src/spritemath.cpp
)

add_executable(proot ${SOURCES})

target_include_directories(proot PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(proot PRIVATE
  $<$<BOOL:${TESTING_ENVIRONMENT}>:TESTING_ENVIRONMENT>
  $<$<BOOL:${VERBOSE_LOGGING}>:VERBOSE_LOGGING>
)

target_compile_options(proot PRIVATE
  -Wall -Wextra -Wno-unused-parameter
  $<$<CONFIG:Debug>:-O0 -g3 -Wno-cpp>
  $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

target_link_libraries(proot PRIVATE
  rgbmatrix
  Boost::boost
  opencv_core
  opencv_imgproc
  opencv_videoio
  opencv_highgui
)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries(proot PRIVATE rt pthread m)
endif()

# === Dead code elimination (strip unused RP1 backend etc.) ===
# Not available on Windows (mingw ld supports --gc-sections but needs testing)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_options(proot PRIVATE -Wl,--gc-sections)
endif()

# === Avoid embedding build-machine paths in cross-compiled binaries ===

if(CMAKE_CROSSCOMPILING)
  set(CMAKE_SKIP_RPATH ON)
endif()

# === IPO / LTO for release builds ===

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET proot PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "IPO / LTO enabled (Release)")
endif()

# === Run helper (auto-chdirs so color_maps/ resolves) ===

add_custom_target(run
  COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR} $<TARGET_FILE:proot>
  DEPENDS proot
)

# === Platform notes ===

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND EXISTS "/etc/os-release")
  file(READ "/etc/os-release" OS_RELEASE)
  if(OS_RELEASE MATCHES "ubuntu|debian")
    message(STATUS "Debian/Ubuntu: sudo apt install libopencv-dev libboost-all-dev")
  elseif(OS_RELEASE MATCHES "nixos")
    message(STATUS "NixOS: all deps provided by nix-shell")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Windows build: install OpenCV + Boost via vcpkg or MSYS2")
endif()
