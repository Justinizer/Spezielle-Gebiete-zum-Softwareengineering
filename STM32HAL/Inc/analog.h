#ifndef _ANALOG_H_
#define _ANALOG_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"

void get_analog_values(uint32_t *moisture, uint32_t *light);

#endif