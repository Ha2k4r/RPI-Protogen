#include "globals.hpp"
Expression::Expression_sprite::Expression_sprite(
    const Expression_Params& Params
)
    : MainChords(Params.MainChords),
      OposingChords(Params.OposingChords),
      UNIQUE_IDENTIFYER(Params.UNIQUE_IDENTIFYER),
      wait_time(Params.wait_time),
      wait_time2(Params.wait_time2),

      //THESE SHOULD NEVER CHANGE
      action_time(std::chrono::steady_clock::time_point{}),
      elapsed_time(std::chrono::duration<double>(0)),
      action_time2(std::chrono::steady_clock::time_point{}),
      elapsed_time2(std::chrono::duration<double>(0)),
      //Type of sprite specifyers   
      ExpressionType(Params.ExpressionType)   //const std::string& video_path
{}
Expression::Colormap::Colormap(const Expression_Params& Params) : 
  video(Params.MediaPath), 
  ExpressionType(Params.ExpressionType),
  wait_time(Params.wait_time),

    //THESE SHOULD NEVER CHANGE
  action_time(std::chrono::steady_clock::time_point{}),
  elapsed_time(std::chrono::duration<double>(0)) 
  {
      if (!video.isOpened()) {
        std::cerr << "Error: Could not open the video file." << std::endl;
      } else {
        TOTAL_FRAMES = static_cast<unsigned int>(video.get(cv::CAP_PROP_FRAME_COUNT));
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

