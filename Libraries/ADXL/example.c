/**
***************************************************************************************************
*   Reads an Analog Devices ADXL3xx accelerometer and communicates the
*   acceleration to the computer. 
*   	 
***************************************************************************************************
					### How to use ###
===================================================================================================
(#) Connect the hardware according to the circuit diagram
(#) Run the following code and force due to gravity can be seen on the terminal

***************************************************************************************************
**/

#include "ADXL.h"
void main(){
    int x,y,z;
    shunyaInterfacesSetup();
    initADXL(); //Initializing ADXL
    while (1)
    {
        x = readADXL(A1);
        y = readADXL(A2);
        z = readADXL(A3);
        printf("X pin %d\n",x); 
        delay(100);
        printf("Y pin %d\n",y); 
        delay(100);
        printf("Z pin %d\n",z);
        delay(1000);
        /*Reading converted value at pins of ADC whose addresses are given by 0x01, 0x10, 0x11*/
    }
}
