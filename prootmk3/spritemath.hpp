#ifndef SPRITE_MATH_H
#define SPRITE_MATH_H

#include <map>
#include <string>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include "PathCalculator.hpp"
#include "expression-vectors.hpp"
#include "globals.hpp"
class SpriteMath {
public:
    void SpriteUpdate(Expression& FaceSprites);
    void SpriteColorMapUpdate(Expression& FaceSprites);
    std::map<std::string, cv::Mat> InUseSprites;
    cv::Mat InUseColorMap = cv::Mat::zeros(cv::Size(64,32), CV_8UC1);
private:
    std::vector<cv::Point> RawBezierTrajectory;
};

#endif