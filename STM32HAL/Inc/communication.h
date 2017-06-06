#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#define PC_COMMAND_PACKET_SIZE	4

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

void receive_command(const char *data);

#endif
