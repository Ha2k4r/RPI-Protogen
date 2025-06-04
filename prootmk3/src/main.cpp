#include "include/led-matrix.h"
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <boost/asio.hpp>
#include "spritemath.hpp"
#include "globals.hpp"
#include <stdlib.h>
#include "Config.hpp"

SpriteMath spritemath;
Expression Smile, Blush, Clock;

#ifndef TESTING_ENVIRONMENT
  #include "serialport.hpp"
  #include "buffer.hpp"
  using rgb_matrix::RGBMatrix;
  using rgb_matrix::FrameCanvas;
  RGBMatrix* InitializeMatrix();
  void DisplayImage(SpriteMath& spritemath, RGBMatrix* matrix);
  #else 
    void DisplayImage(SpriteMath& SpriteMath);
  #endif 
int main() {
  std::vector<Expression> Faces = {Smile , Blush, Clock};
  init(Faces);
  //start the matrix and create a object for it
  #ifndef TESTING_ENVIRONMENT 
    RGBMatrix* matrix = InitializeMatrix();
    #endif
  Expression* SelectedFace = &Faces[0];

while (true){ 
    spritemath.SpriteUpdate(*SelectedFace);
    
    spritemath.SpriteColorMapUpdate(*SelectedFace);

    if (spritemath.Change_==true) {
      //apply the mask to the image and display it to the screen
      #ifdef TESTING_ENVIRONMENT
        DisplayImage(spritemath);
        #else
          DisplayImage(spritemath, matrix);
        #endif
      spritemath.Change_=false;
    }
  }
  return 0;
}