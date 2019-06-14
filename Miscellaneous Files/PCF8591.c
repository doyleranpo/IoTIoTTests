#include "PCF8591.h"

int fd,value0,value1,value2,value3;

void initPCF8591(uint8_t x)// x is Address of ADC device
{
	shunyaInterfacesSetup();
	fd=wireSetup(x);	//0x48
	wireWrite(fd,0x10);
}

int readPCF8591(uint8_t y) // y is Pin Address of ADC
{


if(fd<0)
{
printf("I2C device not  found");
}

value0 = wireReadReg8(fd,y); //0x01
return value0;
}

