#include "ADXL.h"

void initADXL(){
	initPCF8591(0x48);
	}

int readADXL(int addr){
	return readPCF8591(addr);
	}
