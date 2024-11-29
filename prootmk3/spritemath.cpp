#include "spritemath.hpp"

void SpriteMath::SpriteUpdate(Expression& FaceSprites){
    for (Expression::Expression_sprite& sprite : FaceSprites.Sprites){
        //Animation Logic, in this implementation its only for a eyeblinking
        if (sprite.ActiveAnimation){
        if (sprite.IsUpdateTime2()){
            if (sprite.AnimationInversion == false){ //If the eye is OPENING 
                if (sprite.UIT < blink_Cycles - 1){
                    sprite.UIT++;
                }
                else {
                    RawBezierTrajectory = Calculate_Many_Bezier_Curves(sprite.OposingChords, sprite.MainChords, blink_Cycles);
                    sprite.UIT = 0;
                    sprite.AnimationInversion = true; 
                }
            } 
            else {
                if (sprite.UIT < blink_Cycles - 1){
                    sprite.UIT++; 
                }
                else {
                    sprite.AnimationInversion = false;
                    sprite.ActiveAnimation = false;
                    sprite.UIT = 0;
                    continue;
                }
            }

            
            std::vector<cv::Point> SpriteGeomatry = UnpackBezierArray(sprite.UIT, RawBezierTrajectory, blink_Cycles);
            const int numPoints[] = { static_cast<int>(sprite.MainChords.size()) };
            cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            const cv::Point* NumberOfChords = &SpriteGeomatry[0];
            cv::fillPoly(TempSprite, &NumberOfChords, numPoints , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
            Change = true; 
        } else { 
            continue;
        }
        }
        else if (sprite.NeedBezier == true && sprite.IsUpdateTime() == true)
        {   
            sprite.UIT = 0;
            RawBezierTrajectory = Calculate_Many_Bezier_Curves(sprite.MainChords, sprite.OposingChords, blink_Cycles);
            static std::vector<cv::Point> SpriteGeomatry = UnpackBezierArray(sprite.UIT, RawBezierTrajectory, blink_Cycles);
            cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            const cv::Point* NumberOfChords = &SpriteGeomatry[0];
            const int numPoints[] = { static_cast<int>(sprite.MainChords.size()) };
            cv::fillPoly(TempSprite, &NumberOfChords, numPoints, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
            Change = true; 
            sprite.ActiveAnimation = true; 
        }
        else if (sprite.IsUpdateTime()){
            cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            const cv::Point* NumberOfChords[1] = { sprite.MainChords.data() };
            const int numPointsNose[] = { static_cast<int>(sprite.MainChords.size()) };
            cv::fillPoly(TempSprite, NumberOfChords, numPointsNose , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
        }
    }
}

void SpriteMath::SpriteColorMapUpdate(Expression& FaceSprites){
    
    if(FaceSprites.ColorMap[0].IsUpdateTime()==true){
        Change=true;
        if(FaceSprites.ColorMap[0].isGif){
            cv::Mat bakrnd_frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            int CurrentFrame = static_cast<int>(FaceSprites.ColorMap[0].video.get(cv::CAP_PROP_POS_FRAMES));
            if (CurrentFrame >= FaceSprites.ColorMap[0].TOTAL_FRAMES){
                FaceSprites.ColorMap[0].video.set(cv::CAP_PROP_POS_FRAMES, 0);
            }
            FaceSprites.ColorMap[0].video.read(bakrnd_frame);
            InUseColorMap = bakrnd_frame;
        }
    }
    else {}
}
/*
void SpriteMath::SpriteUpdate(Expression& FaceSprites){
    for (Expression::Expression_sprite& sprite : FaceSprites.Sprites){
        if (sprite.IsUpdateTime()){
        if (sprite.NeedBezier){
            
        }
        else {
            cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            const cv::Point* NumberOfChords[1] = { sprite.MainChords.data() };
            const int numPointsNose[] = { static_cast<int>(sprite.MainChords.size()) };
            cv::fillPoly(TempSprite, NumberOfChords, numPointsNose , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
        }
        }
    }
}
*/

/*
void SpriteMath::EyeUpdate(){
    timeBetweenBlink.elapsed_time = std::chrono::steady_clock::now() - timeBetweenBlink.action_time;
    if (Blinking == true) {
        blinkSpeed.elapsed_time = std::chrono::steady_clock::now() - blinkSpeed.action_time;
        if (blinkSpeed.elapsed_time.count() >= blinkSpeed.wait_time){
            if (eyeOpening == true){
                if (n < blink_Cycles - 1){
                    n++;
                }
                else {
                    RawBezierEye = Calculate_Many_Bezier_Curves(Happy.CLOSED, Happy.DEFAULT, blink_Cycles);
                    n = 0;
                    eyeOpening = false;
                }
            } 
            else {
                if (n < blink_Cycles - 1){
                    n++; 
                }
                else {
                    eyeOpening = true;
                    Blinking = false;
                    timeBetweenBlink.action_time = std::chrono::steady_clock::now();
                    n = 0;
                    return;
                }
            }
            
            EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles);
            const cv::Point* ArrayEyeVerticies = &EyeVerticies[0];
            cv::fillPoly( EyeSprite, &ArrayEyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            blinkSpeed.action_time = std::chrono::steady_clock::now();
            Change = true; 
        }
    }
    else if (timeBetweenBlink.elapsed_time.count() >= timeBetweenBlink.wait_time)
    {
        n = 0;
        RawBezierEye = Calculate_Many_Bezier_Curves(Happy.DEFAULT, Happy.CLOSED, blink_Cycles);
        EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles);
        EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
        const cv::Point* ArrayEyeVerticies = &EyeVerticies[0];
        cv::fillPoly(EyeSprite, &ArrayEyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
        timeBetweenBlink.action_time = std::chrono::steady_clock::now();
        Change = true; 
        Blinking = true; 
    }
}
*/
