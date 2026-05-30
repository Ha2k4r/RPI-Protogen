#include "globals.hpp"
#include "Config.hpp"

#include <cstdlib>
#include <string>

// Resolve a colormap file path at runtime.  Priority:
//   1. $COLOR_MAPS_DIR env var
//   2. color_maps/<filename> relative to cwd (repo clone expected)
static std::string colormap_path(const std::string& filename) {
  const char* env = std::getenv("COLOR_MAPS_DIR");
  if (env && env[0] != '\0')
    return std::string(env) + "/" + filename;
  return "color_maps/" + filename;
}

void init(std::vector<Expression>& faces){
  //happy face
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Eye", .MainChords=EyeHappyArray, .OposingChords=EyeClosedHappyArray , .ExpressionType=1, .wait_time2=0.01});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .MainChords=NoseHappyArray});
  faces[0].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .MainChords=MouthHappyArray});
  faces[0].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .ExpressionType=1,.MediaPath=colormap_path("WHOH1.gif")});
  //blush face
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Nose", .MainChords=NoseHappyArray});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="Mouth", .MainChords=MouthHappyArray});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushEye", .MainChords=BlushEye});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe1", .MainChords=BlushStripe1});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe2", .MainChords=BlushStripe2});
  faces[1].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BlushStripe3", .MainChords=BlushStripe3}); 
  faces[1].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .ExpressionType=0, .MediaPath=colormap_path("Traaaaaans.png")});
  //Clock
  faces[2].Sprites.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="clock", .wait_time=0.5, .ExpressionType=3, .Flip=false});
  faces[2].ColorMap.emplace_back(Expression_Params{.UNIQUE_IDENTIFYER="BluePink", .wait_time=0.041, .ExpressionType=0, .MediaPath=colormap_path("Traaaaaans.png")});
}
