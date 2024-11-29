#include <string>
#include "led-matrix.h"
#include "expression-vectors.hpp"
#include <chrono>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <boost/asio.hpp>
#include "serialport.hpp"
#include "buffer.hpp"
#include "spritemath.hpp"
#include "globals.hpp"
#include "Setup.hpp"
#include <stdlib.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
RGBMatrix* InitializeMatrix();
void DisplayImage(RGBMatrix* matrix, SpriteMath& spritemath);

SpriteMath spritemath;
Expression Smile;
Expression Blush;
int main() {
  std::vector<Expression> Faces = { Smile , Blush};
  init(Faces);
  static short n=0;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  Expression* SelectedFace = &Faces[0];

while (true){ 

    spritemath.SpriteUpdate(*SelectedFace);

    spritemath.SpriteColorMapUpdate(*SelectedFace);

    if (Change==true) {
      //apply the mask to the image and display it to the screen
      DisplayImage(matrix,spritemath);
      Change=false;
    }
  }
  matrix->Clear();
  delete matrix;
  return 0;
}