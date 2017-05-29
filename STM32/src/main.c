#include "registers.h"
#include "gpio.h"
#include "uart.h"
#include "sds011.h"
#include "util.h"

void main (void) {
	int pm2_5, pm10;
	char pm2_5_string[6];
	char pm10_string[6];

	init_usart(USART_1_ADDR, 833);
	init_usart(USART_2_ADDR, 833);

	while (1) {
		sds011_receive_data(USART_2_ADDR, &pm2_5, &pm10);

		itoa(pm2_5, pm2_5_string);
		itoa(pm10, pm10_string);

		usart_wait_send_string(USART_1_ADDR, "PM 2,5: ");
		usart_wait_send_string(USART_1_ADDR, pm2_5_string);
		usart_wait_send_char(USART_1_ADDR, '\n');
		usart_wait_send_string(USART_1_ADDR, "PM 10: ");
		usart_wait_send_string(USART_1_ADDR, pm10_string);
		usart_wait_send_char(USART_1_ADDR, '\n');
		//usart_wait_send_char(USART_1_ADDR, pm2_5 >> 8);
		//usart_wait_send_char(USART_1_ADDR, pm2_5 & 0xFF);
	}
}
