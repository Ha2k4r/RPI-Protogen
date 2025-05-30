#include "spritemath.hpp"

void SpriteMath::SpriteUpdate(Expression& FaceSprites){
    for (Expression::Expression_sprite& sprite : FaceSprites.Sprites){
        //Animation Logic, in this implementation its only for a eyeblinking
        switch (sprite.ExpressionType)
        {
            case -1 :
                if (sprite.IsUpdateTime()){
                    cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
                    const cv::Point* NumberOfChords[1] = { sprite.MainChords.data() };
                    const int numPointsNose[] = { static_cast<int>(sprite.MainChords.size()) };
                    cv::fillPoly(TempSprite, NumberOfChords, numPointsNose , 1, cv::Scalar(255, 255, 255), cv::LINE_8);
                    InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
                }
                break;

            case 1 :
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
                    else if (sprite.IsUpdateTime() == true)
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
                break;
            case 3:
                auto now = std::chrono::system_clock::now();
                std::time_t time = std::chrono::system_clock::to_time_t(now);
                std::tm* localTime = std::localtime(&time); 
                char buffer[80];
                std::strftime(buffer, sizeof(buffer), "%r:%P", localTime);


                cv::Mat TempSprite = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
                cv::putText(TempSprite,buffer,cv::Point(5,29), cv::FONT_HERSHEY_COMPLEX_SMALL,1,(120,81,169),1);
                InUseSprites[sprite.UNIQUE_IDENTIFYER] = TempSprite;
                break;
        }
    }
}

void SpriteMath::SpriteColorMapUpdate(Expression& FaceSprites){
    
    if(FaceSprites.ColorMap[0].IsUpdateTime()==true){
        Change=true;
            cv::Mat bakrnd_frame = cv::Mat::zeros(cv::Size(64, 32), CV_8UC1);
            int CurrentFrame = static_cast<int>(FaceSprites.ColorMap[0].video.get(cv::CAP_PROP_POS_FRAMES));
            if (CurrentFrame >= FaceSprites.ColorMap[0].TOTAL_FRAMES){
                FaceSprites.ColorMap[0].video.set(cv::CAP_PROP_POS_FRAMES, 0);
            }
            FaceSprites.ColorMap[0].video.read(bakrnd_frame);
            InUseColorMap = bakrnd_frame;
    }
    else {}
}


// Function to calculate a single point on a Bezier curve
int SpriteMath::Calculate_Single_Bezier_Curve(double t, int p0, int p1, int p2, int p3) {
    int value = static_cast<int>(round((1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3));
    return value;
}


std::vector<cv::Point> SpriteMath::Calculate_Many_Bezier_Curves(const std::vector<cv::Point>& Array, const std::vector<cv::Point>& Target_Array, int num_points, double factor1, double factor2) {
    //FUTURE IDEA, Make debug mode or dev mode to switch between advanced error reporting and none for speed purpases.
    //I am absolutly open to any implementation using pointers 
    size_t total_points = Array.size() * num_points;
    std::vector<cv::Point> RawBezierArray;
    RawBezierArray.resize(total_points);

    int index = 0;

    for (int i = 0; i < Array.size(); ++i) {
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


std::vector<cv::Point> SpriteMath::UnpackBezierArray(int Index, std::vector<cv::Point> RawBezierArray, int Number_Of_Points_In_Polygon) {
    int maxindex = RawBezierArray.size()/Number_Of_Points_In_Polygon;
    
    // Allocate memory for the vertices
    std::vector<cv::Point> Polygon_Verticies(maxindex);
    
    // Copy the relevant points to Geometric_Vertices
    for (int i = 0; i < maxindex; i++) {
        int target = (Number_Of_Points_In_Polygon * i) + Index;
        Polygon_Verticies[i] = RawBezierArray[target];
        //std::cout << "Geometric Verticies   : " << Geometric_Vertices[i] << std::endl;
    }
    return Polygon_Verticies;
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
