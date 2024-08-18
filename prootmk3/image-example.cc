#include "led-matrix.h"
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
  RGBMatrix::Options matrix_options;
  matrix_options.pwm_lsb_nanoseconds = 60;
  //matrix_options.limit_refresh_rate_hz = 100;
  //matrix_options.disable_hardware_pulsing = false;
  matrix_options.rows = 32;
  matrix_options.cols = 64;
  matrix_options.chain_length = 2;
  matrix_options.parallel = 1;
  matrix_options.hardware_mapping = "adafruit-hat-pwm";

  RGBMatrix* matrix = RGBMatrix::CreateFromOptions(matrix_options, rgb_matrix::RuntimeOptions());
  if (matrix == NULL) {
    fprintf(stderr, "Failed to initialize matrix.\n");
  }
  offscreen_canvas = matrix->CreateFrameCanvas();
  return matrix;
}

void DisplayImage(RGBMatrix* matrix, const cv::Mat &image) {
  if (image.empty()) {
    fprintf(stderr, "Failed to load image.\n");
    return;
  }

  
  CopyImageToCanvas(image, offscreen_canvas);
  offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
}
