#include <string.h>	// For strerror()
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "serial.h"


int main(int argc, char *argv[]) {
	char buffer[20];
	char packet[4] = {0xAA, 0x01, 0x00, 0xFF};
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

	serial_read_string(fd, buffer, 20);
	printf("%s\n", buffer);

	serial_close(fd);

	return 0;
}