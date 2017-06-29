#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#define COMMAND_PACKET_HEADER	0xAA
#define COMMAND_PACKET_TAIL		0xFF
#define COMMAND_GET_STATUS		0x00
#define COMMAND_GET_DATA		0x01
#define COMMAND_SET_BRIGHTNESS	0x02


#include <stdlib.h>
#include <stdint.h>

/**
 * Sets the brightness of the connected LED.
 * @param serial_device Path to the device. For example: "/dev/ttyUSB0"
 * @param brightness The brightness in percent that should be set. 
 *        100 = 100% = LED is 100% of the time on
 *        0 = 0% = LED is 0% of the time on -> off.
 *        Valid values: 0 <= brightness <= 100
 * @return 0 if the brightness was set successfully. 1 otherwise.
 */
int set_brightness(const char *serial_device, uint8_t brightness);

/**
 * Sends command to send data to serial_device and reads response. The response is saved to buffer.
 * The newline character is replaced with a string terminator ('\0').
 * @param serial_device Path to the device. For example: "/dev/ttyUSB0"
 * @param buffer Pointer to a buffer in which the response should be saved.
 * @param buffersize Size of the buffer in byte. 
 * @return 0 if the data was successfully received. 1 otherwise.
 */
int get_data(const char *serial_device, char *buffer, size_t buffersize);

/**
 * Checks if there is any data available.
 * @param serial_device Path to the device. For example: "/dev/ttyUSB0"
 * @return 0 if data is not available, 1 if data is available, -1 if there was an error.
 */
int get_value_availability(const char *serial_device);

#endif