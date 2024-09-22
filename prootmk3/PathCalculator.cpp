#include <opencv2/opencv.hpp>

// Function to calculate a single point on a Bezier curve
int bezier_point(double t, int p0, int p1, int p2, int p3) {
    return static_cast<int>(round((1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3));
}


cv::Point* BezierCurveCalculation(const cv::Point* Array,const cv::Point* Target_Array, int num_points,int arraysize, double factor1 = 0.5 , double factor2 = 1.8 ){
    //std::cout << "ello there" << std::endl;
    cv::Point* RawBezierArray = new cv::Point[(arraysize * num_points)+15];
    int index = 0;

     // Iterate over the points in both arrays
    for (int i = 0; i < arraysize; ++i) {
        int x1 = Array[i].x, y1 = Array[i].y;
        int x2 = Target_Array[i].x, y2 = Target_Array[i].y;

        // Control points for the Bezier curve
        double xP1 = x1 + (double)(x2 - x1) * factor1;
        double xP2 = x1 + (double)(x2 - x1) * factor2;
        double yP1 = y1 + (double)(y2 - y1) * factor1;
        double yP2 = y1 + (double)(y2 - y1) * factor2;

        // Generate points on the Bezier curve
        for (int j = 0; j < num_points; ++j) {
            double t = static_cast<double>(j) / (num_points - 1);  // Normalize t to range [0, 1]
            int current_point_x = bezier_point(t, x1, xP1, xP2, x2);
            int current_point_y = bezier_point(t, y1, yP1, yP2, y2);
            RawBezierArray[index] = cv::Point(current_point_x, current_point_y);
            //std::cout << index <<"  -   " << RawBezierArray[index] << std::endl;
            index++;
        }

    }
    /* for (int i = 0 ; i < arraysize*num_points; i++){
        std::cout << RawBezierArray[i] <<"  -   " << i << std::endl;
    } */

    return RawBezierArray;
}

const cv::Point* UnpackBezierArray(int Index, cv::Point* RawBezierArray, int numpoints, int maxindex) {
    // Ensure maxindex is correctly calculated
    maxindex = maxindex / numpoints;
    
    // Allocate memory for the vertices
    cv::Point* Geometric_Vertices = new cv::Point[maxindex];
    
    // Copy the relevant points to Geometric_Vertices
    for (int i = 0; i < maxindex; i++) {
        int target = (numpoints * i) + Index;
        if (target >= numpoints * maxindex || target < 0) {
            std::cerr << "PathCalculator TARGET out of bounds: target:" << target << "   max: " << numpoints * maxindex << std::endl;
            // Deallocate memory before returning
            delete[] Geometric_Vertices;
            return nullptr;
        }
        Geometric_Vertices[i] = RawBezierArray[target];
    }
    return Geometric_Vertices;
}
