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

int dht22_get_data(GPIO_TypeDef *gpio, int pin, int *temperature, int *humidity);

#endif /* DHT22_H_ */
