/**
***************************************************************************************************
* @file		small_sound.c
* @author	Ayush Srivastava
* @brief	The small sound sensor can detect sound and outputs both a analog signal as well as a *		digital signal. This sensor is very useful with the relay module. It can be used to *		create a clap on/clap of switch.
*		
***************************************************************************************************

					#### Functions Description ####
===================================================================================================
(#)void setupShunyaInterfaces() 	:Initialize the shunya interface
(#)void pinMode(pin, state)		:Setting up a pin as INPUT/OUTPUT
(#)int digitalRead(pin)			:Reads the pinState as HIGH/LOW
(#)void delay(ms)			:Produces a delay equal to the value given(in ms) 

***************************************************************************************************
	
					### How to use ###
===================================================================================================
(#) Connect the hardware according to the circuit diagram
	(##) Add an LED to the pin defined as trigger
(#) Run the following code and the output can be seen when the LED flashes

***************************************************************************************************
*/

#include<stdio.h>
#include<stdlib.h>
#include <shunyaInterfaces.h>
int trigger = 23;
int button = 3;
int val = 0;

int main()
{
	shunyaInterfacesSetup(); //Setting up shunyaOS interfaces 
	pinMode (trigger, OUTPUT); // Defined as an output pin so that it may act as a switch/trigger
	pinMode (button, INPUT); // Setting the PIN connected to button as input
	val = digitalRead(button); 
	if (val == HIGH) /*Checking the input obtained from the sensor*/
		{
			digitalWrite(trigger,HIGH); 
			/*Sending a HIGH on trigger when sensor output is HIGH*/
			delay(100);
		}
	else
		{
			digitalWrite(trigger,LOW); 
			/*Turning OFF the device connected to trigger when no output from sensor*/
			delay(100);
		}
}
