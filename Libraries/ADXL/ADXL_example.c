#include "ADXL.h"
int main(){
    initADXL()
    int x,y,z;
    while(1){
        x = readADXL(0x01));
        y = readADXL(0x10));
        z = readADXL(0x11));
        printf("X : %d \n Y: %d \n Z: %d \n",x,y,z);
        delay(100);
    }
}
