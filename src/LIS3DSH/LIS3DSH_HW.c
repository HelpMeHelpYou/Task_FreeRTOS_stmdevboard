/*!
 * \author no1wudi
 * \file LIS3DSH_HW.c
 */


#include "LIS3DSH_HW.h"

SPI_HandleTypeDef spi_for_lis3dsg;

HAL_StatusTypeDef LIS3DSH_Init_SPI_Bus(void){
	SPI_InitTypeDef SPI_FOR_LIS3DSH;
	GPIO_InitTypeDef GPIO_FOR_LIS3DSH;
	HAL_StatusTypeDef rc;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitTypeDef gpio;


	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF5_SPI1 ;

	HAL_GPIO_Init(GPIOA, &gpio);

	
	gpio.Pin = GPIO_PIN_3  ;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = 0 ;

	HAL_GPIO_Init(GPIOE, &gpio);
	

	
	CS_H;

	spi_for_lis3dsg.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	spi_for_lis3dsg.Init.CLKPhase = SPI_PHASE_2EDGE;
	spi_for_lis3dsg.Init.CLKPolarity = SPI_POLARITY_HIGH;
	spi_for_lis3dsg.Init.CRCPolynomial = 7;
	spi_for_lis3dsg.Init.DataSize = SPI_DATASIZE_8BIT;
	spi_for_lis3dsg.Init.Direction = SPI_DIRECTION_2LINES;
	spi_for_lis3dsg.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi_for_lis3dsg.Init.Mode = SPI_MODE_MASTER;
	spi_for_lis3dsg.Init.NSS = SPI_NSS_SOFT;
	spi_for_lis3dsg.Instance = SPI1;

	rc = HAL_SPI_Init(&spi_for_lis3dsg);
	if (rc !=  HAL_OK)
	{

	}
	return rc;

}


unsigned char LIS3DSH_SPI_Read_Write(unsigned char Data){
	char result;
	HAL_SPI_TransmitReceive(&spi_for_lis3dsg,&Data,&result,sizeof(char),100);
	return result;
}

