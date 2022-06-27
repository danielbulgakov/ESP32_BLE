#include <cstdlib>
#include <ctime>
#include <array>
// Generate data of pulse oxymeter max30102
class MAX30102{
public:
    MAX30102(){
        srand(time(NULL));
    }
    int Pulse(){
        return (int)(rand() % 150); 
    }
    int O2(){
        return(int)(rand() % 100);
    }
    float Gyro(){
        return 0.1 + rand() % 2; 
    }
    float Acc(){
        return -0.1 - rand() % 2; 
    }
};
