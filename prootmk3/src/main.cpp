#include "Config.hpp"
#include "globals.hpp"
#include "led-matrix.h"
#include "spritemath.hpp"
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

SpriteMath spritemath;
Expression Smile, Blush, Clock;

#ifndef TESTING_ENVIRONMENT
#include "buffer.hpp"
#include "serialport.hpp"
using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;
RGBMatrix* InitializeMatrix();
void DisplayImage(SpriteMath& spritemath, RGBMatrix* matrix);
#else
void DisplayImage(SpriteMath& SpriteMath);
#endif

#ifdef VERBOSE_LOGGING
static constexpr bool verbose = true;
#else
static constexpr bool verbose = false;
#endif

int main() {
  std::vector<Expression> Faces = {Smile, Blush, Clock};
  init(Faces);
// start the matrix and create a object for it
#ifndef TESTING_ENVIRONMENT
  RGBMatrix* matrix = InitializeMatrix();
#endif
  Expression* SelectedFace = &Faces[0];
  if (verbose) {
    std::cerr << "[VERBOSE] Initialized " << Faces.size() << " expressions" << std::endl;
    std::cerr << "[VERBOSE] Mode: "
#ifdef TESTING_ENVIRONMENT
              << "desktop (OpenCV)"
#else
              << "RPi (RGB matrix)"
#endif
              << std::endl;
  }

  while (true) {
    spritemath.SpriteUpdate(*SelectedFace);

    spritemath.SpriteColorMapUpdate(*SelectedFace);

    if (spritemath.Change_ == true) {
// apply the mask to the image and display it to the screen
#ifdef TESTING_ENVIRONMENT
      DisplayImage(spritemath);
#else
      DisplayImage(spritemath, matrix);
#endif
      spritemath.Change_ = false;
    }
  }
  return 0;
}
