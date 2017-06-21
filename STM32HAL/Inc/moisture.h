#ifndef _MOISTURE_H_
#define _MOISTURE_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"

uint32_t get_soil_moisture(ADC_HandleTypeDef *hadc);

#endif