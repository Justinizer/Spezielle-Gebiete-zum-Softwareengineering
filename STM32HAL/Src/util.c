/*
 * util.c
 *
 *  Created on: 03.06.2017
 *      Author: nils
 */


#include "util.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "core_cm3.h"

static inline int get_micros() {
	return HAL_GetTick() * 1000 + 1000 - SysTick->VAL/48;
}

void delay_us(int us) {
	int curTime = get_micros();
	while (us-(get_micros()-curTime) > 0);
}

