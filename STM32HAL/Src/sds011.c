#include "sds011.h"


void get_sensor_data(UART_HandleTypeDef *huart, int *pm2_5, int *pm10) {
	uint8_t receive_buffer[SDS011_DATA_PACKET_SIZE];

    // Wait for message header
    do {
      HAL_UART_Receive(huart, receive_buffer, 1, SDS011_READ_TIMEOUT);
    } while (receive_buffer[0] != SDS011_PACKET_HEADER);

    // Receive rest of message
    HAL_UART_Receive(huart, receive_buffer + 1, SDS011_DATA_PACKET_SIZE - 1, SDS011_READ_TIMEOUT);

    if (receive_buffer[1] != SDS011_DATA_CMD) {
    	return;
    }

    int checksum = 0;
    int i;
    for (i = 2; i < 8; i++) {
    	checksum += receive_buffer[i];
    }

    if (receive_buffer[8] != (checksum & 0xFF)) {
    	return;
    }

    *pm2_5 = (receive_buffer[3] << 8) | receive_buffer[2];
    *pm10 = (receive_buffer[5] << 8) | receive_buffer[4];
}

