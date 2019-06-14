/*
				### Example ####
***************************************************************************************************
*	A simple example to show the setting up and usage of Photo Interupter.
*	It can also be used to trigger a relay/switch by a simple addition of 1 output pin connected
* 	to the switch/relay
	
				### How to use ###
===================================================================================================
(#) Connect the hardware according to the circuit diagram
	(##) Add an LED to the pin defined as trigger
(#) Run the following code and introduce an obstruction to the path of light. The LED attached flashes
    as a result
***************************************************************************************************
*/
#include "LightBlocking.h"

int main(){
	shunyaInterfacesSetup();  //Setting up shunyaOS interfaces
	initLightBlocking(40); //Initializing the sensor 
	while(1){
		val = readLightBlocking();
		if( val == 1) 
			printf("Light path blocked");
		else
			printf("Light path restored");
	}
}
