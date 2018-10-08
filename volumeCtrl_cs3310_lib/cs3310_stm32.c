/**
  * cs3310_stm32.c - Stereo digital volume control, CS3310 driver for STM32 MCU.
  *
  * Written in 2018 by Nattapong W. <https://github.com/jazzpiano1004>.
  *
  * To the extent possible under law, the author(s) have dedicated all
  * copyright and related and neighboring rights to this software to
  * the public domain worldwide. This software is distributed without
  * any warranty.
  *
  */

#include "cs3310_stm32.h"

static uint8_t GainMapping(float dB_gain)
{
		int16_t binary_data;
		
		binary_data = SLOPE_DB_GAIN_VS_CODEVALUE*dB_gain + CODEVALUE_OFFSET;
		if(binary_data > 0xFF)	binary_data = 0xFF;
		if(binary_data < 0)			binary_data = 0;
	
		return (uint8_t)binary_data; 
}

/************************** CS3310 Initialization - start *****************************/
/**
  * @brief  VolumeControl_CS3310_Init() : This function is used to initialize a API's struct for chip CS3310
  * @param  s_volumeCtrl : 	     pointer of volume control API's struct
  * @param  User_SPI_ForCS3310 : pointer of SPI periphiral for this cs3310 chip
  * @param  User_IO_port_cs : 	 GPIO Port of CS (Chip Select) pin
  * @param  User_IO_pin_cs  : 	 GPIO Pin of CS pin
  * @param  User_IO_port_mute :  GPIO Port of mute pin
  * @param  User_IO_pin_mute :   GPIO Pin of mute pin
  * @retval none
  */
void VolumeControl_CS3310_Init(cs3310_TypeDef *s_volumeCtrl, SPI_HandleTypeDef *User_SPI_ForCS3310, 
																GPIO_TypeDef *User_IO_port_cs,   uint32_t User_IO_pin_cs, 
																GPIO_TypeDef *User_IO_port_mute, uint32_t User_IO_pin_mute)
{
		s_volumeCtrl->IO_port_mute = User_IO_port_mute;
		s_volumeCtrl->IO_pin_mute  = User_IO_pin_mute;
		s_volumeCtrl->IO_port_cs   = User_IO_port_cs;
		s_volumeCtrl->IO_pin_cs    = User_IO_pin_cs;
		s_volumeCtrl->s_spi_for_cs3310 = *User_SPI_ForCS3310;
}
/*************************** CS3310 Initialization - end ******************************/


/************************** CS3310 Control - start *****************************/
/**
  * @brief  VolumeControl_CS3310_Mute() : This function is used to mute a chip CS3310
  * @param  s_volumeCtrl : pointer of volume control API's struct
  * @retval none
  */
void VolumeControl_CS3310_Mute(cs3310_TypeDef *s_volumeCtrl)
{
		HAL_GPIO_WritePin(s_volumeCtrl->IO_port_mute, s_volumeCtrl->IO_pin_mute, GPIO_PIN_RESET);
}

/**
  * @brief  VolumeControl_CS3310_Unmute() : This function is used to un-mute a chip CS3310
  * @param  s_volumeCtrl : pointer of volume control API's struct
  * @retval none
  */
void VolumeControl_CS3310_Unmute(cs3310_TypeDef *s_volumeCtrl)
{
		HAL_GPIO_WritePin(s_volumeCtrl->IO_port_mute, s_volumeCtrl->IO_pin_mute, GPIO_PIN_SET);
}

/**
  * @brief  VolumeControl_CS3310_SetGain() : This function is used to set gain chip CS3310
  * @param  s_volumeCtrl  : volume control API's struct
  * @param  dB_gain_left  : Gain value (in dB) of left channel
  * @param  dB_gain_right : Gain value (in dB) of right channel
  * @retval none
  */
void VolumeControl_CS3310_SetGain(cs3310_TypeDef *s_volumeCtrl, float dB_gain_left, float dB_gain_right)
{
		/* Procedue of "Serial Write" to CS3310 */
		// 1) Enable CS pin by drive I/O to logic = LOW
		// 2) Send write data (8 bits data for each R and L channel of CS3310. Totally, 16 bits data are transmitted)
		// 3) Disable CS pin by drive I/O to logic = HIGH
		
		uint16_t spi_tx_dataBuf;
		uint8_t  *p = (uint8_t *)&spi_tx_dataBuf;
	
		//mapping between user's dB gain and the SPI data for CS3310
		*p 			 = GainMapping(dB_gain_left);
		*(p + 1) = GainMapping(dB_gain_right);
	
		// CS enable
		HAL_GPIO_WritePin(s_volumeCtrl->IO_port_cs, s_volumeCtrl->IO_pin_cs, GPIO_PIN_RESET);
		HAL_Delay(1);
	
		// Data transmit
		HAL_SPI_Transmit(&(s_volumeCtrl->s_spi_for_cs3310), (uint8_t *)&spi_tx_dataBuf, 1, 1000);
		// CS disable 
		HAL_Delay(1);
		HAL_GPIO_WritePin(s_volumeCtrl->IO_port_cs, s_volumeCtrl->IO_pin_cs, GPIO_PIN_SET);
}
/*************************** CS3310 Control - end ******************************/


