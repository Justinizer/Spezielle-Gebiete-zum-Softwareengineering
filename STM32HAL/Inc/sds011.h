#ifndef _SDS011_H_
#define _SDS011_H_

#include "stm32f1xx_hal.h"


#define SDS011_DATA_PACKET_SIZE			10
#define SDS011_READ_TIMEOUT				1000

#define SDS011_PACKET_HEADER			0xAA
#define SDS011_DATA_RESPONSE_ID			0xC0
#define SDS011_CMD_RESPONSE_ID			0xC5
#define SDS011_CMD_ID					0xB4
#define SDS011_SLEEP_CMD_ID				0x06
#define SDS011_CMD_QUERY_DATA			0x04
#define SDS011_CMD_DATA_REPORTING_MODE	0x02
#define SDS011_PACKET_TAIL				0xAB

typedef enum {
	SLEEP = 0x00,
	WAKEUP = 0x01
} SDS011_MODE;

typedef enum {
	ACTIVE = 0x00,
	QUERY = 0x01
} SDS011_REPORTING_MODE;


void sds011_init(UART_HandleTypeDef *huart);
void sds011_get_sensor_data(UART_HandleTypeDef *huart, int *pm2_5, int *pm10);
void sds011_set_mode(UART_HandleTypeDef *huart, SDS011_MODE mode);
void sds011_set_data_reporting_mode(UART_HandleTypeDef *huart, SDS011_REPORTING_MODE mode);
void sds011_send_command(UART_HandleTypeDef *huart, uint8_t *data, int datalen);
void sds011_receive_response(UART_HandleTypeDef *huart, uint8_t *buffer, int bufferlen);


#endif