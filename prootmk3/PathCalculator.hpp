#ifndef PATHCALCULATOR_H
#define PATHCALCULATOR_H

#include <math.h>
#include <opencv2/opencv.hpp>

int Calculate_Single_Bezier_Curve(double t, int p0, int p1, int p2, int p3);
                        
std::vector<cv::Point> Calculate_Many_Bezier_Curves(const std::vector<cv::Point>& Array, const std::vector<cv::Point>& Target_Array, int num_points, double factor1 = 0.5 , double factor2 = 1.8 );

std::vector<cv::Point> UnpackBezierArray(int Index, std::vector<cv::Point> RawBezierArray, int numpoints);

#endif