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
#include "config.hpp"


using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
RGBMatrix* InitializeMatrix();
void DisplayImage(RGBMatrix* matrix, const cv::Mat &image );

int main() {
  SpriteMath spritemath;
  Buffer b("/dev/ttyACM0", 9600);
  //var declarations
  cv::Mat bakrnd_frame;

  short n=0;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  // Create a binary mask 
  cv::Mat sprite_canvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);

while (true){ 
    //TODO: Preprocess the gif to reduce the insane processing power required for this to work
    //TODO: obvuscate this whole process behind more .cpp files
    // Gif reader logic for background image
    bakrndUpdate.elapsed_time = std::chrono::steady_clock::now() - bakrndUpdate.action_time;

    if(bakrndUpdate.elapsed_time.count() >= bakrndUpdate.wait_time)
    {
        bakrnd_frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
        int CurrentFrame = static_cast<int>(BluePinkLR.video.get(cv::CAP_PROP_POS_FRAMES));
        if (CurrentFrame >= BluePinkLR.TOTAL_FRAMES){
            BluePinkLR.video.set(cv::CAP_PROP_POS_FRAMES, 0);
        }
        BluePinkLR.video.read(bakrnd_frame);
        Change = true;
        bakrndUpdate.action_time = std::chrono::steady_clock::now();
    }

    spritemath.EyeUpdate();

    spritemath.MouthUpdate();

    spritemath.NoseUpdate();

    if (Change==true) {
      //TODO: Obvuscate this whole process
      // Apply the mask to the image using the bitwise_and operation
      cv::Mat RightFrame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::Mat Left_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::Mat FULLSCREEN = cv::Mat::zeros(cv::Size(128, 32), CV_8UC1);
      cv::add(NoseSprite, EyeSprite, sprite_canvas);
      cv::add(sprite_canvas, MouthSprite, sprite_canvas);
      cv::bitwise_and(bakrnd_frame, bakrnd_frame, RightFrame, sprite_canvas);
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