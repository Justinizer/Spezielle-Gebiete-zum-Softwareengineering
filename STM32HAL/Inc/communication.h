#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#define PC_COMMAND_PACKET_SIZE			4
#define PC_COMMAND_HEADER				0xAA
#define PC_COMMAND_TAIL					0xFF

enum {
	GET_SENSOR_VALUES = 0x01,	// No parameter -> 0x00
	SET_BRIGHTNESS				// Brightness in percent as parameter. Valid values: 0 - 100
} PC_COMMAND_ID;

/*
 * A command packet is constructed as follows:
 * +----+---------------+
 * |0x00| Header (0xAA) |
 * +----+---------------+
 * |0x01|  Command ID   |
 * +----+---------------+
 * |0x02|   Parameter   |
 * +----+---------------+
 * |0x03|  Tail (0xFF)  |
 * +----+---------------+
 *
 */

/**
 * Transmits sensor values over UART to the connected PC.
 * Format: <PM2,5>;<PM10>;<Temperature>;<Humidity>
 * All values must be devided by 10.
 *
 * @param pm2_5: Amount of 2,5µm particles per m³ air.
 * @param pm10: Amount of 10µm particles per m³ air.
 * @param temperature: Temperature in °C.
 * @param humidity: Humidity in %rF.
 */
void transmit_data_to_pc(int pm2_5, int pm10, int temperature, int humidity);

/**
 * Sends a string over UART to the connected PC.
 * @param string: String which should be transmitted.
 */
void send_string(const char *string);

#endif
