/*
 * eeprom.c
 *
 *  Created on: Apr 24, 2016
 *      Author: lemz
 */

#include "ring_buffer_in_eeprom.h"

#include <stdint.h>

#include <stm32f4xx_hal_flash.h>
#include <stm32f4xx_hal_flash_ex.h>
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/task.h"

SemaphoreHandle_t xSemaphore;

void * head;
void * tail;

void * page0 = (void *) 0x080C0000;
void * page0_tail = (void *) 0x080DFFFF;
void * page1 = (void *) 0x080E0000;
void * page1_tail = (void *) 0x080FFFFF;

HAL_StatusTypeDef eeprom_erase(uint32_t Sector) {
	taskENTER_CRITICAL();
	HAL_StatusTypeDef rc;
	FLASH_EraseInitTypeDef erase;
	uint32_t error;
	erase.NbSectors = 1;
	erase.Sector = Sector;
	erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	erase.TypeErase = FLASH_TYPEERASE_SECTORS;
	erase.Banks = 0;
	HAL_FLASH_Unlock();
	rc = HAL_FLASHEx_Erase(&erase, &error);
	HAL_FLASH_Lock();
	taskEXIT_CRITICAL();
	return rc;
}

HAL_StatusTypeDef eeprom_init(void) {
	HAL_StatusTypeDef rc;

	rc = eeprom_erase(FLASH_SECTOR_10);
	rc = eeprom_erase(FLASH_SECTOR_11);

	head = page0;
	tail = page0;

	xSemaphore = xSemaphoreCreateMutex();

	return rc;
}

int eeprom_push(uint8_t * c) {
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	HAL_StatusTypeDef rc;
	void * next = head + 1;
	if (next == page1) {
		next = page0;

		rc = eeprom_erase(FLASH_SECTOR_11);

		if (tail >= page1)
			tail = page0;
		if (rc)
		{
			xSemaphoreGive(xSemaphore);
			return rc;
		}
	}

	if (next >= page1_tail) {
		next = page0;
		rc = eeprom_erase(FLASH_SECTOR_10);
		if (tail < page1)
			tail = page1;
		if (rc)
		{

			xSemaphoreGive(xSemaphore);

			return rc;
		}
	}
	// Cicular buffer is full
	if (next == tail)
		return -1; // quit with an error
	HAL_FLASH_Unlock();
	rc = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (unsigned int) head, *c);
	HAL_FLASH_Lock();
	head = next;
	xSemaphoreGive(xSemaphore);
	return rc;
}

int eeprom_pop(uint8_t *data) {
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	// if the head isn't ahead of the tail, we don't have any characters
	if (head == tail)
	{
		xSemaphoreGive(xSemaphore);
		return -1; // quit with an error
	}
	*data = *(unsigned char *) tail;


	void * next = tail + 1;

	if (next >= page1_tail)
		next = page0;

	tail = next;

	xSemaphoreGive(xSemaphore);
	return 0;
}

