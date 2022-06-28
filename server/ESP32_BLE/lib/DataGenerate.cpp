#include <cstdlib>
#include <ctime>
#include <array>
// Generate data of pulse oxymeter max30102
class MAX30102{
public:
    MAX30102(){
    }
    void _init_(){
        srand(time(NULL));
    }
    int Pulse(){
        return 60 + (rand() % 30); 
    }
    int O2(){
        return 90 + (rand() % 11);
    }
    float Gyro(){
        return 0.1 + rand() % 3; 
    }
    float Acc(){
        return -0.1 - rand() % 3; 
    }
};
