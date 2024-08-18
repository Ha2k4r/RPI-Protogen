#include <string>
#include "led-matrix.h"
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <chrono>

//screen stoof
using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
// Other file prerequisits 
RGBMatrix* InitializeMatrix();
void DisplayImage(RGBMatrix* matrix, const cv::Mat &image );

// this allows for the protogens timers to work with more percision than sleep()
// also its a class which allows as many timers as we may need. personally i found this
// to be the best way to aproach this issue

struct timeValues {
  // holds the time of the last action in place so math arround it can happen
  std::chrono::time_point<std::chrono::steady_clock> action_time = std::chrono::steady_clock::now();
  // THIS should be allowed to change by the user
  double wait_time;
  // current time - action time = how long has passed = elapsed time
  std::chrono::duration<double> elapsed_time;
};


int main() {

  cv::VideoCapture cap("color_maps/WHOH1.gif");
  //void image in memory 
  cv::Mat frame;
  int TOTAL_FRAMES = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();
  struct timeValues blinkValues;
  blinkValues.wait_time = 0.042;



  while (true){ 
    //has time elapsed enough to blink?
    auto now = std::chrono::steady_clock::now();
    blinkValues.elapsed_time = now - blinkValues.action_time;
    if(blinkValues.elapsed_time.count() >= blinkValues.wait_time)
    {
      int CurrentFrame = static_cast<int>(cap.get(cv::CAP_PROP_POS_FRAMES));
      
      if (CurrentFrame >= TOTAL_FRAMES){
        cap.set(cv::CAP_PROP_POS_FRAMES, 0);
      }
    
      bool success = cap.read(frame);
      DisplayImage(matrix, frame);
      if (success == false){
        std::cout << "current frame : " << CurrentFrame << std::endl << "MaxFrames : " << TOTAL_FRAMES;
        std::cout << "breaking out of video file" << std::endl;
        break;
      }
      blinkValues.action_time = std::chrono::steady_clock::now();
    }



    

  }

  matrix->Clear();
  delete matrix;

  return 0;
}