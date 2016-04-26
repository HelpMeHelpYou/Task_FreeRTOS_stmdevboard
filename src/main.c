
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include "diag/Trace.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_conf.h"

#include "serial_port/usart.h"
#include "LIS3DSH/LIS3DSH.h"
#include "eeprom_buffer/ring_buffer_in_eeprom.h"

#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_usart.h"




void vApplicationTickHook(void) {
	HAL_IncTick();
}

void vTask_LED_thread(void * pvParameters) {
	(void) pvParameters;

	__HAL_RCC_GPIOD_CLK_ENABLE()
	;

	GPIO_InitTypeDef gpio;

	gpio.Pin = GPIO_PIN_12;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Alternate = 0;

	HAL_GPIO_Init(GPIOD, &gpio);

	while (1) {

		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		vTaskDelay(500);

		taskYIELD();
	}

}

void vTask_USART_thread(void * pvParameters) {
	(void) pvParameters;
	USART_init();
	while (1)
	{
		uint8_t data = 0;
		HAL_StatusTypeDef rc;


		if (usart.Instance->SR &USART_SR_RXNE) {
			usart.Instance->SR = usart.Instance->SR & ~USART_SR_RXNE;
			while (1) {
				uint8_t transmit_data ='1';
				if (eeprom_pop(&transmit_data))
					break;
				HAL_USART_Transmit(&usart, &transmit_data, sizeof(transmit_data), 100);

			}

		}

		taskYIELD();
	}

}

void vTask_ACC_thread(void * pvParameters) {

	(void) pvParameters;

	LIS3DSH_Init();


	volatile float x, y, z;
	char str[100];
	char str2[100];
	while (1) {

		vTaskDelay(500); //delay 100ms

		x = LIS3DSH_Get_X_Out(LIS3DSH_Sense_2g);
		y = LIS3DSH_Get_Y_Out(LIS3DSH_Sense_2g);
		z = LIS3DSH_Get_Z_Out(LIS3DSH_Sense_2g);

		sprintf(str, "%d, %d, %d", ((int) (x * 1000)), (int) (y * 1000),
				(int) (z * 1000));

		for (int i = 0; str[i] != 0; i++) {
			eeprom_push((unsigned char *)&str[i]);
		}
		strncpy(str2, (void *) 0x080C0000, 100);

		taskYIELD();
	}

}

int main(int argc, char* argv[]) {
	(void) argc;
	(void) argv;

	eeprom_init();
	xTaskCreate(&vTask_LED_thread, "vTask_LED_poll", 1536, NULL, 4, NULL);
	xTaskCreate(&vTask_ACC_thread, "vTask_ACC_poll", 1536, NULL, 4, NULL);
	xTaskCreate(&vTask_USART_thread, "vTask_USART_poll", 1536, NULL, 4, NULL);

	vTaskStartScheduler();

	for (;;)
		;
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
	(void) pcTaskName;
	(void) pxTask;

	for (;;)
		{;}
}

// ----------------------------------------------------------------------------
