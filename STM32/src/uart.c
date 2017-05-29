#include "uart.h"

void init_usart(struct usart_reg *usart, int baudrate) {
	switch ((int)usart) {
		case (int)USART_1_ADDR:
			enable_apb2_clock(GPIO_A_ENABLE_BIT);
			enable_apb2_clock(USART_1_CLOCK_ENABLE_BIT);

			// Set gpios to alternate function. See reference manual page 166
			set_pin_mode(GPIO_A_BASE_ADDR, USART_1_PIN_TX, PIN_MODE_ALT_FUNC_PUSH_PULL);
			set_pin_mode(GPIO_A_BASE_ADDR, USART_1_PIN_RX, PIN_MODE_INPUT_FLOATING);
			break;

		case (int)USART_2_ADDR:
			enable_apb2_clock(GPIO_A_ENABLE_BIT);
			enable_apb1_clock(USART_2_CLOCK_ENABLE_BIT);

			// Set gpios to alternate function. See reference manual page 166
			set_pin_mode(GPIO_A_BASE_ADDR, USART_2_PIN_TX, PIN_MODE_ALT_FUNC_PUSH_PULL);
			set_pin_mode(GPIO_A_BASE_ADDR, USART_2_PIN_RX, PIN_MODE_INPUT_FLOATING);
			break;

		case (int)USART_3_ADDR:
			enable_apb2_clock(GPIO_B_ENABLE_BIT);
			enable_apb1_clock(USART_3_CLOCK_ENABLE_BIT);

			// Set gpios to alternate function. See reference manual page 166
			set_pin_mode(GPIO_B_BASE_ADDR, USART_3_PIN_TX, PIN_MODE_ALT_FUNC_PUSH_PULL);
			set_pin_mode(GPIO_B_BASE_ADDR, USART_3_PIN_RX, PIN_MODE_INPUT_FLOATING);
			break;
	}

	usart->baud_rate = baudrate;
	usart->control1 = (1 << USART_RX_ENABLE_BIT) | (1 << USART_TX_ENABLE_BIT) | (1 << USART_ENABLE_BIT);
}

void usart_wait_send_buffer(struct usart_reg *usart, const char *buffer, unsigned long len) {
	while (len > 0) {
		while ((usart->status & USART_TRANSMIT_REGISTER_EMPTY) == 0);	// Wait for empty transmission register
		usart->data = *buffer;
		buffer++;
		len--;
	}
}

void usart_wait_send_string(struct usart_reg *usart, const char *string) {
	while (*string) {
		while ((usart->status & USART_TRANSMIT_REGISTER_EMPTY) == 0);	// Wait for empty transmission register
		usart->data = *string;
		string++;
	}
}

void usart_wait_send_char(struct usart_reg *usart, char character) {
	while ((usart->status & USART_TRANSMIT_REGISTER_EMPTY) == 0);	// Wait for empty transmission register
	usart->data = character;
}

char usart_wait_receive_char(struct usart_reg *usart) {
	while (!(usart->status & USART_READ_DATA_REG_NOT_EMPTY));	// Wait for full data register
	return usart->data;
}

void usart_wait_receive_buffer(struct usart_reg *usart, char *buffer, unsigned long len) {
	while (len > 0) {
		while (!(usart->status & USART_READ_DATA_REG_NOT_EMPTY));	// Wait for full data register
		*buffer = usart->data;
		buffer++;
		len--;
	}
}