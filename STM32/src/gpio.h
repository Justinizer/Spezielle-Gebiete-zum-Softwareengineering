#ifndef _GPIO_H_
#define _GPIO_H_

#include "registers.h"

#define GPIO_PA0	0
#define GPIO_PA1	1
#define GPIO_PA2	2
#define GPIO_PA3	3
#define GPIO_PA4	4
#define GPIO_PA5	5
#define GPIO_PA6	6
#define GPIO_PA7	7
#define GPIO_PA8	8
#define GPIO_PA9	9
#define GPIO_PA10	10
#define GPIO_PA11	11
#define GPIO_PA12	12
#define GPIO_PA13	13
#define GPIO_PA14	14
#define GPIO_PA15	15

#define GPIO_PB0	0
#define GPIO_PB1	1
#define GPIO_PB2	2
#define GPIO_PB3	3
#define GPIO_PB4	4
#define GPIO_PB5	5
#define GPIO_PB6	6
#define GPIO_PB7	7
#define GPIO_PB8	8
#define GPIO_PB9	9
#define GPIO_PB10	10
#define GPIO_PB11	11
#define GPIO_PB12	12
#define GPIO_PB13	13
#define GPIO_PB14	14
#define GPIO_PB15	15

#define GPIO_PC0	0
#define GPIO_PC1	1
#define GPIO_PC2	2
#define GPIO_PC3	3
#define GPIO_PC4	4
#define GPIO_PC5	5
#define GPIO_PC6	6
#define GPIO_PC7	7
#define GPIO_PC8	8
#define GPIO_PC9	9
#define GPIO_PC10	10
#define GPIO_PC11	11
#define GPIO_PC12	12
#define GPIO_PC13	13
#define GPIO_PC14	14
#define GPIO_PC15	15

// Pin modes. See reference manual page 160
							//     CNFy --v  v-- MODE
#define PIN_MODE_OUTPUT_PUSH_PULL		0b0010
#define PIN_MODE_OUTPUT_OPEN_DRAIN		0b0110
#define PIN_MODE_ALT_FUNC_PUSH_PULL		0b1010 // Max. 2 MHz
#define PIN_MODE_ALT_FUNC_OPEN_DRAIN	0b1110 // Max. 2 MHz
#define PIN_MODE_INPUT_ANALOG			0b0000
#define PIN_MODE_INPUT_FLOATING			0b0100
#define PIN_MODE_INPUT_PULL_DOWN		0b1000	// Clear bit in PxODR for pull-down
#define PIN_MODE_INPUT_PULL_UP			0b1000	// Set bit in PxODR for pull-up


void set_pin_mode(struct gpio *port, int pin, int mode);
void write_pin(struct gpio *port, int pin, int value);

#endif