#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <chrono>
#include <opencv2/opencv.hpp>
#include "expression-vectors.hpp"

struct timeValues {
  std::chrono::time_point<std::chrono::steady_clock> action_time;
  double wait_time;
  std::chrono::duration<double> elapsed_time; 
};

struct EyeArray {
    const std::vector<cv::Point>& DEFAULT;
    const std::vector<cv::Point>& CLOSED;
    int arraysize = DEFAULT.size();
    EyeArray(const std::vector<cv::Point>& def, const std::vector<cv::Point>& closed) : DEFAULT(def), CLOSED(closed), arraysize(def.size()) {}
};

struct NoseArray {
    const std::vector<cv::Point>& DEFAULT;
    NoseArray(const std::vector<cv::Point>& def) : DEFAULT(def) {}
};

struct MouthArray {
    const std::vector<cv::Point>& DEFAULT;
    MouthArray(const std::vector<cv::Point>& def) : DEFAULT(def) {}
};

struct color_map {
  cv::VideoCapture video;
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

extern MouthArray morm;
extern NoseArray norm;
extern EyeArray Happy;

extern timeValues bakrndUpdate;
extern timeValues timeBetweenBlink;
extern timeValues blinkSpeed;
extern timeValues noseUpdate;
extern timeValues mouthUpdate;

extern color_map BluePinkLR;

extern bool Blinking;
extern bool Change;
extern bool eyeOpening; 
extern int n;
extern int blink_Cycles;

extern const int numPointsEye[];
extern const int numPointsMouth[];
extern const int numPointsNose[];

extern cv::Mat EyeSprite;
extern cv::Mat NoseSprite;
extern cv::Mat MouthSprite;

#endif
