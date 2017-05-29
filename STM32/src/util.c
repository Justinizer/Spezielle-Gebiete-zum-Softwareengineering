#include "util.h"

void itoa(int value, char *buffer) {
	int digits = count_digits(value);
	buffer[digits] = '\0';
	digits--;

	while (value > 0) {
		buffer[digits] = '0' + (value % 10);
		digits--;
		value /= 10;
	}
}

int count_digits(int value) {
	int digits = 0;

	while (value > 0) {
		value /= 10;
		digits++;
	}

	return digits;
}