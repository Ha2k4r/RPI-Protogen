#ifndef EXPR_V_H
#define EXPR_V_H
#include <opencv2/opencv.hpp>

//These are the hardcoded Verticies for your various facial expressions and its hell to make new ones so good luck future entropenuers ! 

//NOTE TO ANYONE LOOKING TO MAKE ONE, be nice to yourself and dont bite off more than you can chew, its quite tedious to make one so dont go making 50 in a day
//make it in python then port it over for faster creation and try to stay at the same number of verticies of other sprites of its type
//so in the future, expression changes can have animations between them. doing socan be FAR prettyer, faster, and overall easyer to implement in the future (on all coders behalf)

extern std::vector<cv::Point> EyeHappyArray;
extern std::vector<cv::Point> EyeClosedHappyArray;
extern std::vector<cv::Point> MouthHappyArray;
extern std::vector<cv::Point> NoseHappyArray;
extern std::vector<cv::Point> BlushEye;
extern std::vector<cv::Point> BlushStripe1;
extern std::vector<cv::Point> BlushStripe2;
extern std::vector<cv::Point> BlushStripe3;
#endif
