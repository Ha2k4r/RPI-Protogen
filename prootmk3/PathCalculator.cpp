#include "PathCalculator.hpp"
#include <opencv2/opencv.hpp>

// Function to calculate a single point on a Bezier curve
int Calculate_Single_Bezier_Curve(double t, int p0, int p1, int p2, int p3) {
    int value = static_cast<int>(round((1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3));
    return value;
}


std::vector<cv::Point> Calculate_Many_Bezier_Curves(const std::vector<cv::Point>& Array, const std::vector<cv::Point>& Target_Array, int num_points, double factor1, double factor2) {
    //FUTURE IDEA, Make debug mode or dev mode to switch between advanced error reporting and none for speed purpases.
    //I am absolutly open to any implementation using pointers 
    size_t total_points = Array.size() * num_points;
    std::vector<cv::Point> RawBezierArray;
    RawBezierArray.resize(total_points);

    int index = 0;

    for (int i = 0; i < Array.size(); ++i) {
        int x1 = Array[i].x, y1 = Array[i].y;
        int x2 = Target_Array[i].x, y2 = Target_Array[i].y;

        double xP1 = x1 + (double)(x2 - x1) * factor1;
        double xP2 = x1 + (double)(x2 - x1) * factor2;
        double yP1 = y1 + (double)(y2 - y1) * factor1;
        double yP2 = y1 + (double)(y2 - y1) * factor2;

        for (int j = 0; j < num_points; ++j) {
            double t = static_cast<double>(j) / (num_points - 1);
            int current_point_x = Calculate_Single_Bezier_Curve(t, x1, xP1, xP2, x2);
            int current_point_y = Calculate_Single_Bezier_Curve(t, y1, yP1, yP2, y2);
            RawBezierArray[index] = cv::Point(current_point_x, current_point_y);
            index++;
        }
    }

    return RawBezierArray;
}


std::vector<cv::Point> UnpackBezierArray(int Index, std::vector<cv::Point> RawBezierArray, int numpoints) {
    int maxindex = RawBezierArray.size()/numpoints;
    
    // Allocate memory for the vertices
    std::vector<cv::Point> Geometric_Vertices(maxindex);
    
    // Copy the relevant points to Geometric_Vertices
    for (int i = 0; i < maxindex; i++) {
        int target = (numpoints * i) + Index;
        Geometric_Vertices[i] = RawBezierArray[target];
    }
    return Geometric_Vertices;
}

    return Geometric_Vertices;
}
