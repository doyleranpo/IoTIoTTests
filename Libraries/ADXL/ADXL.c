#include "ADXL.h"

void initADXL(){
	pcf8591Setup();
	}

int readADXL(int addr){
	return pcf8591Read(addr);
	}
