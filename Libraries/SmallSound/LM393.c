/**
***************************************************************************************************
* @file		LM393.c
* @author	Ayush Srivastava
* @brief	The small sound sensor can detect sound and outputs both a analog signal as well as a
*		digital signal. This sensor is very useful with the relay module. It can be used to
*		create a clap on/clap of switch.
*		
***************************************************************************************************

				#### Functions Description ####
===================================================================================================
(#)void setupShunyaInterfaces() 	:Initialize the shunyaOS interface
(#)void pinMode(pin, state)		:Setting up a pin as INPUT/OUTPUT
(#)int digitalRead(pin)			:Reads the pinState as HIGH/LOW
(#)void delay(ms)			:Produces a delay equal to the value given(in ms) 

***************************************************************************************************
*/

#include "LM393.h"

int pin;

//Initialization
void initSoundSensor(int x)
{
    pin = x;
    pinMode(x, INPUT);// Setting the PIN connected to button as input
}

//Read Raw data
int readSoundSensor()
{
    if (digitalRead(pin)) /*Checking the input obtained from the sensor*/
        return 1;
    return 0;
}
