#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <stdlib.h>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "expression-vectors.hpp"
#include <variant>
struct Expression_Params {
  //Universial required items. 
  const std::string& UNIQUE_IDENTIFYER;
  //This is in seconds
  const double wait_time = 10;
  //sprite specific items
  const std::vector<cv::Point>& MainChords = {};
  const std::vector<cv::Point>& OposingChords = {};
  
  const int ExpressionType = -1;
  const double wait_time2 = 253;
  //ColorMap Specific items
  const std::string& MediaPath = "";
  const bool Flip = true;
  
};
//Class to encapsulate different facial expressions and the data involved to perform calculations based on them
class Expression {
  //first time making nested anything so please be nice i know it is shit but its better than the spagetti i had before at least somewhat
  public:
    struct Expression_sprite {
      private:
          //time keeping wizardry 
          std::chrono::time_point<std::chrono::steady_clock> action_time;
          const double wait_time;
          std::chrono::duration<double> elapsed_time;
          //durring an animation the below values are used, otherwise they will not be used 
          std::chrono::time_point<std::chrono::steady_clock> action_time2;
          const double wait_time2;
          std::chrono::duration<double> elapsed_time2;
      public:

          //REQUIRED, otherwise it will conflict with others in the final Map (in the future colormaps will make use of this as well)
          const std::string UNIQUE_IDENTIFYER;
          //Geometric sprites will have cordinates and thus be rendered using fillpoly()
          const std::vector<cv::Point>& MainChords;
          //Special geometric sprites that use "NeedBezier" will need a target point for the bezier curve to go to
          const std::vector<cv::Point>& OposingChords;
          //Flip the image to the left screen or preserve the original primarily usefull for text
          const bool Flip;
          //will use a bezier curve calculation to go to a setpoint, therby needing oposing chords as the setpoint
          //usefull for eye blinking, animations or cool transitions 
          //usually as transitions happen the updates should be faster so it switches to waitime2
          const int ExpressionType;
          bool IsUpdateTime();
          bool IsUpdateTime2();
          
          //Only used in image sprites which goes against math based the math based aproach of this program but some things should not be vectorized and do not need math operations performed on them
          std::string location;
          bool is_Preloaded_Image;

          //
          // All Below are only used by Functions for blinking or otherwise usage in future functions
          //

          //switch to timer2 and use those values instead
          bool ActiveAnimation = false;
          //implementation for eyeball going to the setpoint of closed
          //then open, false means closing
          bool AnimationInversion = false;
          //unique iterator 
          int UIT = 0;
          // Constructor
          Expression_sprite(const Expression_Params& Params);
      };


      struct Colormap {
        private:
          std::chrono::time_point<std::chrono::steady_clock> action_time;
          const double wait_time;
          std::chrono::duration<double> elapsed_time;
        public: 
          //required
          const int ExpressionType; //If -1 throw error, if 0 its a image if 1 its a gif or other video i havent tested
          std::string location;
          cv::Mat Frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
          
          //Video Only
          cv::VideoCapture video;

          unsigned int CurrentFrame = 0;
          unsigned int TOTAL_FRAMES;
          
        bool IsUpdateTime();
        //initializer
        Colormap(const Expression_Params& Params);
    };
    public:
    std::vector<Colormap> ColorMap;
    std::vector<Expression_sprite> Sprites;
    
};

//Untill i can find a better way of handling this dumb process it stays. :(
extern std::vector<std::string> Ports;

//as time goes on i will be slowly removing many aspects of this file for refactoring purpases

extern int blink_Cycles;

extern cv::Mat bakrnd_frame;

#endif
