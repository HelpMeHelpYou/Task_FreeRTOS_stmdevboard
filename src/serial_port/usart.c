#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_usart.h"

USART_HandleTypeDef usart;

void USART_init(void) {
	GPIO_InitTypeDef gpio;

	__enable_irq();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();

	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_11 | GPIO_PIN_10;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pull = GPIO_PULLUP;
	gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOB, &gpio);

	HAL_NVIC_EnableIRQ(USART3_IRQn);

	usart.Instance = USART3;
	usart.Init.Mode = USART_MODE_RX | USART_MODE_TX;
	usart.Init.BaudRate = 9600;
	usart.Init.WordLength = USART_WORDLENGTH_8B;
	usart.Init.StopBits = USART_STOPBITS_1;
	usart.Init.CLKLastBit = 0;

	HAL_USART_Init(&usart);

}

