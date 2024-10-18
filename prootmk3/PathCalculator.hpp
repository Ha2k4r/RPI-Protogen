#ifndef SPRITE_MATH_H
#define SPRITE_MATH_H

#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include "PathCalculator.hpp"
#include "globals.hpp"
class SpriteMath {
public:
    void EyeUpdate();

    void MouthUpdate();

    void NoseUpdate();

protected:
    std::vector<cv::Point> EyeVerticies;
    std::vector<cv::Point> RawBezierEye;
};

#endif
