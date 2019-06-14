void setup(int in_put, int out_put){
    pinMode(in_put, INPUT);
    pinMode(out_put, OUTPUT);
}
//ULTRASONIC SENSOR
// void ultra_init(int trig, int echo){
// 	pinMode(echo,INPUT); // Pin set as output
// 	pinMode(trig,OUTPUT); //Pin set as input
// }
void read_pulse(int echo)
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


int ultra_calc_duration(int trig, int echo){
    if(doPing){ 
		//Clearing the trigger pin 
	    digitalWrite(trig,LOW);
	    delayMicroseconds(2);
	    //Set the trigger pin HIGH for 10 microseconds
    	digitalWrite(trig, HIGH);
	    delayMicroseconds(10);	
	    digitalWrite(trig, LOW);
		    doPing = 0; //No ping to be sent until echo is obtained
		}
    read_pulse(echo);
	if(duration!=0){
		cm = duration*(0.034/2);
		/*Note: The formula is calculated assuming the speed of sound as 340 m/s or 
		0.034 cm/us. t = 2*s/v or s = t * 0.034/2 (in cm)*/ 
		doPing = 1; //Enables re-sending of sound pulse once distance is calculated
        return cm;
	}
}
int readUltrasonic(int trig, int echo){
    ultra_init(trig, echo);
    return ultra_calc_duration(trig, echo)
}
