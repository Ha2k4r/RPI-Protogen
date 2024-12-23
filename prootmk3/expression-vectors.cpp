#include <opencv2/opencv.hpp>

//std::vector EyeHappyArray 

std::vector<cv::Point> EyeHappyArray = {
    cv::Point(0, 4), // far Left
    cv::Point(6, 5),
    cv::Point(11, 5),
    cv::Point(20, 7),
    cv::Point(24, 6),
    cv::Point(24, 4),
    cv::Point(18, 1),
    cv::Point(15, 0),
    cv::Point(12, 0)  // top left on top of the angle
};

std::vector<cv::Point> EyeClosedHappyArray = {
    cv::Point(0,  1),  // far Left
    cv::Point(6,  1),
    cv::Point(11, 1),
    cv::Point(20, 1),
    cv::Point(24, 1),
    cv::Point(24, 1),
    cv::Point(18, 1),
    cv::Point(15, 0),
    cv::Point(12, 0)  // top left on top of the angle
};
std::vector<cv::Point> MouthHappyArray = {
    cv::Point(14, 19),
    cv::Point(32, 29),
    cv::Point(45, 23),
    cv::Point(53, 23),
    cv::Point(61, 26),
    cv::Point(59, 27),
    cv::Point(52, 25),
    cv::Point(46, 25),
    cv::Point(34, 31),
    cv::Point(29, 31),
    cv::Point(19, 25),
    cv::Point(11, 25),
    cv::Point(7, 21),
    cv::Point(10, 20)

};

std::vector<cv::Point> NoseHappyArray = {
    // counterclockwise
    cv::Point(63, 10),  // bottom of nose
    cv::Point(63, 6),
    cv::Point(63, 4),
    cv::Point(61, 2),
    cv::Point(56, 1),
    cv::Point(56, 1),
    cv::Point(53, 2),  // far left of nose end point
    cv::Point(57, 2),
    cv::Point(61, 5),
    cv::Point(63, 10),

};
//unused as of here and down but will be used eventually

std::vector<cv::Point> BlushEye{
    cv::Point(0, 5),  // far Left
    cv::Point(3, 8),
    cv::Point(13, 5),
    cv::Point(18, 5),
    cv::Point(23, 8),
    cv::Point(24, 8),
    cv::Point(23, 3),
    cv::Point(19, 1),
    cv::Point(13, 1),
    cv::Point(8, 4)  // top left on top of the angle
};

std::vector<cv::Point> BlushStripe1 = {
    cv::Point(3, 10),  // far Left
    cv::Point(7, 16)
};

std::vector<cv::Point> BlushStripe2 = {
    cv::Point(6, 9),  // far Left
    cv::Point(10, 15)
};

std::vector<cv::Point> BlushStripe3 = {
    cv::Point(11, 10),  // far Left
    cv::Point(12, 14)
};
