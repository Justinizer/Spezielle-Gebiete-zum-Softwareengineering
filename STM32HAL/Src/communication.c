#include "communication.h"

// Buffer for storing command packets received from the pc over usb
uint8_t command_buffer[COMMAND_BUFFER_SIZE];
int received_command_bytes;

extern UART_HandleTypeDef huart1;

void transmit_data_to_pc(int pm2_5, int pm10) {
  char buffer[TRANSMIT_BUFFER_SIZE];

  snprintf(buffer, TRANSMIT_BUFFER_SIZE, "%d;%d\n", pm2_5, pm10);
  //CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer));
  HAL_UART_Transmit(&huart1, buffer, strlen(buffer), 1000);
}

void send_string(const char *string) {
	//CDC_Transmit_FS((uint8_t *)string, strlen(string));
	HAL_UART_Transmit(&huart1, string, strlen(string), 1000);
}
