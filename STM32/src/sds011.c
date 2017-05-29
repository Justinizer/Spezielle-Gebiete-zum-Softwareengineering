#include "sds011.h"

void sds011_send_command(struct usart_reg *usart, int command, const char *data, char *response) {
	usart_wait_send_char(usart, 0xAA);
	usart_wait_send_char(usart, 0xB4);
	usart_wait_send_char(usart, command);
	usart_wait_send_buffer(usart, data, 12);
	usart_wait_send_char(usart, 0xFF);
	usart_wait_send_char(usart, 0xFF);

	int checksum = 0;
	for (int i = 0; i < 12; i++) {
		checksum += *data;
		data++;
	}

	usart_wait_send_char(usart, checksum & 0xFF);
	usart_wait_send_char(usart, 0xAB);

	char buffer;
	buffer = usart_wait_receive_char(usart);
	if (buffer != 0xAA) {
		return;
	}

	buffer = usart_wait_receive_char(usart);	// Command-ID
	usart_wait_receive_buffer(usart, response, 4);
	usart_wait_receive_char(usart);	// ID
	usart_wait_receive_char(usart);	// ID
	usart_wait_receive_char(usart);	// Checksum
	usart_wait_receive_char(usart);	// 0xAB
}

void sds011_receive_data(struct usart_reg *usart, int *pm2_5, int *pm10) {
	char buffer[9];

	while (usart_wait_receive_char(usart) != 0xAA);
	usart_wait_receive_buffer(usart, buffer, 9);

	*pm2_5 = (buffer[3] << 8) | buffer[2];
	*pm10 = (buffer[5] << 8) | buffer[4];
}