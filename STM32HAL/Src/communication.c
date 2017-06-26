#include "communication.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include <string.h>

#define TRANSMIT_BUFFER_SIZE	35

extern UART_HandleTypeDef huart1;
extern uint8_t pc_command[PC_COMMAND_PACKET_SIZE];


void transmit_data_to_pc(int pm2_5, int pm10, int temperature, int humidity, int moisture) {
	char buffer[TRANSMIT_BUFFER_SIZE];

	snprintf(buffer, TRANSMIT_BUFFER_SIZE, "%d;%d;%d;%d;%d\n", pm2_5, pm10, temperature, humidity, moisture);

	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
}

void send_string(const char *string) {
	HAL_UART_Transmit(&huart1, (uint8_t *)string, strlen(string), 1000);
}

void send_status(uint8_t available) {
	uint8_t buffer[3];

	buffer[0] = available + '0';
	buffer[1] = '\n';
	buffer[2] = '\0';

	HAL_UART_Transmit(&huart1, buffer, 2, 1000);
}
