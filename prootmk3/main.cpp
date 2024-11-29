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
#include <stdlib.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
RGBMatrix* InitializeMatrix();
void DisplayImage(RGBMatrix* matrix, const cv::Mat &image );

SpriteMath spritemath;

Expression Smile;
int main() {
  //TODO : Obfuscate this implementation of expressions behind a init system in other files
  Smile.Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Eye", .wait_time=10, .MainChords=EyeHappyArray, .OposingChords=EyeClosedHappyArray , .NeedBezier=true, .wait_time2=0.01});
  Smile.Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .wait_time=10, .MainChords=NoseHappyArray});
  Smile.Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .wait_time=10, .MainChords=MouthHappyArray});
  Smile.ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .isGif=true, .MediaPath="color_maps/WHOH1.gif"});
  static short n=0;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  // Create a binary mask 

while (true){ 
    spritemath.SpriteUpdate(Smile);
    spritemath.SpriteColorMapUpdate(Smile);
    
    if (Change==true) {
      //TODO: Obvuscate this whole process
      // Apply the mask to the image using the bitwise_and operation
      cv::Mat SpriteCanvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::Mat Left_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::Mat RightFrame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::Mat FULLSCREEN = cv::Mat::zeros(cv::Size(128, 32), CV_8UC1);
      for (auto const& [key, val] : spritemath.InUseSprites){
        cv::add(val, SpriteCanvas, SpriteCanvas);
      }
      cv::bitwise_and(spritemath.InUseColorMap, spritemath.InUseColorMap, RightFrame, SpriteCanvas);
      cv::flip(RightFrame, Left_Frame, 1 );
      cv::hconcat(RightFrame, Left_Frame, FULLSCREEN);
      DisplayImage(matrix, FULLSCREEN);
      Change=false;
    }
  }
  matrix->Clear();
  delete matrix;
  return 0;
}
