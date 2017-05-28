#ifndef _REGISTERS_H_
#define _REGISTERS_H_

// Reference manual - page 120
// RCC (reset and clock control) registers  (0x4002 1000 - 0x4002 13FF)
struct rcc {
	volatile unsigned long cr;			// Clock control register
	volatile unsigned long cfgr;		// Clock configuration register
	volatile unsigned long cir;			// Clock interrupt register
	volatile unsigned long apb2rstr;	// APB2 peripheral reset register
	volatile unsigned long apb1rstr;	// APB1 peripheral reset register
	volatile unsigned long ahbenr;		// AHB peripheral clock enable register
	volatile unsigned long apb2enr;		// APB2 peripheral clock enable register
	volatile unsigned long apb1enr;		// APB1 peripheral clock enable register
	volatile unsigned long bdcr;		// Backup domain control register
	volatile unsigned long csr;			// Control/status register
};

#define RCC_BASE_ADDR	((struct rcc *) 0x40021000)

// Bits in APB2 peripheral clock enable register
// Reference manual page 112, 113
#define GPIO_A_ENABLE_BIT			2
#define GPIO_B_ENABLE_BIT			3
#define GPIO_C_ENABLE_BIT			4
#define ADC_1_CLOCK_ENABLE_BIT		9
#define ADC_2_CLOCK_ENABLE_BIT		10
#define TIMER_1_CLOCK_ENABLE_BIT	11
#define SPI_1_CLOCK_ENABLE_BIT		12
#define USART_1_CLOCK_ENABLE_BIT	14

// Bits in APB1 peripheral clock enable register
// Reference manual page 114, 115, 116
#define TIMER_2_CLOCK_ENABLE_BIT	0
#define TIMER_3_CLOCK_ENABLE_BIT	1
#define SPI_2_CLOCK_ENABLE_BIT		14
#define USART_2_CLOCK_ENABLE_BIT	17
#define USART_3_CLOCK_ENABLE_BIT	18
#define I2C_1_CLOCK_ENABLE_BIT		21
#define I2C_2_CLOCK_ENABLE_BIT		22
#define USB_CLOCK_ENABLE_BIT		23
#define CAN_CLOCK_ENABLE_BIT		25
#define BACKUP_CLOCK_ENABLE_BIT		27	// BKPEN: Backup interface clock enable
#define POWER_CLOCK_ENABLE_BIT		28	// PWREN: Power interface clock enable

#define enable_apb2_clock(bit) RCC_BASE_ADDR->apb2enr |= (1 << bit);
#define enable_apb1_clock(bit) RCC_BASE_ADDR->apb1enr |= (1 << bit);


struct gpio {
	volatile unsigned long crl;		// Port configuration register low
	volatile unsigned long crh;		// Port configuration register high
	volatile unsigned long idr;		// Port input data register
	volatile unsigned long odr;		// Port output data register
	volatile unsigned long bsrr;	// Port bit set/reset register
	volatile unsigned long brr;		// Port bit reset register
	volatile unsigned long lckr;	// Port configuration lock register
};

#define GPIO_A_BASE_ADDR	((struct gpio *) 0x40010800)	// Reference manual - page 50
#define GPIO_B_BASE_ADDR	((struct gpio *) 0x40010C00)
#define GPIO_C_BASE_ADDR	((struct gpio *) 0x40011000)


#define HIGH	1
#define LOW		0


#endif