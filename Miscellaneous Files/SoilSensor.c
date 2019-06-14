#include "PCF8591.h"
long output;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(){
    initPCF8591(0x48);
    printf("Reading from Sensor")
    while(1){
        output = readPCF8591(0x01);
        output = map(output, 0, 550, 0, 100);
        printf("Soil Moisture is %d %%",output);
    }
}