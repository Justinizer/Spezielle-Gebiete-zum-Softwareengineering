#ifndef _SDS011_h_
#define _SDS011_h_

#include "uart.h"

#define SDS011_CMD_SET_DATA_REPORTING_MODE	0x02
#define SDS011_CMD_QUERY_DATA				0x04
#define SDS011_CMD_SET_DEVICE_ID			0x05
#define SDS011_CMD_SET_SLEEP				0x06
#define SDS011_CMD_CHECK_FIRMWARE_VERSION	0x07
#define SDS011_CMD_SET_WORKING_PERIOD		0x08

void sds011_send_command(struct usart_reg *usart, int command, const char *data, char *response);
void sds011_receive_data(struct usart_reg *usart, int *pm2_5, int *pm10);

#endif