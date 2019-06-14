#include "ultra.h"

int echo;
int trigger;
int doPing = 1;

unsigned long duration;

void initUltra(int a, int b){
   	echo = a;
  	trigger = b;
  	pinMode(pin,INPUT);
  	pinMode(trig,OUTPUT);
}

void ping(){
	//Clearing the trigger pin 
	digitalWrite(trig,LOW)
	delayMicroseconds(2);
	//Set the trigger pin HIGH for 10 microseconds
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);	
	digitalWrite(trigger, LOW);
}

int readUltra(){
	if(doPing){ 
			ping(); //Transmit a sound pulse if it hasn't been already sent
			doPing = 0; //No ping to be sent until echo is obtained
		}
	read_pulse();
	if(duration!=0){
		cm=duration*(0.034/2);
		/*Note: The formula is calculated assuming the speed of sound as 340 m/s or 
		0.034 cm/us. t = 2*s/v or s = t * 0.034/2 (in cm)*/	 
		doPing = 1;
		return cm
		}
}

void read_pulse()
{
    static unsigned long rising_time;  // time of the rising edge
    static int last_state;             // previous pin state
    int state = digitalRead(echo);      // current pin state
    duration = 0; // default return value

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
