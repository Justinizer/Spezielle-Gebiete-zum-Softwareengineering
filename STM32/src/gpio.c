#include "gpio.h"

void set_pin_mode(struct gpio *port, int pin, int mode) {
	int config;

	if (pin > 7) {
		config = port->crh & ~(0b1111 << ((pin - 8) * 4));	// Clear CNFy- and MODEy-bits for pin y
		config |= mode << ((pin - 8) * 4);
		port->crh = config;

	} else {
		config = port->crl & ~(0b1111 << pin);
		config |= mode << (pin * 4);
		port->crl = config;
	}
}

void write_pin(struct gpio *port, int pin, int value) {
	if (value) {	// Set pin
		port->bsrr = (1 << pin);

	} else {		// Clear pin
		port->brr = (1 << pin);
	}
}