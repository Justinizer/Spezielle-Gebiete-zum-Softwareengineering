#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdlib.h>	// For size_t


/**
 * Open a serial connection over /dev/ttyAMA0.
 * @param serial_device Device that should be used for serial communication.
 * @return New file descriptor of the opened serial connection or -1 if an error occurred.
 */
int serial_open(const char *serial_device);

/**
 * Receives a string over a serial connection. Terminates the string with '\0'.
 * @param fd File descriptor of the serial connection.
 * @param buffer Pointer to a buffer in which the string should be stored.
 * @param buffersize Size of the buffer.
 * @return 1 if a string has been successfully received. 0 or -1 otherwise.
 */
int serial_read_string(int fd, char *buffer, size_t buffersize);

/**
 * Sends a string over a serial connection.
 * @param fd File descriptor of the serial connection.
 * @param string The string that should be send.
 * @return The number of written bytes. 0 or -1 if an error occurred.
 */
int serial_write_string(int fd, const char *string);

/**
 * Sends a buffer of characters/values over a serial connection.
 * @param fd File descriptor of the serial connection.
 * @param buffer The buffer containing the values to be send.
 * @para, buffersize Size of the buffer.
 * @return The number of written bytes. 0 or -1 if an error occurred.
 */
int serial_write_buffer(int fd, const char *buffer, size_t buffersize);

/**
 * Closes a serial connection.
 * @param fd File descriptor of the serial connection.
 * @return 0 on success. -1 otherwise.
 */
int serial_close(int fd);

#endif