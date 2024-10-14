#include "config.hpp"

timeValues bakrndUpdate = {std::chrono::steady_clock::time_point{}, 0.042, std::chrono::duration<double>(0)};
timeValues timeBetweenBlink = {std::chrono::steady_clock::time_point{}, 10, std::chrono::duration<double>(0)};
timeValues blinkSpeed = {std::chrono::steady_clock::time_point{}, 0.01, std::chrono::duration<double>(0)};
timeValues noseUpdate = {std::chrono::steady_clock::time_point{}, 100, std::chrono::duration<double>(0)};
timeValues mouthUpdate = {std::chrono::steady_clock::time_point{}, 100, std::chrono::duration<double>(0)};

MouthArray morm(MouthHappyArray);
NoseArray norm(NoseHappyArray);
EyeArray Happy(EyeHappyArray, EyeClosedHappyArray);

color_map BluePinkLR("color_maps/WHOH1.gif");

bool Blinking = false;
bool Change = false;
bool eyeOpening = true;
int arraySize = 9;
int n = 0;
int blink_Cycles = 20;

const int numPointsEye[] = { 9 };
const int numPointsMouth[] = { 14 };
const int numPointsNose[] = { 10 };

cv::Mat EyeSprite = {cv::Mat::zeros(cv::Size(64, 32), CV_8UC1)}; // TODO: yayyy i love magic numbers
cv::Mat NoseSprite = {cv::Mat::zeros(cv::Size(64, 32), CV_8UC1)};
cv::Mat MouthSprite = {cv::Mat::zeros(cv::Size(64, 32), CV_8UC1)};