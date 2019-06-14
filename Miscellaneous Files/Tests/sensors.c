#include "sensors.h"

int doPing;
unsigned long duration;
int cm;
int pin;
int trig;

//Sensor Initialization
void initReedSwitch(int a) {
  pin = a;
  pinMode(pin, INPUT);
}

void initTiltSwitch(int a) {
  pin = a;
  pinMode(pin, INPUT);
}

void initReadSound(int a) {
  pin = a;
  pinMode(pin, INPUT);
}

void initLightBlocking(int a){
  pin = a;
  pinMode(pin, INPUT);
}

void initTouchSensor(int a){
  pin = a;
  pinMode(pin,INPUT);
}

void initBallSwitch(int a){
  pin = a;
  pinMode(pin,INPUT);
}

void initHeartbeat(int a) {
  pin = a;
  pinMode(pin, INPUT);
}

void initUltra(int a, int b){
    pin = a;
  	trig = b;
  	pinMode(pin,INPUT);
  	pinMode(trig,OUTPUT);
}

//readRaw functions

//returns the current state of the reed switch
int readReedSwitch() {
    return ifCompare(pin);
}

//returns the current state of the tilt sensor
int readTiltSensor() {
    return ifCompare(pin);
}

//returns if the sound level is above a certain value or not (works for big sound and small sound)
void readSound() {
    return ifCompare(pin);
}

//returns if the light path is blocked or not
int readLightBlocking() {
    return ifCompare(pin);
}

//returns if the sensor is touched or not
int readTouchSensor() {
    return ifCompare(pin);
}

//returns if the sensor tilts
int readBallSwitch(){
  	return ifCompare(pin);
}

int readAvoidanceSensor() {
  return ifCompare(pin);
}

//returns 1/0 depending upon the state of the pin
int ifCompare() {
    if(!digitalRead(pin)) return 0;
    return 1;
}

//Computation
//returns distance from the object
int readUltrasonic() {
    if (doPing) {
        digitalWrite(trig, LOW);
        delayMicroseconds(2);

        //Set the trigger pin HIGH for 10 microseconds
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(trig, LOW);
        doPing = 0;
    }

    read_pulse(pin);

    if (!duration) {
        cm = duration * (0.034 / 2);
        doPing = 1; //Enables re-sending of sound pulse once distance is calculated
        return cm;
    }
}

//returns the heart rate of a person
int readHeartbeat() {
    int time = millis();
    while(digitalRead(pin));
    time = millis() - time;
    delay(10);

    int tempLOW = millis();
    while(!digitalRead(pin));
    time += millis() - tempLOW;

    int bpm = 60*(1000.0/time);
    if(bpm < 220) return bpm;
    return -1;
}

void readPulse(int pin) {
    static unsigned long rising_time; // time of the rising edge
    static int last_state;            // previous pin state
    int state = digitalRead(pin);    // current pin state
    duration = 0;                     // default return value

    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) { 
        //Check if the pin pin state has changed from low to high
        rising_time = micros();
        /*Obtain the instance when the change takes place
        (+)Note: micros() returns time passed before it was invoked*/
    }

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW){
        //Check once the pin pin state changes from high to low
        unsigned long falling_time = micros();
        /*Obtain the instance when pin changes from HIGH to LOW*/
        duration = falling_time - rising_time;
        /*Once the state changes from HIGH to LOW the total time can be computed for 
        which the pulse on pin was high. This is then used to calculate the duration 
        using the formula falling_time - rising time.*/
    }

    last_state = state;
}


