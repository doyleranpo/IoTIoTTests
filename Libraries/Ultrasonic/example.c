
/**
					## NOTE ##
*	Adding any sort of delay to print the values of distance can cause issues with output being
*	diplayed as 0

					## Example
***************************************************************************************************
*	Simple example showing how to use the ultrasonic sensor to measure the distance between
*	the senor and an object. 
*	Minimum Distance: 2 cm
*	Maximum Distance: 4 m
***************************************************************************************************


					### How to use ###
===================================================================================================
(#) Connect the hardware according to the circuit diagram
(#) Run the following code and the distance of the object can be seen on the terminal

***************************************************************************************************
**/
#include "HSC-SR04.h"
int main(){
	shunyaInterfacesSetup();
	initUltra(38,40); //to initialize the order of pins is INPUT,OUTPUT
	/*In the above function call 38 is connected to ECHO pin of the Sensor and 40 to the trigger*/
	int x;
	while(1){
		x = readUltra();
		printf("Distance is %d cm \n",x);
		}
	}

