/*
 * dht22.h
 *
 *  Created on: 03.06.2017
 *      Author: nils
 */

#ifndef DHT22_H_
#define DHT22_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

/**
 * Retrieves the temperature and the humidity from a connected DHT22 sensor.
 * @param gpio: The gpio port of the pin to which the sensor is connected.
 * @param pin: The pin number of the pin to which the sensor is connected.
 * @param temperature [out]: Pointer to a variable in which the temeprature value should be stored. Can be NULL.
 * @param humidity [out]: Pointer to a variable in which the humidity value should be stored. Can be NULL.
 * @return 1 if there was a checksum error. 0 otherwise.
 */
int dht22_get_data(GPIO_TypeDef *gpio, int pin, uint16_t *temperature, uint16_t *humidity);

#endif /* DHT22_H_ */
