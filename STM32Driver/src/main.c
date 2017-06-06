#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int open_serial_connection(void);
int serial_read_string(int fd, char *buffer, size_t buffersize);

int main(int argc, char *argv[]) {
	char buffer[20];
	char packet[4] = {0xAA, 0x01, 0x00, 0xFF};
	int fd;

	fd = open_serial_connection();
	if (fd == -1) {
		printf("Error opening the serial connection: %s\n", strerror(errno));
		return 1;
	}

	if (write(fd, packet, 4) < 0) {
		printf("Error sending data: %s\n", strerror(errno));
		close(fd);
		return 1;
	}

	serial_read_string(fd, buffer, 20);
	printf("%s\n", buffer);

	close(fd);

	return 0;
}

int open_serial_connection(void) {
	int fd;

	fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);
	if (fd == -1) {
		return -1;
	}

	struct termios options;
	tcgetattr(fd, &options);

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	options.c_cflag |=  CS8 | CLOCAL | CREAD;
	options.c_cflag &= ~PARENB;	// No parity bit
	options.c_cflag &= ~CSTOPB;	// 1 stop bit
	options.c_iflag = IGNPAR;	// Ignore parity errors and framing errors
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VMIN] = 1;		// Wait for at least 1 character
	options.c_cc[VTIME] = 0;	// No timeout

	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSAFLUSH, &options);

	return fd;
}

int serial_read_string(int fd, char *buffer, size_t buffersize) {
	if (!buffer || buffersize == 0) {
		return 0;
	}


	while (read(fd, buffer, 1) > 0 && buffersize > 0 && buffer[0] != '\n') {
		buffer++;
		buffersize--;
	}

	// Replace newline with string terminator
	buffer[0] = '\0';

	return 1;
}