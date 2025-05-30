#include "include/led-matrix.h"
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <boost/asio.hpp>
//#include "buffer.hpp"
#include "spritemath.hpp"
#include "globals.hpp"
#include "Setup.hpp"
#include <stdlib.h>
#include "Config.hpp"

SpriteMath spritemath;

#ifndef TESTING_ENVIRONMENT
  #include "serialport.hpp"
  using rgb_matrix::RGBMatrix;
  using rgb_matrix::FrameCanvas;
  RGBMatrix* InitializeMatrix();
  void DisplayImage(SpriteMath& spritemath, RGBMatrix* matrix);
#else 
void DisplayImage(SpriteMath& SpriteMath);
#endif 
Expression Smile, Blush, Clock;
int main() {
  std::vector<Expression> Faces = {Smile , Blush, Clock};
  init(Faces);
  static short n=0;
  //start the matrix and create a object for it
  #ifndef TESTING_ENVIRONMENT 
  RGBMatrix* matrix = InitializeMatrix();
  #endif
  Expression* SelectedFace = &Faces[0];

while (true){ 
    spritemath.SpriteUpdate(*SelectedFace);
    
    spritemath.SpriteColorMapUpdate(*SelectedFace);

    if (Change==true) {
      //apply the mask to the image and display it to the screen
      #ifdef TESTING_ENVIRONMENT
      DisplayImage(spritemath);
      #else
      DisplayImage(spritemath, matrix);
      #endif
      Change=false;
    }
  }
  #ifndef TESTING_ENVIRONMENT 
  matrix->Clear();
  delete matrix;
  #endif
  return 0;
}
