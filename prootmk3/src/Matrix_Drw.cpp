#include "led-matrix.h"
#include "graphics.h"
#include "spritemath.hpp"
#ifdef __ARM_NEON
#include <arm_neon.h>
#endif
#include <opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "Config.hpp"
using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;

FrameCanvas* offscreen_canvas = nullptr;

void CopyImageToCanvas(const cv::Mat &image, Canvas *canvas) {
  rgb_matrix::SetImage(canvas, 0, 0, image.data,
                       image.total() * image.elemSize(),
                       image.cols, image.rows, true);
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
  runtime_opt.drop_privileges = false; 
  RGBMatrix* matrix = RGBMatrix::CreateFromOptions(options, runtime_opt);
  if (matrix == NULL) {
    fprintf(stderr, "Failed to initialize matrix.\n");
  }
  offscreen_canvas = matrix->CreateFrameCanvas();
  return matrix;
}

#ifdef __ARM_NEON
static void ApplyColormap(const cv::Mat& mask, const cv::Mat& colormap, cv::Mat& out) {
  int total = static_cast<int>(mask.total());
  const uint8_t* m = mask.data;
  const uint8_t* c = colormap.data;
  uint8_t* o = out.data;
  int i = 0;
  for (; i + 8 <= total; i += 8) {
    uint8x8_t vm = vld1_u8(m + i);
    uint8x8x3_t vc = vld3_u8(c + i * 3);
    vc.val[0] = vand_u8(vc.val[0], vm);
    vc.val[1] = vand_u8(vc.val[1], vm);
    vc.val[2] = vand_u8(vc.val[2], vm);
    vst3_u8(o + i * 3, vc);
  }
  for (; i < total; i++) {
    uint8_t mv = m[i];
    o[i*3]   = mv & c[i*3];
    o[i*3+1] = mv & c[i*3+1];
    o[i*3+2] = mv & c[i*3+2];
  }
}
#else
static void ApplyColormap(const cv::Mat& mask, const cv::Mat& colormap, cv::Mat& out) {
  cv::Mat tmp;
  cv::cvtColor(mask, tmp, cv::COLOR_GRAY2BGR);
  cv::bitwise_and(tmp, colormap, out);
}
#endif

#ifndef TESTING_ENVIRONMENT
void DisplayImage(SpriteMath& spritemath, RGBMatrix* matrix) {
  cv::Mat Right_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
  cv::Mat Left_Frame  = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
  cv::Mat FULLSCREEN  = cv::Mat::zeros(cv::Size(128, 32), CV_8UC3);

    // Clear the right frame before each new draw pass
    Right_Frame.setTo(0);
    Left_Frame.setTo(0);
    FULLSCREEN.setTo(0);

    // Shared canvas to hold one sprite per iteration
  for (const auto& [key, spritePair] : spritemath.InUseSprites) {
    cv::Mat SpriteCanvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
    const cv::Mat& spriteImage = spritePair.first;
    Expression::Expression_sprite* expr = spritePair.second;

    if (!expr) continue;

    if (expr->is_Preloaded_Image) {
      if (spriteImage.channels() == 1)
        cv::cvtColor(spriteImage, SpriteCanvas, cv::COLOR_GRAY2BGR);
      else
        spriteImage.copyTo(SpriteCanvas);
    } else {
      if (spriteImage.size() != spritemath.InUseColorMap.size()) {
        std::cerr << "Sprite size mismatch — skipping: " << key << std::endl <<
          spriteImage.size() << " != " << spritemath.InUseColorMap.size() << std::endl;
        continue;
      }

      ApplyColormap(spriteImage, spritemath.InUseColorMap, SpriteCanvas);
    }

    cv::add(Right_Frame, SpriteCanvas, Right_Frame);

    if (expr->Flip) {
      cv::flip(SpriteCanvas, SpriteCanvas, 1);
    }

    cv::add(Left_Frame, SpriteCanvas, Left_Frame);
  }

  cv::hconcat(Right_Frame, Left_Frame, FULLSCREEN);

  CopyImageToCanvas(FULLSCREEN, offscreen_canvas);
  offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);
  offscreen_canvas -> Clear();
}
#else
void DisplayImage(SpriteMath& spritemath) {
  cv::Mat Right_Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
  cv::Mat Left_Frame  = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
  cv::Mat FULLSCREEN  = cv::Mat::zeros(cv::Size(128, 32), CV_8UC3);

    // Clear the right frame before each new draw pass
    Right_Frame.setTo(0);
    Left_Frame.setTo(0);
    FULLSCREEN.setTo(0);

    // Shared canvas to hold one sprite per iteration
  for (const auto& [key, spritePair] : spritemath.InUseSprites) {
    cv::Mat SpriteCanvas = cv::Mat::zeros(cv::Size(64, 32), CV_8UC3);
    const cv::Mat& spriteImage = spritePair.first;
    Expression::Expression_sprite* expr = spritePair.second;

    if (!expr) continue;

    if (expr->is_Preloaded_Image) {
      if (spriteImage.channels() == 1)
        cv::cvtColor(spriteImage, SpriteCanvas, cv::COLOR_GRAY2BGR);
      else
        spriteImage.copyTo(SpriteCanvas);
    } else {
      if (spriteImage.size() != spritemath.InUseColorMap.size()) {
        std::cerr << "Sprite size mismatch — skipping: " << key << std::endl <<
          spriteImage.size() << " != " << spritemath.InUseColorMap.size() << std::endl;
        continue;
      }

      ApplyColormap(spriteImage, spritemath.InUseColorMap, SpriteCanvas);
    }

    cv::add(Right_Frame, SpriteCanvas, Right_Frame);

    if (expr->Flip) {
      cv::flip(SpriteCanvas, SpriteCanvas, 1);
    }

    cv::add(Left_Frame, SpriteCanvas, Left_Frame);
  }

  cv::hconcat(Right_Frame, Left_Frame, FULLSCREEN);

  cv::Mat Upscaled;
  cv::resize(FULLSCREEN, Upscaled, cv::Size(FULLSCREEN.cols * 10, FULLSCREEN.rows * 10), 0, 0, cv::INTER_LINEAR);
  cv::imshow("Protogen Output", Upscaled);
  cv::waitKey(1);
}

#endif

