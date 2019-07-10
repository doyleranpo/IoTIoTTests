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
    initADXL(); //Initializing ADXL
    while (1)
    {
        printf("X pin %d\n",readADXL(A1)); 
        // delay(100);
        printf("Y pin %d\n",readADXL(A2)); 
        // delay(100);
        printf("Z pin %d\n",readADXL(A3));
        delay(1000);
        /*Reading converted value at pins of ADC whose addresses are given by 0x01, 0x10, 0x11*/
    }
}