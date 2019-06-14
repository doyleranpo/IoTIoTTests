/**
***************************************************************************************************
* @file		light_blocking.c
* @author	Ayush Srivastava
* @brief	In the light block sensor module, when there is an object in the middle of the U
*		shape. The sensor will output a high level signal.		
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
(#) Run the following code and introduce an obstruction to the path of light. The LED attached flashes
    as a result
***************************************************************************************************
*/
#include<stdio.h>
#include<shunyaInterfaces.h>
int button = 38;
int led = 40;

int val = 0;

int main()
{
	shunyaInterfacesSetup(); //Setting up shunyaOS interfaces
	pinMode(led,OUTPUT); // Defined as an output pin so that it may act as a switch/trigger
	pinMode(button,INPUT);  // Setting the PIN connected to button as input to receive a signal from the photo interrupter 
	while(1)
	{
		val = digitalRead(button)
		if( val == HIGH )
		{
			digitalWrite(led, HIGH);
			//Sending a HIGH to trigger when the light is obstructed
		}
		else
		{
			digitalWrite(led, LOW);
			//Turning the circuit OFF once light path is restored
		}
	}
}
