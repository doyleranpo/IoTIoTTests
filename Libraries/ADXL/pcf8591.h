/**
  ******************************************************************************
  * @file       pcf8591.h
  * @author     Shunya Core Team
  * @brief      Header file containing functions prototypes for pcf8591 ADC 
  *                     library
  *         
  @verbatim
 
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

#define PCF8591_I2C_ADDRESS 0x48


#define A1 0x01 
#define A2 0x10 
#define A3 0x11 


#ifdef __cplusplus
extern "C" {
#endif
//initialization functions
void pcf8591Setup();

//read functions
int pcf8591Read(int physicalPin);

#ifdef __cplusplus
}
#endif

