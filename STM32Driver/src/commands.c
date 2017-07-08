#include "commands.h"
#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>

int set_brightness(const char *serial_device, uint8_t brightness) {
	char packet[] = {COMMAND_PACKET_HEADER, COMMAND_SET_BRIGHTNESS, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (!serial_device) {
		return 1;
	}

	if (brightness > 100) {
		brightness = 100;
	}

	packet[2] = (brightness & 0xFF);

	fd = serial_open(serial_device);
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

int get_data(const char *serial_device, char *buffer, size_t buffersize) {
	char packet[] = {COMMAND_PACKET_HEADER, COMMAND_GET_DATA, 0x00, COMMAND_PACKET_TAIL};
	int fd;

	if (!serial_device || !buffer || buffersize == 0) {
		return 1;
	}

	fd = serial_open(serial_device);
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

int get_value_availability(const char *serial_device) {
	char packet[] = {COMMAND_PACKET_HEADER, COMMAND_GET_STATUS, 0x00, COMMAND_PACKET_TAIL};
	char buffer[3];
	int fd;

	if (!serial_device) {
		return -1;
	}

	fd = serial_open(serial_device);
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

	return (buffer[0] == '1') ? 1 : 0;
}