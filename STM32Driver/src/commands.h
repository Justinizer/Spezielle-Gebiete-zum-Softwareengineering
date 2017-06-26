#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#define COMMAND_PACKET_HEADER	0xAA
#define COMMAND_PACKET_TAIL		0xFF
#define COMMAND_GET_STATUS		0x00
#define COMMAND_GET_DATA		0x01
#define COMMAND_SET_BRIGHTNESS	0x02


#include <stdlib.h>

int set_brightness(const char *serial_device, int brightness);
int get_data(const char *serial_device, char *buffer, size_t buffersize);
int get_value_availability(const char *serial_device, char *availability);

#endif