/*!
 * \author no1wudi
 * \file LIS3DSH_HW.h
 */

#ifndef __LIS3DSH_HW_H
#define __LIS3DSH_HW_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"


#define CS_H 	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
#define CS_L 	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

#define LIS3DSH_Write_Mask 0x00

#define LIS3DSH_Read_Mask 0x80

#define LIS3DSH_Sense_2g 0.06

#define LIS3DSH_Sense_4g 0.12

#define LIS3DSH_Sense_6g 0.18

#define LIS3DSH_Sense_8g 0.24

#define LIS3DSH_Sense_16g 0.73

extern SPI_HandleTypeDef spi_for_lis3dsg;

HAL_StatusTypeDef LIS3DSH_Init_SPI_Bus(void);

unsigned char LIS3DSH_SPI_Read_Write(unsigned char LIS3DSH_Reg);

#endif
