#ifndef  _RING_H_
#define _RING_H_

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_conf.h>

HAL_StatusTypeDef eeprom_init (void);

int eeprom_push(uint8_t * c);
int eeprom_pop(uint8_t *data) ;

#endif
