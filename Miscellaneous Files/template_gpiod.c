/**
  ******************************************************************************
  * @file       template.c
  * @author     Shunya Core Team
  * @brief      This file contains functions & variables specific to 
  *             <boardname>
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


#include "../gpioHAL/gpiodCore.h"
#include "shunyaInterfaces.h"
#include "boardname.h"

#define GPIO_CHIP_NUMBER_INDEX 0
#define GPIO_LINE_NUMBER_INDEX 1


static const int physicalToCharDevMap[64][2] = {
// {GPIOchipNumer, GPIOlineNumber}   Physical pin 
        { -1, -1 },                 //  0
        {-1, -1 },  {-1, -1 },      //  1 |  2 :
        {-1, -1 },  {-1, -1 },      //  3 |  4 : 
        {-1, -1 },  {-1, -1 },      //  5 |  6 :
        {-1, -1 },  {-1, -1 },      //  7 |  8 :
        {-1, -1 },  {-1, -1 },      //  9 | 10 :
        {-1, -1 },  {-1, -1 },      // 11 | 12 :
        {-1, -1 },  {-1, -1 },      // 13 | 14 :
        {-1, -1 },  {-1, -1 },      // 15 | 16 :
        {-1, -1 },  {-1, -1 },      // 17 | 18 :
        {-1, -1 },  {-1, -1 },      // 19 | 20 :
        {-1, -1 },  {-1, -1 },      // 21 | 22 :
        {-1, -1 },  {-1, -1 },      // 23 | 24 :
        {-1, -1 },  {-1, -1 },      // 25 | 26 :
        {-1, -1 },  {-1, -1 },      // 27 | 28 :
        {-1, -1 },  {-1, -1 },      // 29 | 30 :
        {-1, -1 },  {-1, -1 },      // 31 | 32 :
        {-1, -1 },  {-1, -1 },      // 33 | 34 :
        {-1, -1 },  {-1, -1 },      // 35 | 36 :
        {-1, -1 },  {-1, -1 },      // 37 | 38 :
        {-1, -1 },  {-1, -1 },      // 39 | 40 :
        // Unused  pins
        {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}, // 41...46
        {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}, // 47...52
        {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}, // 53...58
        {-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}          // 59...63
};


static struct boardData *pointerToBoard = NULL;

static struct gpiod_line *gpioLineHandles[64];

static void _pinMode (int physicalPin , int mode)
{
        setupCheck ("pinMode") ;

        int gpioChipNumber = physicalToCharDevMap[physicalPin][GPIO_CHIP_NUMBER_INDEX];
        int gpioLineNumber = physicalToCharDevMap[physicalPin][GPIO_LINE_NUMBER_INDEX];

        gpioLineHandles[physicalPin] = gpiodGpioExport(gpioChipNumber,gpioLineNumber, physicalPin);
        if (gpioLineHandles[physicalPin] < 0) {
                // exit
                exit(EXIT_FAILURE); 
        }
        gpiodGpioDirection(gpioLineHandles[physicalPin],physicalPin,mode);
}


static void _pullUpDnControl (int pin, int pud)         { return; }


static int _digitalRead (int physicalPin)
{

        int value = gpiodGpioRead(gpioLineHandles[physicalPin],physicalPin);
        return(value);
}


static void _digitalWrite (int physicalPin, int value)
{
        gpiodGpioWrite(gpioLineHandles[physicalPin],physicalPin,value);
}

/*
static int _attachInterrupt (int physicalPin, int mode, void (*function)(void))
{
        int gpioChipNumber = physicalToGpiochipNumber[physicalPin];
        int gpioLineNumber = physicalToGpiolineNumber[physicalPin];     
        return gpiodISR(gpioChipNumber,gpioLineNumber,mode,function);
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
	/* Core function initialize */
	devBoard->pinMode			= _pinMode;
	devBoard->pullUpDnControl	= _pullUpDnControl;
	devBoard->digitalRead		= _digitalRead;
	devBoard->digitalWrite		= _digitalWrite;
	devBoard->getI2cDevice		= _getI2cDevice;
	devBoard->getSpiDevice		= _getSpiDevice;
        //devBoard->attachInterrupt     = _attachInterrupt;

	/* global variable setup */
	pointerToBoard = devBoard;
}

