#include "sds011.h"

void sds011_init(UART_HandleTypeDef *huart) {
	sds011_set_data_reporting_mode(huart, QUERY);
	//sds011_set_mode(huart, SLEEP);
}

void sds011_get_sensor_data(UART_HandleTypeDef *huart, int *pm2_5, int *pm10) {
	uint8_t data[4];

	//sds011_set_mode(huart, WAKEUP);

	data[0] = SDS011_CMD_QUERY_DATA;
	sds011_send_command(huart, data, 1);
	sds011_receive_response(huart, data, 4);

	// TODO: Error handling

    if (pm2_5) {
    	*pm2_5 = (data[1] << 8) | data[0];
	}
	if (pm10) {
    	*pm10 = (data[3] << 8) | data[2];
	}

	//sds011_set_mode(huart, SLEEP);
}

void sds011_set_mode(UART_HandleTypeDef *huart, SDS011_MODE mode) {
	uint8_t data[3];

	data[0] = SDS011_SLEEP_CMD_ID;
	data[1] = 0x01;	// Set mode
	data[2] = mode;

	sds011_send_command(huart, data, 3);
	sds011_receive_response(huart, data, 3);

	// TODO: Error handling
}

void sds011_set_data_reporting_mode(UART_HandleTypeDef *huart, SDS011_REPORTING_MODE mode) {
	uint8_t data[3];

	data[0] = SDS011_CMD_DATA_REPORTING_MODE;
	data[1] = 0x01;	// Set reporting mode
	data[2] = mode;

	sds011_send_command(huart, data, 3);
	sds011_receive_response(huart, data, 3);

	// TODO: Error handling
}

void sds011_send_command(UART_HandleTypeDef *huart, uint8_t *data, int datalen) {
	uint8_t cmd_buffer[19] = { 0 };

	cmd_buffer[0] = SDS011_PACKET_HEADER;
	cmd_buffer[1] = SDS011_CMD_ID;

	int checksum = 0;
	int i;
	for (i = 2; i < 2 + datalen && i < 15; i++) {
		cmd_buffer[i] = *data;
		checksum += *data;
		data++;
	}

	cmd_buffer[15] = 0xFF;
	cmd_buffer[16] = 0xFF;
	checksum += 0xFF + 0xFF;

	cmd_buffer[17] = checksum & 0xFF;
	cmd_buffer[18] = SDS011_PACKET_TAIL;

	HAL_UART_Transmit(huart, cmd_buffer, 19, SDS011_READ_TIMEOUT);
	sds011_receive_response(huart, cmd_buffer, 4);
}

void sds011_receive_response(UART_HandleTypeDef *huart, uint8_t *buffer, int bufferlen) {
	uint8_t receive_buffer[SDS011_DATA_PACKET_SIZE];

    // Wait for message header
    do {
      HAL_UART_Receive(huart, receive_buffer, 1, SDS011_READ_TIMEOUT);
    } while (receive_buffer[0] != SDS011_PACKET_HEADER);

    // Receive rest of message
    HAL_UART_Receive(huart, receive_buffer + 1, SDS011_DATA_PACKET_SIZE - 1, SDS011_READ_TIMEOUT);

    if (receive_buffer[1] != SDS011_DATA_RESPONSE_ID) {
    	return;
    }

    // Check if checksum is correct
    int checksum = 0;
    int i;
    for (i = 2; i < 8; i++) {
    	checksum += receive_buffer[i];
    }

    if (receive_buffer[8] != (checksum & 0xFF)) {
    	return;
    }

    // Copy received data
    for (i = 2; i < 6 && i < 2 + bufferlen; i++) {
    	*buffer = receive_buffer[i];
    	buffer++;
    }
}