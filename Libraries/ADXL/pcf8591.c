/**
  ******************************************************************************
  * @file       pcf8591.c
  * @author     Shunya Core Team
  * @brief      This file contains low level code for pcf8591 ADC 
  *             library
  *         
  @verbatim
        PCF8591
        ---------------------------------------------------------------
        PCF8591 is a 8-bit data acquisition device, with analog inputs, 
        one analog output and a serial I2C-bus interface.

        For Users
        ================================================================
        Calling function -
        [1] pcf8591Read(physicalPin) - will return digital value taken from
        analog sensor connected to the corresponding physical pin. 


    @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 IoTIoT.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by IoTIoT under GPLv3 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/GPL-3.0
  *
  ******************************************************************************
  */

//includes
#include <stdio.h>
#include <stdlib.h>

#include <shunyaInterfaces.h>
#include <Wire.h>

#include "pcf8591.h"

static int isPcf8951Setuped = FALSE;
static int pcf8591Handle;

void pcf8591Setup()
{
        pcf8591Handle = wireSetup(PCF8591_I2C_ADDRESS);
        if(pcf8591Handle < 0)
        {
                printf("Failed to detect sensor PCF8591 on I2C bus. \n" 
                        "Sensor PCF8591 may not be connected to I2C bus.\n");

                EXIT_FAILURE;
        }

	wireWrite(pcf8591Handle,0x10);//0x10 sets high res-mode
        isPcf8951Setuped = TRUE; 
}

int pcf8591Read(int physicalPin)
{
        if (!isPcf8951Setuped){
                pcf8591Setup();    
        }
        return  wireReadReg8(pcf8591Handle,physicalPin); 
}

