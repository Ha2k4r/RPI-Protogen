#ifndef SPRITE_MATH_H
#define SPRITE_MATH_H

#include <map>
#include <string>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include "expression-vectors.hpp"
#include "globals.hpp"
#include <math.h>
#include <bits/stdc++.h>
class SpriteMath {
public:
    //checks the sprites to see if they need to be updated and if so, the geomatrys of the 2d sprites get updated
    void SpriteUpdate(Expression& FaceSprites);
    //Checks to see if the colormap backround needs to be updated changed or otherwise incremented 
    void SpriteColorMapUpdate(Expression& FaceSprites);
    
    std::map<std::string, std::pair< cv::Mat, Expression::Expression_sprite* >> InUseSprites;
    
    cv::Mat InUseColorMap = cv::Mat::zeros(cv::Size(64,32), CV_8UC1);

    //Set to true if anything changes that the display needs to update 
    static bool Change_;
private:
    //Performs a singuler Bezier curve calculation
    int Calculate_Single_Bezier_Curve(double t, int p0, int p1, int p2, int p3);
    //Returns a RAW and importantly unpacked bezier path from a 2d polygon to another of the same number of verticies 
    //Notably factors can be used to increse or decrese the amplitude of the Path
    std::vector<cv::Point> Calculate_Many_Bezier_Curves(const std::vector<cv::Point>& Array, const std::vector<cv::Point>& Target_Array, int num_points, double factor1 = 0.5 , double factor2 = 1.8 );
    //Unpacks the RAW output from the ABOVE function -- and importantly returns a valid 2d polygon at (Index) location on the path 
    std::vector<cv::Point> UnpackBezierArray(int Index, std::vector<cv::Point> RawBezierArray, int Number_Of_Points_In_Polygon);

    //The fucked up trajectory from array to target polgons as performed in Calculate_Many_Bezier_Curves
    std::vector<cv::Point> RawBezierTrajectory;
};

#endif
