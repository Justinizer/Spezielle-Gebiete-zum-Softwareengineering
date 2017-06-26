#include "commands.h"
#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>


int set_brightness(int brightness) {
	char packet[4] = {COMMAND_PACKET_HEADER, COMMAND_SET_BRIGHTNESS, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (brightness < 0) {
		brightness = 0;
	}

	packet[2] = (brightness & 0xFF);

	fd = serial_open();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (serial_write_buffer(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		serial_close(fd);
		return 1;
	}

	serial_close(fd);

	return 0;
}

int get_data(char *buffer, size_t buffersize) {
	char packet[4] = {COMMAND_PACKET_HEADER, COMMAND_GET_DATA, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (!buffer || buffersize == 0) {
		return 1;
	}

	fd = serial_open();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (serial_write_buffer(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		serial_close(fd);
		return 1;
	}

	serial_read_string(fd, buffer, buffersize);
	serial_close(fd);


	return 0;
}

int get_value_availability(char *availability) {
	char packet[4] = {COMMAND_PACKET_HEADER, COMMAND_GET_STATUS, 0x00, COMMAND_PACKET_TAIL};
	char buffer[3];
	int fd;

	fd = serial_open();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (serial_write_buffer(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		serial_close(fd);
		return 1;
	}

	serial_read_string(fd, buffer, 3);
	serial_close(fd);

	*availability = buffer[0];

	return 0;
}