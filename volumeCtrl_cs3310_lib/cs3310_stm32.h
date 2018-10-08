/**
  * cs3310_stm32.h - Stereo digital volume control, CS3310 driver for STM32 MCU.
  *
  * Written in 2017 by Nattapong W. <https://github.com/jazzpiano1004>.
  *
  * To the extent possible under law, the author(s) have dedicated all
  * copyright and related and neighboring rights to this software to
  * the public domain worldwide. This software is distributed without
  * any warranty.
  *
  */

#ifndef  CS3310_STM32_H
#define  CS3310_STM32_H

/** Define which ARM cortex core that will be used in this api.
  * Note : Un-comment only one type of ARM cortex core which you're using.
  */

//#include "stm32f7xx_hal.h"
#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef SLOPE_DB_GAIN_VS_CODEVALUE 		
#define SLOPE_DB_GAIN_VS_CODEVALUE		2.0
#endif

#ifndef CODEVALUE_OFFSET		
#define CODEVALUE_OFFSET           		192
#endif

/* Struct for CS3310 digital volume control */
typedef struct
{
		GPIO_TypeDef 				*IO_port_mute;
		uint32_t 						IO_pin_mute;
		GPIO_TypeDef 				*IO_port_cs;
		uint32_t 						IO_pin_cs;
		SPI_HandleTypeDef 	s_spi_for_cs3310;
	
}cs3310_TypeDef;

/* function prototype */


/**************************** CS3310 Initialization - start ****************************/
void VolumeControl_CS3310_Init(cs3310_TypeDef *s_volumeCtrl, SPI_HandleTypeDef *User_SPI_ForCS3310, 
																GPIO_TypeDef *User_IO_port_cs, 	 uint32_t User_IO_pin_cs, 
																GPIO_TypeDef *User_IO_port_mute, uint32_t User_IO_pin_mute);
/****************************  CS3310 Initialization - end  ****************************/


/********************************* CS3310 Control - start *******************************/
void VolumeControl_CS3310_Mute(cs3310_TypeDef *s_volumeCtrl);
void VolumeControl_CS3310_Unmute(cs3310_TypeDef *s_volumeCtrl);
void VolumeControl_CS3310_SetGain(cs3310_TypeDef *s_volumeCtrl, float dB_gain_left, float dB_gain_right);
/********************************* CS3310 Control - end *********************************/


#endif
