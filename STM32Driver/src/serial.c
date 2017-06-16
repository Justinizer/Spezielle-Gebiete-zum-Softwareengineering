#include "serial.h"
#include <string.h>		// For strlen()
#include <unistd.h>		// For write(), read(), close()
#include <fcntl.h>		// For open()
#include <errno.h>		// For errno
#include <termios.h>	// For tcgetattr(), cfsetispeed(), cfsetospeed(), tcflush(), tcsetattr()

int serial_open(void) {
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
	ssize_t result;

	if (!buffer || buffersize == 0) {
		return 0;
	}


	while ((result = read(fd, buffer, 1)) > 0 && buffersize > 0 && *buffer != '\n') {
		buffer++;
		buffersize--;
	}

	if (result < 0) {
		return result;
	}

	// Replace newline with string terminator
	*buffer = '\0';

	return 1;
}

int serial_write_string(int fd, const char *string) {
	if (!string) {
		return 0;
	}

	return serial_write_buffer(fd, string, strlen(string));
}

int serial_write_buffer(int fd, const char *buffer, size_t buffersize) {
	ssize_t bytes_written = 0;
	ssize_t result;

	if (!buffer || buffersize == 0) {
		return 0;
	}

	do {
		result = write(fd, buffer + bytes_written, buffersize);
		bytes_written += result;

	} while (result < buffersize && result >= 0);

	return result;
}

int serial_close(int fd) {
	return close(fd);
}