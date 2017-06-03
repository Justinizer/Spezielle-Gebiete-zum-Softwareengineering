#include "communication.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include <string.h>

#define TRANSMIT_BUFFER_SIZE	30

extern UART_HandleTypeDef huart1;


void transmit_data_to_pc(int pm2_5, int pm10, int temperature, int humidity) {
	char buffer[TRANSMIT_BUFFER_SIZE];

	snprintf(buffer, TRANSMIT_BUFFER_SIZE, "%d;%d;%d;%d\n", pm2_5, pm10, temperature, humidity);

	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), 1000);
}

void send_string(const char *string) {
	HAL_UART_Transmit(&huart1, (uint8_t *)string, strlen(string), 1000);
}
