#ifndef _SDS011_H_
#define _SDS011_H_

#include "stm32f1xx_hal.h"


#define SDS011_DATA_PACKET_SIZE	10
#define SDS011_READ_TIMEOUT		1000

#define SDS011_PACKET_HEADER	0xAA
#define SDS011_DATA_CMD			0xC0
#define SDS011_PACKET_TAIL		0xAB



void get_sensor_data(UART_HandleTypeDef *huart, int *pm2_5, int *pm10);


#endif