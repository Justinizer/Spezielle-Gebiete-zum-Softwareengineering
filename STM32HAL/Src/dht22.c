/*
 * dht22.c
 *
 *  Created on: 03.06.2017
 *      Author: nils
 */

#include "dht22.h"
#include "util.h"

static inline void dht22_set_pin_low(GPIO_TypeDef *gpio, int pin) {
	GPIO_InitTypeDef GPIO_InitStruct;

	// Set pin to output
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;	// Open Drain Mode
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(gpio, &GPIO_InitStruct);
	HAL_GPIO_WritePin(gpio, pin, 0);	// 0: Activate N-MOS -> Connect pin to ground
}

static inline void dht22_set_pin_input(GPIO_TypeDef *gpio, int pin) {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

	HAL_GPIO_Init(gpio, &GPIO_InitStruct);
}

int dht22_get_data(GPIO_TypeDef *gpio, int pin, int *temperature, int *humidity) {
	uint8_t input_buffer[5];

	dht22_set_pin_low(gpio, pin);
	HAL_Delay(10);
	dht22_set_pin_input(gpio, pin);
	delay_us(50);

	// Preamble from sensor
	while (HAL_GPIO_ReadPin(gpio, pin) == 0);
	while (HAL_GPIO_ReadPin(gpio, pin) == 1);

	uint8_t i;
	uint8_t bit = 0;
	uint8_t byte = 0;
	for (i = 0; i < 40; i++) {
		while (HAL_GPIO_ReadPin(gpio, pin) == 1);	// Wait for low level
		while (HAL_GPIO_ReadPin(gpio, pin) == 0);	// Wait for high level

		delay_us(50);

		input_buffer[byte] <<= 1;

		if (HAL_GPIO_ReadPin(gpio, pin)) {
			input_buffer[byte] |= 1;
		}

		bit++;
		if (bit == 8) {
			byte++;
			bit = 0;
		}
	}

	int checksum = 0;
	for (i = 0; i < 4; i++) {
		checksum += input_buffer[i];
	}

	if ((checksum & 0xFF) != input_buffer[4]) {
		if (temperature) { *temperature = 0; }
		if (humidity) { *humidity = 0; }

		return 1;
	}


	if (temperature) {
		*temperature = (input_buffer[2] << 8) | input_buffer[3];
	}

	if (humidity) {
		*humidity = (input_buffer[0] << 8) | input_buffer[1];
	}


	return 0;
}
