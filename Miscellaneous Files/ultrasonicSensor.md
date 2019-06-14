## Code
```c
#include<stdio.h>
#include<stdlib.h>
#include<shunyaInterfaces.h>

int cm;
int echo = 38;
int trig = 40;
int doPing = 1;

unsigned long duration;
void ping(){
	digitalWrite(trig,LOW);
	delayMicroseconds(2);
	//Set the trigger pin HIGH for 10 microseconds
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);	
	digitalWrite(trig, LOW);
}
void read_pulse()
{
    static unsigned long rising_time;  // time of the rising edge
    static int last_state;             // previous pin state
    int state = digitalRead(echo);      // current pin state
    duration = 0; // default return value

    // On rising edge: record current time.
    if (last_state == LOW && state == HIGH) {
        rising_time = micros();
    }

    // On falling edge: report pulse length.
    if (last_state == HIGH && state == LOW) {
        unsigned long falling_time = micros();
        duration = falling_time - rising_time;
    }

    last_state = state;
}
int main(){
	shunyaInterfacesSetup();
	pinMode(echo,INPUT);
	pinMode(trig,OUTPUT);
	while(1){
		if(doPing){
			ping();
			doPing = 0;
		}
		read_pulse();
		if(duration!=0){
			cm=duration*(0.034/2);
			printf("Distance: %d\n",cm);
			doPing = 1;
		}
	}
	return 0;
}
```

## output
```
Distance: 45
Distance: 43
Distance: 44
Distance: 44
Distance: 43

```
