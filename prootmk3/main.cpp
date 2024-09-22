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

//#include "PathCalculator.cpp"
//screen stoof
using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
// Other file prerequisits 
RGBMatrix* InitializeMatrix();
void DisplayImage(RGBMatrix* matrix, const cv::Mat &image );
cv::Point* BezierCurveCalculation(const cv::Point* Array,const cv::Point* Target_Array, int num_points,int arraysize, double factor1 = 0.5 , double factor2 = 1.8 );
const cv::Point* UnpackBezierArray( int Index, cv::Point* RawBezierArray, int numpoints, int maxindex);
struct timeValues {
  // holds the time of the last action in place so math arround it can happen
  std::chrono::time_point<std::chrono::steady_clock> action_time;
  // THIS should be allowed to change by the user
  double wait_time;
  // current time - action time = how long has passed = elapsed time
  std::chrono::duration<double> elapsed_time;
};

//this was added soley because i intend to expand this 
//project and needed a programatic way of handling
//new color maps so enjoy my spagetti
struct color_map {
  cv::VideoCapture video;
  //no reason to have this so i did it anyways
  //might be usefull in the future for like a easter egg or somthing idk
  std::string location;
  int TOTAL_FRAMES;
  //initializer
  color_map(const std::string& video_path) : video(video_path) {
    if (!video.isOpened()) {
      std::cerr << "Error: Could not open the video file." << std::endl;
    } else {
      TOTAL_FRAMES = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));
    }
  }
};

struct EyeArray {
    const cv::Point* DEFAULT[1];  // Array of pointers to cv::Point arrays
    const cv::Point* CLOSED[1];    // Other arrays for different expressions
    int arraysize = (sizeof(DEFAULT)/8);
    // more to be added for advanced expressions
};
struct NoseArray {
    const cv::Point* DEFAULT[1];  // Array of pointers to cv::Point arrays
};
struct MouthArray {
    const cv::Point* DEFAULT[1];  // Array of pointers to cv::Point arrays
};

int main() {
  struct MouthArray morm;
  struct NoseArray norm;
  struct EyeArray Happy;
  struct timeValues bakrndUpdate;
  struct timeValues timeBetweenBlink;
  struct timeValues blinkSpeed;
  struct timeValues noseUpdate;
  struct timeValues mouthUpdate;
  Buffer b("/dev/ttyACM0", 9600);
//  struct timeValues serialRead;
  //user changeable variables

  int blink_Cycles = 30;
  bakrndUpdate.wait_time = 0.042; // TODO: this needs a comment lol????
  timeBetweenBlink.wait_time = 10;
  blinkSpeed.wait_time = 0.01;
  noseUpdate.wait_time = 100;
  mouthUpdate.wait_time = 100;
 // serialRead.wait_time = 0.1;
  //hardcoded thingies
  struct color_map BluePinkLR("color_maps/WHOH1.gif");
  //var declarations
  cv::Mat bakrnd_frame;
  bool Change= false;
  bool Blinking=false;
  bool eyeOpening=true;
  short n=0;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  // Create a binary mask 
  cv::Mat EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1); // TODO: yayyy i love magic numbers
  cv::Mat NoseSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
  cv::Mat MouthSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
  cv::Mat sprite_canvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
        

  const int numPointsEye[]   = { 9 };
  const int numPointsMouth[] = { 14 };
  const int numPointsNose[]  = { 10 };

  morm.DEFAULT[0] = MouthHappyArray;
  norm.DEFAULT[0] = NoseHappyArray;
  Happy.CLOSED[0] = EyeClosedHappyArray;
  Happy.DEFAULT[0] = EyeHappyArray;
  int arraySize = 9;


  
  //*********************************************************************MAIN LOOP************ */
while (true){ 
    cv::Point* RawBezierEye; // Initialize to nullptr
    const cv::Point* EyeVerticies; // Initialize to nullptr
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

    timeBetweenBlink.elapsed_time = std::chrono::steady_clock::now() - timeBetweenBlink.action_time;
    if (Blinking == true) {
        blinkSpeed.elapsed_time = std::chrono::steady_clock::now() - blinkSpeed.action_time;
        if (blinkSpeed.elapsed_time.count() >= blinkSpeed.wait_time){
            if (eyeOpening == true){
                if (n < blink_Cycles - 1){
                    n++;
                }
                else {
                    delete[] RawBezierEye;
                    RawBezierEye = BezierCurveCalculation(Happy.CLOSED[0], Happy.DEFAULT[0], blink_Cycles, arraySize);
                    n = 0;
                    eyeOpening = false;
                }
            }
            else {
                if (n < blink_Cycles - 1){
                    n++; 
                }
                else {
                    eyeOpening = true;
                    Blinking = false;
                    timeBetweenBlink.action_time = std::chrono::steady_clock::now();
                    delete[] RawBezierEye;
                    RawBezierEye = nullptr; // Prevent double deletion
                    n = 0;
                    continue;
                }
            }
            
            int Sizeof_RawBezierEye = arraySize * blink_Cycles;
            EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles, Sizeof_RawBezierEye);
            cv::fillPoly(EyeSprite, &EyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            blinkSpeed.action_time = std::chrono::steady_clock::now();
            Change = true;
            delete[] EyeVerticies; 
        }
    }
    else if (timeBetweenBlink.elapsed_time.count() >= timeBetweenBlink.wait_time)
    {
        n = 0;
        int Sizeof_RawBezierEye = (arraySize * blink_Cycles) * 8;
        RawBezierEye = BezierCurveCalculation(Happy.DEFAULT[0], Happy.CLOSED[0], blink_Cycles, arraySize);
        EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles, Sizeof_RawBezierEye);
        EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
        cv::fillPoly(EyeSprite, &EyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
        timeBetweenBlink.action_time = std::chrono::steady_clock::now();
        Change = true;
        //delete[] EyeVerticies;
        Blinking = true;
        delete[] EyeVerticies; 
    }
    noseUpdate.elapsed_time = std::chrono::steady_clock::now() - noseUpdate.action_time;
    if(noseUpdate.elapsed_time.count() >= noseUpdate.wait_time){
      NoseSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::fillPoly(NoseSprite, &norm.DEFAULT[0],numPointsNose , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
    }
    mouthUpdate.elapsed_time = std::chrono::steady_clock::now() - mouthUpdate.action_time;
    if(mouthUpdate.elapsed_time.count() >= mouthUpdate.wait_time){
      MouthSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      cv::fillPoly(MouthSprite, &morm.DEFAULT[0], numPointsMouth, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
    }
        auto returned = b.readBuf();
        if (returned.empty()) {
          //std::cout << "null ptr" << std::endl;
        }
        else {
          for (std::pair<fullDescriptor, int16_t>& i : returned){
            fullDescriptor name = std::get<0>(i);
            int16_t value = std::get<1>(i);
          }
        }
        /*
        // Check if serial data has changed and use it as needed
        // Check if the read data is valid (e.g., no default/invalid values)
        if (returned[0] == -1 || returned[1] == -1 || returned[2] == -1) {

        } else {
            //std::cout << "Received values: " << returned[0] << std::endl;
            matrix->SetBrightness(returned[0]);
        
    }
    */
  if (Change==true) {
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

