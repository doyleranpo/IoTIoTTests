/**
***************************************************************************************************
* @file		ultra.c
* @author	Ayush Srivastava
* @brief	The sensor sends 8 cyclic sonic bursts on being triggered. The output is obtained on
*		the mic. The time difference between transmission and reception is used to calculate
*		the distance of the object from the sensor.
		
***************************************************************************************************
					#### Functions Description ####
===================================================================================================
(#)void setupShunyaInterfaces() 	:Initialize the shunya interface
(#)void pinMode(pin, state)		:Setting up a pin as INPUT/OUTPUT
(#)int digitalRead(pin)			:Reads the pinState as HIGH/LOW
(#)digitalWrite(pin, vale)		:Sets the pinState as HIGH/LOW
(#)void delayMicroseconds(us)		:Produces a delay equal to the value given(in us) 
(#)int micros(us)			:Returns the time(in us) which have passed since the start of execution

****************************************************************************************************
*/
#include "HC-SR04.h"

int echo;
int trigger;
int doPing = 1;
int cm;

unsigned long duration = 0;

//Initialization
void initUltra(int a, int b){
   	echo = a;
  	trigger = b;
  	pinMode(echo,INPUT); //Setting up pin as INPUT
  	pinMode(trigger,OUTPUT); //Setting up pin as output
}

//Configuration
void ping(){
	//Clearing the trigger pin 
	digitalWrite(trigger,LOW);
	delayMicroseconds(2);
	//Set the trigger pin HIGH for 10 microseconds
	digitalWrite(trigger, HIGH);
	delayMicroseconds(10);	
	digitalWrite(trigger, LOW);
}

//Reading Raw data
int readUltra(){
	if(doPing){ 
			ping(); //Transmit a sound pulse if it hasn't been already sent
			doPing = 0; //No ping to be sent until echo is obtained
		}
	readPulse();
	if(duration!=0){
		cm=duration*(0.034/2);
		/*Note: The formula is calculated assuming the speed of sound as 340 m/s or 
		0.034 cm/us. t = 2*s/v or s = t * 0.034/2 (in cm)*/	 
		doPing = 1;
		return cm;
		}
}

//Computing values
void readPulse()
{
    static unsigned long rising_time;  // time of the rising edge
    static int last_state;             // previous pin state
    int state = digitalRead(echo);      // current pin state

    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) { //Check if the echo pin state has changed from low to high
        rising_time = micros();
        /*Obtain the instance when the change takes place
        (+)Note: micros() returns time passed before it was invoked*/
    }

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW) { //Check once the echo pin state changes from high to low
        unsigned long falling_time = micros();
        /*Obtain the instance when echo changes from HIGH to LOW*/
        duration = falling_time - rising_time;
        /*Once the state changes from HIGH to LOW the total time can be computed for which the pulse
        on echo was high. This is then used to calculate the duration using the formula falling_time -
        rising time.*/
    }

    last_state = state;
}
