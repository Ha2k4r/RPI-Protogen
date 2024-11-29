#include "globals.hpp"

void init(std::vector<Expression>& faces){
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Eye", .wait_time=10, .MainChords=EyeHappyArray, .OposingChords=EyeClosedHappyArray , .NeedBezier=true, .wait_time2=0.01});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .wait_time=10, .MainChords=NoseHappyArray});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .wait_time=10, .MainChords=MouthHappyArray});
  faces[0].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .isGif=true, .MediaPath="color_maps/WHOH1.gif"});
}