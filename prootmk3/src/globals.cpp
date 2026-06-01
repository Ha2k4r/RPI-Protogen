#include "globals.hpp"
Expression::Expression_sprite::Expression_sprite(const Expression_Params& Params)

    //THESE SHOULD NEVER CHANGE
    : action_time(std::chrono::steady_clock::time_point{}),
      wait_time(Params.wait_time),
      elapsed_time(std::chrono::duration<double>(0)),
      action_time2(std::chrono::steady_clock::time_point{}),
      wait_time2(Params.wait_time2),
      elapsed_time2(std::chrono::duration<double>(0)),

      UNIQUE_IDENTIFYER(Params.UNIQUE_IDENTIFYER),
      MainChords(Params.MainChords),
      OposingChords(Params.OposingChords),
      Flip(Params.Flip),
      //Type of sprite specifyers
      ExpressionType(Params.ExpressionType),   //const std::string& video_path
      location(Params.MediaPath),
      TrajectoryTimestamp(std::chrono::steady_clock::time_point{})
{ 
  if (location.empty()){
    is_Preloaded_Image = false;
  }else{
    is_Preloaded_Image = true;
  }    
}
Expression::Colormap::Colormap(const Expression_Params& Params) :
  action_time(std::chrono::steady_clock::time_point{}),
  wait_time(Params.wait_time),
  elapsed_time(std::chrono::duration<double>(0)),
  ExpressionType(Params.ExpressionType),
  location(Params.MediaPath) 
  {
      if (ExpressionType == -1) {
        std::cerr << "BUCKLE UP FUCKER, you put in no expression type for a sprite." << std::endl << 
        "Valid options are : 0 for Image or 1 for Gif  DO: remember to resize your image or gif to the size of one of the panels" << std::endl;
        std::cerr << "Error: Could not open the video file." << std::endl;
      } else {
        //TOTAL_FRAMES = static_cast<unsigned int>(video.get(cv::CAP_PROP_FRAME_COUNT));
      }
    }


bool Expression::Expression_sprite::IsUpdateTime() {
  auto now = std::chrono::steady_clock::now();
  elapsed_time = now - action_time;
  if (elapsed_time.count() >= wait_time) {
    action_time = now;  // Reset action_time
    return true;
    } 
    else {
      return false;
  }
}
bool Expression::Expression_sprite::IsUpdateTime2() {
  auto now = std::chrono::steady_clock::now();
  elapsed_time2 = now - action_time2;
  if (elapsed_time2.count() >= wait_time2) {
    //std::cout << "IsUpdateTime! " << UNIQUE_IDENTIFYER << std::endl; //This can be used 
    action_time2 = now;  // Reset action_time
    return true;
    } 
  else {
    return false;
  }
}
bool Expression::Colormap::IsUpdateTime() {
  auto now = std::chrono::steady_clock::now();
  elapsed_time = now - action_time;
  if (elapsed_time.count() >= wait_time) {
    action_time = now;  // Reset action_time
    return true;
    } 
  else {
    return false;
  }
}

bool Blinking = false;
bool Change = false;
bool eyeOpening = true;
int blink_Cycles = 20;

std::vector<std::string> Ports = {"TODO:COME BACK HERE AND ADD PORTS"};

