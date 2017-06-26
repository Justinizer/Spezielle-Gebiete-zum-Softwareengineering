#include "analog.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

void get_analog_values(uint32_t *moisture, uint32_t *light) {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	*moisture = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 1000);
	*light = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
}