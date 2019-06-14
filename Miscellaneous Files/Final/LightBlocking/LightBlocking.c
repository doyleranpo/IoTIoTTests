/**
***************************************************************************************************
* @file		LightBlocking.c
* @author	Ayush Srivastava
* @brief	This is a basic example to toggle an 
*		LED using a light blocking/photo interrupter sensor
***************************************************************************************************

				#### Functions Description ####
===================================================================================================
(#)void setupShunyaInterfaces() 	:Initialize the shunya interface
(#)void pinMode(pin, state)		:Setting up a pin as INPUT/OUTPUT
(#)int digitalRead(pin)			:Reads the pinState as HIGH/LOW
(#)void delay(ms)			:Produces a delay equal to the value given(in ms) 

***************************************************************************************************
*/


#include "LightBlocking.h"

int pin;
 //Initialization
void initLightBlocking(int a){
	
	pin = a;
	pinMode(pin, INPUT); // Setting the PIN connected to button as input to receive a signal from the photo interrupter 
}

//Reading Raw data
int readLightBlocking {
    if(!digitalRead(pin)) return 0;  // Checking sensor output 
    return 1;
}

