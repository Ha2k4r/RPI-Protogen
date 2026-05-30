#!/usr/bin/env bash
set -euo pipefail

# Creates a Debian Bookworm arm64 sysroot for cross-compiling prootmk3.
#
# Usage:
#   sudo ./tools/create-rpi-sysroot.sh [output-dir]
#
# Requirements:
#   - debootstrap (apt install debootstrap, or add pkgs.debootstrap to shell.nix)
#   - sudo (for chroot-based second stage)
#
# Alternative (no root, no debootstrap):
#   Instead of this script, rsync from a running Pi:
#     rsync -av pi@<ip>:/usr/lib/aarch64-linux-gnu/ /path/to/sysroot/usr/lib/aarch64-linux-gnu/
#     rsync -av pi@<ip>:/usr/include/ /path/to/sysroot/usr/include/
#     rsync -av pi@<ip>:/lib/aarch64-linux-gnu/ /path/to/sysroot/lib/aarch64-linux-gnu/
#
# The sysroot contains everything needed for the cross toolchain to link
# binaries compatible with Debian Bookworm (glibc 2.36).
#
# Dependencies included in the sysroot:
#   Core:     libc6-dev, libstdc++-dev
#   OpenCV:   libopencv-dev (pulls core, imgproc, highgui, videoio, etc.)
#   Vision:   libavcodec-dev, libavformat-dev, libavutil-dev,
#             libswscale-dev, libswresample-dev
#   Image:    libpng-dev, libjpeg-dev, libtiff-dev, libwebp-dev
#   Math:     libopenexr-dev, libtbb-dev
#   IO:       libdc1394-dev, libboost-dev

SYSROOT="${1:-"$(cd "$(dirname "$0")/.." && pwd)/build/rpi-sysroot"}"
# libc6-dev + linux-libc-dev: glibc & kernel headers (missing from the
# rsync-based sysroot, causing the Nix cross-compiler to fall back to
# its own glibc 2.42 headers and emit GLIBC_2.38 symbols).
PACKAGES="libc6-dev,linux-libc-dev,libstdc++-12-dev"
PACKAGES="$PACKAGES,libopencv-dev,libboost-dev"
PACKAGES="$PACKAGES,libavcodec-dev,libavformat-dev,libavutil-dev"
PACKAGES="$PACKAGES,libswscale-dev,libswresample-dev"
PACKAGES="$PACKAGES,libpng-dev,libjpeg-dev,libtiff-dev,libwebp-dev"
PACKAGES="$PACKAGES,libdc1394-dev,libtbb-dev,libopenexr-dev"

if ! command -v debootstrap &>/dev/null; then
  echo "ERROR: debootstrap not found."
  echo ""
  echo "Install it:"
  echo "  Debian/Ubuntu:  apt install debootstrap"
  echo "  NixOS:          add pkgs.debootstrap to buildInputs in shell.nix"
  echo ""
  echo "Alternative (without debootstrap):"
  echo "  rsync from a running Pi -- see script header for details."
  exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
  echo "Re-running with sudo (debootstrap needs chroot for package install) ..."
  echo "  sudo SYSROOT=$SYSROOT $(realpath "$0")"
  exec sudo SYSROOT="$SYSROOT" "$(realpath "$0")"
fi

echo "Creating Debian Bookworm arm64 sysroot at: $SYSROOT"
echo "Packages: $PACKAGES"
echo ""

debootstrap --arch=arm64 \
  --include="$PACKAGES" \
  --variant=minbase \
  bookworm "$SYSROOT" http://deb.debian.org/debian

echo ""
echo "Sysroot created at: $SYSROOT"
echo ""
echo "To cross-compile prootmk3:"
echo "  export SYSROOT=$SYSROOT"
echo "  cd prootmk3"
echo "  cmake --preset rpi-release"
echo "  cmake --build --preset rpi-release"
echo ""
echo "Copy to Pi (binary expects color_maps/ relative to cwd):"
echo "  rsync -avz build/rpi-release/proot DEVICE@IP:~/RPI-Protogen/prootmk3/build/rpi-release/proot"
echo "  ssh DEVICE@IP 'cd ~/RPI-Protogen/prootmk3 && sudo ./build/rpi-release/proot'"
