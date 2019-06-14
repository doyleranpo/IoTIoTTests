#include<stdio.h>
#include<stdlib.h>
//#include<shunyaInterfaces.h>
#include "PCF8591.h"
int x = 40;
int y = 38;
int z = 37;

int main(){
    initPCF8591(0x48);
    while(1){
        printf("X pin %d\n",readPCF8591(0x01));
        printf("Y pin %d\n",readPCF8591(0x10));
        printf("Z pin %d\n",readPCF8591(0x11));
        delay(100)
    }
}
