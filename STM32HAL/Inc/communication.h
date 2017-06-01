#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "usbd_cdc_if.h"

#define TRANSMIT_BUFFER_SIZE	20
#define COMMAND_BUFFER_SIZE		128

extern uint8_t command_buffer[];
extern int received_command_bytes;

void transmit_data_to_pc(int pm2_5, int pm10);

#endif