/**
  ******************************************************************************
  * @file       template.c
  * @author     Shunya Core Team
  * @brief      This file contains functions & variables specific to 
  *             
  * @note       This file defines board pins and I2C/SPI device nodes
  *         
  @verbatim
 
    @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 IoTIoT.in
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by IoTIoT under GPLv3 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/GPL-3.0
  *
  ******************************************************************************
  */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <sys/mman.h>
#include <sys/utsname.h>


#include "../gpioHAL/sysfsCore.h"
#include "shunyaInterfaces.h"
#include "boardname.h"


static const int physicalToLinuxpinNumber[64] = {
	// physical header pin number to Linux pin Number
	-1,		//  0
	-1,  	-1,	//  1 |  2 :
	-1,  	-1,	//  3 |  4 :
	-1,  	-1,	//  5 |  6 :
	-1, 	-1,	//  7 |  8 :
	-1,  	-1,	//  9 | 10 :
	-1,	-1,	// 11 | 12 :
	-1, 	-1,	// 13 | 14 :
	-1, 	-1,	// 15 | 16 :
	-1, 	-1,	// 17 | 18 :
	-1, 	-1,	// 19 | 20 :
	-1,	-1,	// 21 | 22 :
  	-1,    -1,    // 23 | 24 :
	-1,    -1,    // 25 | 26 : 
	-1,    -1,    // 27 | 28 :
	-1,    -1,     // 29 | 30 : 
	-1,    -1,    // 31 | 32 :      
	-1,    -1,     // 33 | 34 :	
	-1,    -1,	// 35 | 36 :
	-1,    -1,	// 37 | 38 :
	-1,    -1,	// 39 | 40 :
	// Not used
	-1, -1, -1, -1, -1, -1, -1, -1,	// 41...48
	-1, -1, -1, -1, -1, -1, -1, -1,	// 49...56
	-1, -1, -1, -1, -1, -1, -1	// 57...63
};

static struct boardData *pointerToBoard = NULL;


static void		_pinMode		(int pin, int mode);
static void		_pullUpDnControl	(int pin, int pud);
static int		_digitalRead	(int pin);
static void		_digitalWrite	(int pin, int value);


static void _pinMode (int physicalPin, int mode)
{
	setupCheck ("pinMode") ;
	sysfsGpioExport(physicalToLinuxpinNumber[physicalPin]);
	sysfsGpioDirection(physicalToLinuxpinNumber[physicalPin],mode);
}


static void _pullUpDnControl (int pin, int pud)		{ return; }


static int _digitalRead (int physicalPin)
{
	int value;
        value = sysfsGpioRead(physicalToLinuxpinNumber[physicalPin]);

	return value;
}


static void _digitalWrite (int physicalPin, int value)
{
	sysfsGpioWrite(physicalToLinuxpinNumber[physicalPin],value);
}

/*
static int _attachInterrupt (int physicalPin, int mode, void (*function)(void))
{
	int linuxPinNumber;

	linuxPinNumber = physicalToLinuxpinNumber [physicalPin];
	shunyaISR (linuxPinNumber, mode, function);
}
*/
static char *_getI2cDevice ()
{
        const char *i2cDevice = NULL;

        i2cDevice = "/dev/i2c-?";

        return i2cDevice;
}

static char *_getSpiDevice ()
{
        const char *spiDevice = NULL;

        spiDevice = "/dev/spidev?.?";

        return spiDevice;
}

static char *_getUARTDevice ()
{
        const char *UARTDevice = NULL;

        UARTDevice = "/dev/tty?";

        return UARTDevice;
}

void boardnameSetup (struct boardData *devBoard)
{
	/* initialize Core functions */
	devBoard->pinMode		= _pinMode;
	devBoard->pullUpDnControl	= _pullUpDnControl;
	devBoard->digitalRead		= _digitalRead;
	devBoard->digitalWrite		= _digitalWrite;
	devBoard->getI2cDevice		= _getI2cDevice;
	devBoard->getSpiDevice		= _getSpiDevice;

	/* global variable setup */
	pointerToBoard = devBoard;
}

