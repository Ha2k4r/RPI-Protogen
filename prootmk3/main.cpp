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
SpriteMath spritemath;
void DisplayImage(RGBMatrix* matrix, const cv::Mat &image );

int main() {
  for (const std::string& port : Ports) {
        try {
            std::cout << "Trying port: " << port << std::endl;
            // Attempt to initialize Buffer (assuming Buffer is your serial connection class)
            Buffer b(port, 9600);
            
            std::cout << "Connected successfully to " << port << std::endl;
            break;  // Exit the loop once a successful connection is made
        } catch (...) {
            std::cout << "FAIL: Could not connect to " << port << std::endl;
            if (port == Ports[Ports.size()-1]){
              std::cout << "\n ERROR, SerialPort not detected and all functionality is now DISABLED. \n";
            }
        }
    }
  //var declarations
  cv::Mat bakrnd_frame;

  static short n=0;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  // Create a binary mask 
  cv::Mat sprite_canvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);

while (true){ 
    //TODO: Preprocess the gif to reduce the insane processing power required for this to work
    //TODO: obvuscate this whole process behind more .hpp files
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
  /*
    auto returned = b.readBuf();
    if (returned.empty()) {
      //std::cout << "null ptr" << std::endl;
    }
    else {
      for (std::pair<fullDescriptor, int16_t>& i : returned){
        fullDescriptor name = std::get<0>(i);
        int16_t value = std::get<1>(i);
        matrix->SetBrightness(value);
        std::cout << "Value : " << value << std::endl;
      }
    }*/

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
