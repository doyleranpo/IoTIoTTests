#include "small_sound.c"
/*The following 2 values LED and button can be changed in accordance with the pins used on the board.*/
int LED = 23; //Defining pin number with LED
int button = 3; //D0 Sensor
int main()
{	
	sensor_init(LED,button)
	while(1)
	{
		sensor_exec(LED,button)
	}
	return 0;
}

