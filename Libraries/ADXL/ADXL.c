#include "ADXL.h"

void initADXL(){
	pcf8591Setup(); //Initializing the ADC PCF8591
	}

int readADXL(int addr){
	return pcf8591Read(addr); //Get value from the ADC depending on the pin which was given
	}
