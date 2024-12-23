#include "globals.hpp"

void init(std::vector<Expression>& faces){
  //happy face
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Eye", .wait_time=10, .MainChords=EyeHappyArray, .OposingChords=EyeClosedHappyArray , .ExpressionType=1, .wait_time2=0.01});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .wait_time=10, .MainChords=NoseHappyArray});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .wait_time=10, .MainChords=MouthHappyArray});
  faces[0].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .MediaPath="color_maps/WHOH1.gif"});
    //blush face
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .wait_time=10, .MainChords=NoseHappyArray});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .wait_time=10, .MainChords=MouthHappyArray});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushEye", .wait_time=10, .MainChords=BlushEye});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe1", .wait_time=10, .MainChords=BlushStripe1});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe2", .wait_time=10, .MainChords=BlushStripe2});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe3", .wait_time=10, .MainChords=BlushStripe3});
  faces[1].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .ExpressionType=9, .MediaPath="/home/fur3/RPI-Protogen/prootmk3/color_maps/WHOH1.gif"});
  //Clock
  faces[2].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Clock", .wait_time=0.5, .ExpressionType=3});
  faces[2].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .MediaPath="color_maps/WHOH1.gif"});
}