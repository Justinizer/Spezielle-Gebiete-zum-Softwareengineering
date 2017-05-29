#ifndef _UART_H_
#define _UART_H_

#include "registers.h"
#include "gpio.h"

#define USART_1_PIN_TX	GPIO_PA9
#define USART_1_PIN_RX	GPIO_PA10
#define USART_1_PIN_CK	GPIO_PA8
#define USART_1_PIN_CTS	GPIO_PA11
#define USART_1_PIN_RTS	GPIO_PA12

#define USART_2_PIN_TX	GPIO_PA2
#define USART_2_PIN_RX	GPIO_PA3
#define USART_2_PIN_CK	GPIO_PA4
#define USART_2_PIN_CTS	GPIO_PA0
#define USART_2_PIN_RTS	GPIO_PA1

#define USART_3_PIN_TX	GPIO_PB10
#define USART_3_PIN_RX	GPIO_PB11
#define USART_3_PIN_CK	GPIO_PB12
#define USART_3_PIN_CTS	GPIO_PB13
#define USART_3_PIN_RTS	GPIO_PB14

// Reference manual - page 823
struct usart_reg {
	volatile unsigned long status;		// Status register
	volatile unsigned long data;		// Data register
	volatile unsigned long baud_rate;	// Baud rate register
	volatile unsigned long control1;	// Control register 1
	volatile unsigned long control2;	// Control register 2
	volatile unsigned long control3;	// Control register 3
	volatile unsigned long gtpr;		// Guard time and prescaler register
};

#define USART_1_ADDR	((struct usart_reg *) 0x40013800)		// Reference manual - page 50
#define USART_2_ADDR	((struct usart_reg *) 0x40004400)		// Reference manual - page 51
#define USART_3_ADDR	((struct usart_reg *) 0x40004800)

// USART control register 1 bits
// Reference manual page 826
#define USART_ENABLE_BIT	13	// Bit 13 in control register 1 (USART_CR1)
#define USART_TX_ENABLE_BIT	3
#define USART_RX_ENABLE_BIT	2

// USART status register bits
// Reference manual page 823
#define USART_TRANSMIT_REGISTER_EMPTY	0b10000000	// Bit 7 TXE: Transmit data register empty
#define USART_TRANSMISSION_COMPLETE		0b01000000	// Bit 6 TC: Transmission complete
#define USART_READ_DATA_REG_NOT_EMPTY	0b00100000	// Bit 5 RXNE: Read data register not empty
#define USART_OVERRUN_ERROR				0b00001000	// Bit 3 ORE: Overrun error
#define USART_NOISE_ERROR_FLAG			0b00000100	// Bit 2 NE: Noise error flag
#define USART_FRAMING_ERROR				0b00000010	// Bit 1 FE: Framing error
#define USART_PARITY_ERROR				0b00000001	// Bit 0 PE: Parity error

void init_usart(struct usart_reg *usart, int baudrate);
void usart_wait_send_buffer(struct usart_reg *usart, const char *buffer, unsigned long len);
void usart_wait_send_string(struct usart_reg *usart, const char *string);
void usart_wait_send_char(struct usart_reg *usart, char character);
char usart_wait_receive_char(struct usart_reg *usart);
void usart_wait_receive_buffer(struct usart_reg *usart, char *buffer, unsigned long len);

#endif