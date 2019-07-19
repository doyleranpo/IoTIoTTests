/**
  ******************************************************************************
  * @file       template.h
  * @author     Shunya Core Team
  * @brief      Header file contains function prototypes for 
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

#ifndef	__TEMPLATE_H__
#define	__TEMPLATE_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addgroup boardLayer 
 * @{
 *
 */

/**
 *  @breif      Initialize board  specific data
 *  @param      *devBoard pointer to the board data structure 
 *  @return     None
 */
extern void rock960Setup (struct boardData *devBoard);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif	/* __TEMPLATE_H__ */

