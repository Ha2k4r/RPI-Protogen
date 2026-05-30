# ARM64 (aarch64) toolchain for Raspberry Pi (Debian Bookworm)
#
# On native aarch64 (building directly on a Pi), no sysroot is needed.
#
# On cross (NixOS): The bundled glibc 2.42 headers define C23 symbols that
# require GLIBC_2.38 (not on Pi).  We bypass them with -nostdinc + explicit
# Debian Bookworm sysroot headers via -isystem.
#
# Usage:
#   cmake --preset rpi-release -DSYSROOT=/path/to/sysroot
#   or:  export SYSROOT=/path/to/sysroot   (nix-shell does this automatically)
#
# To create a sysroot:
#   tools/create-rpi-sysroot.sh /path/to/output

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
  return()
endif()

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# --- Compiler ---
if(EXISTS "/etc/os-release")
  file(READ "/etc/os-release" OS_RELEASE)
  if(OS_RELEASE MATCHES "nixos")
    set(CMAKE_C_COMPILER aarch64-unknown-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-unknown-linux-gnu-g++)
    set(_IS_NIXOS TRUE)
  else()
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
  endif()
else()
  set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
  set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
endif()

# --- Sysroot ---
if(SYSROOT)
elseif(DEFINED ENV{SYSROOT})
  set(SYSROOT "$ENV{SYSROOT}")
else()
  get_filename_component(_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
  foreach(_candidate
      "${_root}/prootmk3/build/rpi-sysroot"
      "${_root}/build/rpi-sysroot"
  )
    if(EXISTS "${_candidate}/usr/lib/aarch64-linux-gnu/libc.so.6")
      set(SYSROOT "${_candidate}")
      break()
    endif()
  endforeach()
endif()

if(NOT SYSROOT)
  message(FATAL_ERROR "\n"
    "  No aarch64 sysroot found for cross-compilation.\n"
    "  Set -DSYSROOT=/path/to/debian-bookworm-arm64-rootfs or\n"
    "  export SYSROOT=/path/to/debian-bookworm-arm64-rootfs\n"
    "  See tools/create-rpi-sysroot.sh\n")
endif()

message(STATUS "SYSROOT=${SYSROOT}")

set(CMAKE_FIND_ROOT_PATH ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_LIBRARY_PATH
  "${SYSROOT}/usr/lib/aarch64-linux-gnu"
  "${SYSROOT}/lib/aarch64-linux-gnu"
  "${SYSROOT}/usr/lib"
)

# --- NixOS cross-compiler: bypass bundled glibc 2.42 headers ---
# The Nix cross-compiler ships a sys-include dir (glibc 2.42) that is
# searched BEFORE -isystem/-idirafter, emitting C23 symbols (__isoc23_strtol,
# __isoc23_sscanf) requiring GLIBC_2.38.  Use -nostdinc to remove all
# built-in paths and manually add GCC + Debian sysroot includes.
if(_IS_NIXOS)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libgcc.a
    OUTPUT_VARIABLE _LIBGCC_A
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  get_filename_component(_LIBGCC_DIR "${_LIBGCC_A}" DIRECTORY)
  get_filename_component(_GCC_ROOT "${_LIBGCC_DIR}/../../../.." ABSOLUTE)

  file(GLOB _CXX_VERSION_DIRS "${_GCC_ROOT}/include/c++/*")
  list(GET _CXX_VERSION_DIRS 0 _CXX_DIR)
  set(_CXX_PLATFORM "${_CXX_DIR}/aarch64-unknown-linux-gnu")
  set(_GCC_INTERNAL "${_GCC_ROOT}/lib/gcc/aarch64-unknown-linux-gnu/${_CXX_DIR}/include")
  # fix paths with /15.2.0/ in them — glob got the version dir, find the gcc one
  file(GLOB _GCC_VERSION_DIRS "${_GCC_ROOT}/lib/gcc/aarch64-unknown-linux-gnu/*")
  list(GET _GCC_VERSION_DIRS 0 _GCC_VER_DIR)
  set(_GCC_INTERNAL "${_GCC_VER_DIR}/include")

  set(_NostdincFlags
    "-nostdinc"
    "-isystem ${_GCC_INTERNAL}"
    "-isystem ${_CXX_DIR}"
    "-isystem ${_CXX_PLATFORM}"
    "-isystem ${SYSROOT}/usr/include/aarch64-linux-gnu"
    "-isystem ${SYSROOT}/usr/include"
  )
  string(REPLACE ";" " " CMAKE_C_FLAGS_INIT "${_NostdincFlags}")
  string(REPLACE ";" " " CMAKE_CXX_FLAGS_INIT "${_NostdincFlags}")
  message(STATUS "GCC internal headers: ${_GCC_INTERNAL}")
  message(STATUS "C++ headers: ${_CXX_DIR}")
endif()

# The Nix cross-compiler's built-in search paths are Nix store paths,
# NOT Debian multiarch paths.  We add rpath-link manually for transitive
# dep resolution.
set(_LinkerFlags
  "-Wl,-rpath-link,${SYSROOT}/usr/lib/aarch64-linux-gnu"
  "-Wl,-rpath-link,${SYSROOT}/lib/aarch64-linux-gnu"
  "-Wl,--allow-shlib-undefined"
  "-Wl,--dynamic-linker,/lib/ld-linux-aarch64.so.1"
  "-Wl,-rpath,/lib/aarch64-linux-gnu:/usr/lib/aarch64-linux-gnu"
)
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS_INIT "${_LinkerFlags}")

# --- OpenCV ---
set(_ocv_dir "${SYSROOT}/usr/lib/aarch64-linux-gnu/cmake/opencv4")
if(EXISTS "${_ocv_dir}/OpenCVConfig.cmake")
  set(OpenCV_DIR "${_ocv_dir}" CACHE PATH "" FORCE)
  message(STATUS "OpenCV_DIR=${OpenCV_DIR}")
else()
  message(WARNING "OpenCV not found in sysroot (${_ocv_dir}/OpenCVConfig.cmake)")
endif()

# --- Boost (header-only since 1.75) ---
if(EXISTS "${SYSROOT}/usr/include/boost/version.hpp")
  set(Boost_INCLUDE_DIR "${SYSROOT}/usr/include" CACHE PATH "" FORCE)
  message(STATUS "Boost_INCLUDE_DIR=${Boost_INCLUDE_DIR}")
else()
  message(WARNING "Boost not found in sysroot")
endif()
