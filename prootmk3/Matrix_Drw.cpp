#include "led-matrix.h"
#include "spritemath.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <unistd.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;

FrameCanvas* offscreen_canvas = nullptr;
// Copy an image to a Canvas.
void CopyImageToCanvas(const cv::Mat &image, Canvas *canvas) {
  const int offset_x = 0, offset_y = 0;  // If you want to move the image.
  
  cv::Mat rgb_image;
  cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);  // Convert BGR to RGB

  // Copy all the pixels to the canvas.
  for (int y = 0; y < rgb_image.rows; ++y) {
    for (int x = 0; x < rgb_image.cols; ++x) {
      cv::Vec3b pixel = rgb_image.at<cv::Vec3b>(y, x);
      canvas->SetPixel(x + offset_x, y + offset_y,
                       pixel[0],  // Red
                       pixel[1],  // Green
                       pixel[2]); // Blue
    }
  }
}

RGBMatrix* InitializeMatrix() {

  rgb_matrix::RuntimeOptions runtime_opt;
  RGBMatrix::Options options;

  //options.pwm_lsb_nanoseconds = 300;
  options.show_refresh_rate = false;
  //options.limit_refresh_rate_hz = 300;
  //options.disable_hardware_pulsing=true;
  options.rows = 32;
  options.cols = 64;
  options.chain_length = 2;
  options.hardware_mapping = "adafruit-hat-pwm";
  runtime_opt.gpio_slowdown = 4;
  //runtime_opt.drop_privileges = 1; 
  RGBMatrix* matrix = RGBMatrix::CreateFromOptions(options, runtime_opt);
  if (matrix == NULL) {
    fprintf(stderr, "Failed to initialize matrix.\n");
  }
  offscreen_canvas = matrix->CreateFrameCanvas();
  return matrix;
}

void DisplayImage(RGBMatrix* matrix, SpriteMath& spritemath, bool debugmode) {
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
  if(!debugmode){
  CopyImageToCanvas(FULLSCREEN, offscreen_canvas);
  offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
  offscreen_canvas -> Clear();
  }
  else{ 
  cv::imshow("Protogen Output", SpriteCanvas);
  }
}
