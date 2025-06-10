#include "led-matrix.h"
#include "spritemath.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "Config.hpp"
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

  //options.pwm_lsb_nanoseconds = 50;
  //options.show_refresh_rate = true;
  //options.limit_refresh_rate_hz = 300;
  //options.disable_hardware_pulsing=false;
  options.rows = 32;
  options.cols = 64;
  options.chain_length = 2;
  options.hardware_mapping = "adafruit-hat-pwm";
  runtime_opt.gpio_slowdown = 4;
  //runtime_opt.drop_privileges = false; 
  RGBMatrix* matrix = RGBMatrix::CreateFromOptions(options, runtime_opt);
  if (matrix == NULL) {
    fprintf(stderr, "Failed to initialize matrix.\n");
  }
  offscreen_canvas = matrix->CreateFrameCanvas();
  return matrix;
}

#ifndef TESTING_ENVIRONMENT
/*void DisplayImage(SpriteMath& spritemath, RGBMatrix* matrix) {

  static cv::Mat SpriteCanvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
  static cv::Mat Left_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
  static cv::Mat RightFrame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
  static cv::Mat FULLSCREEN = cv::Mat::zeros(cv::Size(128, 32), CV_8UC1);
  for (auto const& [key, val] : spritemath.InUseSprites){
    cv::add(val, SpriteCanvas, SpriteCanvas);
  }
  //Mask the Image over the Colormaps
  cv::bitwise_and(spritemath.InUseColorMap, spritemath.InUseColorMap, RightFrame, SpriteCanvas);
  //make the inverted other frame NOT GOOD FOR TEXT
  cv::flip(RightFrame, Left_Frame, 1 ); 
  cv::hconcat(RightFrame, Left_Frame, FULLSCREEN);

  CopyImageToCanvas(FULLSCREEN, offscreen_canvas);
  offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
  offscreen_canvas -> Clear();
}*/
#else
void DisplayImage(SpriteMath& spritemath) {
    // Right half of the final frame
    static cv::Mat Right_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
    // Mirrored left half
    static cv::Mat Left_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
    // Combined fullscreen display (Right + Left)
    static cv::Mat FULLSCREEN = cv::Mat::zeros(cv::Size(128, 32), CV_8UC3);

    // Clear the right frame before each new draw pass
    Right_Frame.setTo(0);

    // Shared canvas to hold one sprite per iteration
    cv::Mat SpriteCanvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);

    for (const auto& [key, spritePair] : spritemath.InUseSprites) {
        const cv::Mat& spriteImage = spritePair.first;
        Expression::Expression_sprite* expr = spritePair.second;

        #ifdef NOT_PRODUCTION_ENVIRNMENT
        if (!expr) {
            std::cerr << "Missing expression for key: " << key << std::endl;
            continue;
        }
        #endif

        cv::Mat spriteColor;

        if (expr->is_Preloaded_Image) {
            // If sprite is preloaded, use it directly like a person whos afraid of math. HISS 
            if (spriteImage.channels() == 1)
                cv::cvtColor(spriteImage, SpriteCanvas, cv::COLOR_GRAY2BGR);
            else
                spriteImage.copyTo(SpriteCanvas);
        } else {
            // Convert sprite to 3-channel color if needed
            if (spriteImage.channels() == 1)
                cv::cvtColor(spriteImage, spriteColor, cv::COLOR_GRAY2BGR);
            else
                spriteColor = spriteImage;

            #ifdef NOT_PRODUCTION_ENVIRNMENT
            if (spriteColor.size() != spritemath.InUseColorMap.size() ||
                spriteColor.type() != spritemath.InUseColorMap.type()) {
                std::cerr << "Sprite mismatch (type or size) — skipping: " << key << "\n";
                continue;
            }
            #endif  

            // Mask the sprite using the color map
            cv::bitwise_and(spriteColor, spritemath.InUseColorMap, SpriteCanvas);
        }

        // Composite onto the main right frame
        cv::add(Right_Frame, SpriteCanvas, Right_Frame);
    }

    // Flip right frame to create left frame
    cv::flip(Right_Frame, Left_Frame, 1);

    // Combine left + right for fullscreen
    cv::hconcat(Right_Frame, Left_Frame, FULLSCREEN);

    // Upscale for viewing
    cv::resize(FULLSCREEN, FULLSCREEN,
               cv::Size(FULLSCREEN.cols * 10, FULLSCREEN.rows * 10),
               0, 0, cv::INTER_LINEAR);

    // Display
    cv::imshow("Protogen Output", FULLSCREEN);
    cv::waitKey(1);
}

#endif

