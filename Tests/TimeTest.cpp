#include <chrono>
#include <iostream>

struct BlinkValues
{
    std::chrono::time_point<std::chrono::steady_clock> BlinkTime = std::chrono::steady_clock::now();
    double Blink_Wait_Time = 0.2; // Wait time in seconds
};

int main(){
    struct BlinkValues bv;

    while(true){
        
        std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - bv.BlinkTime;

        if(elapsed.count() >= bv.Blink_Wait_Time){
            std::cout << "Time Elapsed" << std::endl;
            bv.BlinkTime = std::chrono::steady_clock::now(); // Reset BlinkTime
        }
    }

    return 0;
}
