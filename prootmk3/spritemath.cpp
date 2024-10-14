#include "spritemath.hpp"
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
                    RawBezierEye = Calculate_Many_Bezier_Curves(Happy.CLOSED, Happy.DEFAULT, blink_Cycles, arraySize);
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
            
            int Sizeof_RawBezierEye = arraySize * blink_Cycles;
            EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles, Sizeof_RawBezierEye);
            const cv::Point* ArrayEyeVerticies = &EyeVerticies[0];
            cv::fillPoly( EyeSprite, &ArrayEyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
            blinkSpeed.action_time = std::chrono::steady_clock::now();
            Change = true; 
        }
    }
    else if (timeBetweenBlink.elapsed_time.count() >= timeBetweenBlink.wait_time)
    {
        n = 0;
        int Sizeof_RawBezierEye = (arraySize * blink_Cycles) * 8;
        RawBezierEye = Calculate_Many_Bezier_Curves(Happy.DEFAULT, Happy.CLOSED, blink_Cycles, arraySize);
        EyeVerticies = UnpackBezierArray(n, RawBezierEye, blink_Cycles, Sizeof_RawBezierEye);
        EyeSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
        const cv::Point* ArrayEyeVerticies = &EyeVerticies[0];
        cv::fillPoly(EyeSprite, &ArrayEyeVerticies, numPointsEye, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
        timeBetweenBlink.action_time = std::chrono::steady_clock::now();
        Change = true; 
        Blinking = true; 
    }
}

void SpriteMath::NoseUpdate(){    
    noseUpdate.elapsed_time = std::chrono::steady_clock::now() - noseUpdate.action_time;
    if(noseUpdate.elapsed_time.count() >= noseUpdate.wait_time){
      NoseSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      const cv::Point* ArrayNoseVerticies = &norm.DEFAULT[0];
      cv::fillPoly(NoseSprite, &ArrayNoseVerticies, numPointsNose , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
    }
}

void SpriteMath::MouthUpdate(){
    mouthUpdate.elapsed_time = std::chrono::steady_clock::now() - mouthUpdate.action_time;
    if(mouthUpdate.elapsed_time.count() >= mouthUpdate.wait_time){
      MouthSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
      const cv::Point* ArrayMouthVerticies = &morm.DEFAULT[0];
      cv::fillPoly(MouthSprite, &ArrayMouthVerticies, numPointsMouth, 1, cv::Scalar(255, 255, 255), cv::LINE_8);
    }
}