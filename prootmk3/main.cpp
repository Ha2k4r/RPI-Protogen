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
//this was added soley because i intend to expand this project and needed a programatic way of handling
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

cv::Mat mask(const cv::Mat& image, const cv::Mat& mask) {
    // Check if the image and mask have the same size and type
    if (image.size() != mask.size() || mask.type() != CV_8UC1) {
        std::cerr << "Error: Image and mask must have the same size, and the mask must be a single-channel image." << std::endl;
        return cv::Mat(); // Return an empty Mat if there is an error
    }

    // Create an output image of the same size and type as the input image
    cv::Mat maskedImage;

    // Apply the mask to the image using the bitwise_and operation
    cv::bitwise_and(image, image, maskedImage, mask);

    // Return the masked image
    return maskedImage;
}

int main() {
  struct timeValues bakrnd_update;
  //user changeable variables
  bakrnd_update.wait_time = 0.042;


  //hardcoded thingies
  struct color_map BluePinkLR("color_maps/WHOH1.gif");
  //void image in memory 
  cv::Mat frame;
  cv::Mat bakrnd_frame;
  bool Change;
  //start the matrix and create a object for it
  RGBMatrix* matrix = InitializeMatrix();



  while (true){ 
    //WHILE(it may look gross but it handles the timer problem quite well
    // im open to help from anyone) do this working method
    auto now = std::chrono::steady_clock::now();
    bakrnd_update.elapsed_time = now - bakrnd_update.action_time;
    //has time elapsed enough to update?
    if(bakrnd_update.elapsed_time.count() >= bakrnd_update.wait_time)
    {
      int CurrentFrame = static_cast<int>(BluePinkLR.video.get(cv::CAP_PROP_POS_FRAMES));
      //this loops the mp4
      //yes.. mp4
      if (CurrentFrame >= BluePinkLR.TOTAL_FRAMES){
        BluePinkLR.video.set(cv::CAP_PROP_POS_FRAMES, 0);
      }
      BluePinkLR.video.read(bakrnd_frame);
      Change = true;
      bakrnd_update.action_time = std::chrono::steady_clock::now();
    }
  if (Change==true) {
    DisplayImage(matrix, bakrnd_frame);
    Change=false;
  }
  }
  matrix->Clear();
  delete matrix;
  return 0;
}
