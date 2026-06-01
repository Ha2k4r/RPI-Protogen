#include "spritemath.hpp"
bool SpriteMath::Change_ = false;

void SpriteMath::SpriteUpdate(Expression& FaceSprites) {
  for (Expression::Expression_sprite& sprite : FaceSprites.Sprites) {
    switch (sprite.ExpressionType) {
    case -1: {
      if (sprite.IsUpdateTime()) {
        sprite.SpriteBuffer.setTo(cv::Scalar::all(0));
        const cv::Point* NumberOfChords[1] = {sprite.MainChords.data()};
        const int numPointsNose[] = {
            static_cast<int>(sprite.MainChords.size())};
        cv::fillPoly(sprite.SpriteBuffer, NumberOfChords, numPointsNose, 1,
                     cv::Scalar(255, 255, 255), cv::LINE_8);
        InUseSprites[sprite.UNIQUE_IDENTIFYER] =
            std::make_pair(sprite.SpriteBuffer, &sprite);
      }
      break;
    case 1:
      if (sprite.CloseTrajectory.empty()) {
        sprite.CloseTrajectory = Calculate_Many_Bezier_Curves(
            sprite.MainChords, sprite.OposingChords, blink_Cycles,
            sprite.UNIQUE_IDENTIFYER);
        sprite.OpenTrajectory = Calculate_Many_Bezier_Curves(
            sprite.OposingChords, sprite.MainChords, blink_Cycles,
            sprite.UNIQUE_IDENTIFYER);
        sprite.TrajectoryTimestamp = std::chrono::steady_clock::now();
      }
      if (sprite.ActiveAnimation) {
        if (sprite.IsUpdateTime2()) {
          if (sprite.AnimationInversion == false) {
            if (sprite.UIT < blink_Cycles - 1) {
              sprite.UIT++;
            } else {
              RawBezierTrajectory = sprite.OpenTrajectory;
              sprite.UIT = 0;
              sprite.AnimationInversion = true;
            }
          } else {
            if (sprite.UIT < blink_Cycles - 1) {
              sprite.UIT++;
            } else {
              sprite.AnimationInversion = false;
              sprite.ActiveAnimation = false;
              sprite.UIT = 0;
              continue;
            }
          }

          std::vector<cv::Point> SpriteGeomatry =
              UnpackBezierArray(sprite.UIT, RawBezierTrajectory, blink_Cycles);
          const int numPoints[] = {static_cast<int>(sprite.MainChords.size())};
          sprite.SpriteBuffer.setTo(cv::Scalar::all(0));
          const cv::Point* NumberOfChords = &SpriteGeomatry[0];
          cv::fillPoly(sprite.SpriteBuffer, &NumberOfChords, numPoints, 1,
                       cv::Scalar(255, 255, 255), cv::LINE_8);
          InUseSprites[sprite.UNIQUE_IDENTIFYER] =
              std::make_pair(sprite.SpriteBuffer, &sprite);
          Change_ = true;
        }
      } else if (sprite.IsUpdateTime() == true) {
        {
          auto now = std::chrono::steady_clock::now();
          if (std::chrono::duration_cast<std::chrono::seconds>(
                  now - sprite.TrajectoryTimestamp)
                  .count() >= 60) {
            sprite.CloseTrajectory = Calculate_Many_Bezier_Curves(
                sprite.MainChords, sprite.OposingChords, blink_Cycles,
                sprite.UNIQUE_IDENTIFYER);
            sprite.OpenTrajectory = Calculate_Many_Bezier_Curves(
                sprite.OposingChords, sprite.MainChords, blink_Cycles,
                sprite.UNIQUE_IDENTIFYER);
            sprite.TrajectoryTimestamp = now;
          }
        }
        sprite.UIT = 0;
        RawBezierTrajectory = sprite.CloseTrajectory;
        std::vector<cv::Point> SpriteGeomatry =
            UnpackBezierArray(sprite.UIT, RawBezierTrajectory, blink_Cycles);
        sprite.SpriteBuffer.setTo(cv::Scalar::all(0));
        const cv::Point* NumberOfChords = &SpriteGeomatry[0];
        const int numPoints[] = {static_cast<int>(sprite.MainChords.size())};
        cv::fillPoly(sprite.SpriteBuffer, &NumberOfChords, numPoints, 1,
                     cv::Scalar(255, 255, 255), cv::LINE_8);
        InUseSprites[sprite.UNIQUE_IDENTIFYER] =
            std::make_pair(sprite.SpriteBuffer, &sprite);
        Change_ = true;
        sprite.ActiveAnimation = true;
      }
      break;
    case 2: {
      InUseSprites[sprite.UNIQUE_IDENTIFYER] =
          std::make_pair(cv::imread(sprite.location), &sprite);
      break;
    }
    case 3:
      if (sprite.IsUpdateTime()) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&time);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%r:%P", localTime);

        sprite.SpriteBuffer.setTo(cv::Scalar::all(0));
        cv::putText(sprite.SpriteBuffer, buffer, cv::Point(1, 22),
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 1,
                    cv::Scalar(255, 255, 255), 1);
        InUseSprites[sprite.UNIQUE_IDENTIFYER] =
            std::make_pair(sprite.SpriteBuffer, &sprite);
        break;
      }
    }
    }
  }
}

void SpriteMath::SpriteColorMapUpdate(Expression& FaceSprites) {

  if (FaceSprites.ColorMap[0].IsUpdateTime() == true) {
    Change_ = true;
    // check if its a image
    if (FaceSprites.ColorMap[0].ExpressionType == 0) {
      InUseColorMap = cv::imread(FaceSprites.ColorMap[0].location);
    } // Check if its a Video (GIF)
    else if (FaceSprites.ColorMap[0].ExpressionType == 1) {
      if (FaceSprites.ColorMap[0].video.isOpened()) {
        if (FaceSprites.ColorMap[0].CurrentFrame + 1 >=
            FaceSprites.ColorMap[0].TOTAL_FRAMES) {
          FaceSprites.ColorMap[0].CurrentFrame = 0;
          FaceSprites.ColorMap[0].video.set(
              cv::CAP_PROP_POS_FRAMES, FaceSprites.ColorMap[0].CurrentFrame);
          FaceSprites.ColorMap[0].video.read(InUseColorMap);
        } else {
          FaceSprites.ColorMap[0].CurrentFrame =
              FaceSprites.ColorMap[0].CurrentFrame + 1;
          FaceSprites.ColorMap[0].video.set(
              cv::CAP_PROP_POS_FRAMES, FaceSprites.ColorMap[0].CurrentFrame);
          FaceSprites.ColorMap[0].video.read(InUseColorMap);
        }
      } else {
        FaceSprites.ColorMap[0].video =
            cv::VideoCapture(FaceSprites.ColorMap[0].location);
        // if (!(FaceSprites.ColorMap[0].video.isOpened())){
        //     std::cerr << "Video Failed to open, check your location bud.";
        //     return;
        // }
        FaceSprites.ColorMap[0].TOTAL_FRAMES = static_cast<unsigned int>(
            FaceSprites.ColorMap[0].video.get(cv::CAP_PROP_FRAME_COUNT));
        FaceSprites.ColorMap[0].CurrentFrame = 0;
        FaceSprites.ColorMap[0].video.read(InUseColorMap);
      }
    }
  } else {
  }
}

// Function to calculate a single point on a Bezier curve
int SpriteMath::Calculate_Single_Bezier_Curve(double t, int p0, int p1, int p2, int p3) {
  /*
  Cubic Bezier in Bernstiens form runs in 7 microseconds
  int value = static_cast<int>((1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3);
  return value;
  Polynomial Bezier runs in 5 microseconds
  double t2 = t * t;
  double t3 = t2 * t;

  double result = p0 + t * (-3.0 * p0 + 3.0 * p1) +
            t2 * (3.0 * p0 - 6.0 * p1 + 3.0 * p2) +
            t3 * (-p0 + 3.0 * p1 - 3.0 * p2 + p3);
  */
  // Horners Cubic Bezier runs in 5 microseconds
  double a = -p0 + 3.0 * p1 - 3.0 * p2 + p3;
  double b = 3.0 * p0 - 6.0 * p1 + 3.0 * p2;
  double c = -3.0 * p0 + 3.0 * p1;

  double result = ((a * t + b) * t + c) * t + p0;

  return static_cast<int>(std::round(result));
}

std::vector<cv::Point> SpriteMath::Calculate_Many_Bezier_Curves(
    const std::vector<cv::Point>& Array,
    const std::vector<cv::Point>& Target_Array, int num_points,
    std::string Name, double factor1, double factor2) {
  // FUTURE IDEA, Make debug mode or dev mode to switch between advanced error
  // reporting and none for speed purpases. I am absolutly open to any
  // implementation using pointers
  size_t total_points = Array.size() * num_points;
  std::vector<cv::Point> RawBezierArray;
  RawBezierArray.resize(total_points);
  if (Target_Array.size() != Array.size()) {
    std::cerr
        << "[SpriteMath] Error: Target_Array and Array must be the same size."
        << std::endl
        << "Offender is : " << Name << std::endl;
    return {}; // return empty result
  }
  int index = 0;

  for (size_t i = 0; i < Array.size(); ++i) {
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

std::vector<cv::Point>
SpriteMath::UnpackBezierArray(int Index, std::vector<cv::Point> RawBezierArray,
                              int Number_Of_Points_In_Polygon) {
  int maxindex = RawBezierArray.size() / Number_Of_Points_In_Polygon;

  // Allocate memory for the vertices
  std::vector<cv::Point> Polygon_Verticies(maxindex);

  // Copy the relevant points to Geometric_Vertices
  for (int i = 0; i < maxindex; i++) {
    int target = (Number_Of_Points_In_Polygon * i) + Index;
    Polygon_Verticies[i] = RawBezierArray[target];
    // std::cout << "Geometric Verticies   : " << Geometric_Vertices[i] <<
    // std::endl;
  }
  return Polygon_Verticies;
}
