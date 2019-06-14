#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
//#include <OneWire.h>

OneWire ds(10); //DS18S20 on pin 10

int main()
{
	serialOpen(9600);
	while(1)
	{
		byte i;
		byte present = 0;
		byte data[12];
		byte addr[8];
		
		df.reset_search();
		if ( !ds.search(addr)){
			serialPrintf("No more addresses.\n");
			ds.reset_search();
			return;
		}
		
		serialPrintf("R=");
		for( i = 0; i < 8; i++) {
			serialPrintf(addr[i],HEX);
			serialPrintf(" ");
		}
		
		if (OneWire::crc8( addr, 7)!= addr[7]){
			serialPrintf("CRC is not valid\n");
			return;
		}
		
		if( addr[0] == 0x10){
			serialPrintf("Device is a DS18S20 family device.\n");
		}
		else if( addr[0] == 0x28){
			serialPrintf("Device is a DS18B20 family device.\n");
		}
		else {
			serialPrintf("Device family is not recognized: 0x");
			serialPrintf(addr[0],HEX);
			return;
		}
		
		ds.reset();
		ds.select(addr);
		ds.write(0x44, 1);
		
		delay(1000);
		
		present = ds.reset();
		ds.select(addr);
		ds.write(0xBE);
		
		serialPrintf("P=");
		serialPrintf(present,HEX);
		serialPrintf(" ");
		for ( i = 0; i < 9; i++){
			data[i] = ds.read();
			serialPrintf(data[i], HEX);
			serialPrintf(" ");
		}
		serialPrintf(" CRC=");
		serialPrintf( OneWire::crc8(data, 8),HEX);
		serailPrintf("\n");
	}		
}			
