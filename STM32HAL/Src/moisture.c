#include "moisture.h"

uint32_t get_soil_moisture(ADC_HandleTypeDef *hadc) {
	uint32_t value;

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 1000);

	value = HAL_ADC_GetValue(hadc);

	HAL_ADC_Stop(hadc);

	return value;
}