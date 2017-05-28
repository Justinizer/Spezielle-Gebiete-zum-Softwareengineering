.cpu cortex-m3
.thumb
.syntax unified

.global _start
_start:
//.text
.word	0x20005000	/* top address of stack */

/* interrupt vector table -> ST32F103C8 Reference manual.pdf page 203 */
.word	_reset_irq_handler		/* Reset */
.word	bad_irq_loop			/* NMI */
.word	bad_irq_loop			/* HardFault */
.word	bad_irq_loop			/* MemManage */
.word	bad_irq_loop			/* BusFault */
.word	bad_irq_loop			/* UsageFault */
.word	0						/* Reserved */
.word	0						/* Reserved */
.word	0						/* Reserved */
.word	0						/* Reserved */
.word	bad_irq_loop			/* SVCall */
.word	bad_irq_loop			/* Debug Monitor */
.word	0						/* Reserved */
.word	bad_irq_loop			/* PendSV */
.word	bad_irq_loop			/* SysTick */
.word	bad_irq_loop			/* WWDG */
.word	bad_irq_loop			/* PVD */
.word	bad_irq_loop			/* TAMPER */
.word	bad_irq_loop			/* RTC */
.word	bad_irq_loop			/* FLAH */
.word	bad_irq_loop			/* RCC */
.word	bad_irq_loop			/* EXTI0 */
.word	bad_irq_loop			/* EXTI1 */
.word	bad_irq_loop			/* EXTI2 */
.word	bad_irq_loop			/* EXTI3 */
.word	bad_irq_loop			/* EXTI4 */
.word	bad_irq_loop			/* DMA1_Channel1 */
.word	bad_irq_loop			/* DMA1_Channel2 */
.word	bad_irq_loop			/* DMA1_Channel3 */
.word	bad_irq_loop			/* DMA1_Channel4 */
.word	bad_irq_loop			/* DMA1_Channel5 */
.word	bad_irq_loop			/* DMA1_Channel6 */
.word	bad_irq_loop			/* DMA1_Channel7 */
.word	bad_irq_loop			/* ADC1_2 */
.word	bad_irq_loop			/* USB_HP_CAN_TX */
.word	bad_irq_loop			/* USB_LP_CAN_RX0 */
.word	bad_irq_loop			/* CAN_RX1 */
.word	bad_irq_loop			/* CAN_SCE */
.word	bad_irq_loop			/* EXTI9_5 */
.word	bad_irq_loop			/* TIM1_BRK */
.word	bad_irq_loop			/* TIM1_UP */
.word	bad_irq_loop			/* TIM1_TRG_COM */
.word	bad_irq_loop			/* TIM1_CC */
.word	bad_irq_loop			/* TIM2 */
.word	bad_irq_loop			/* TIM3 */
.word	bad_irq_loop			/* TIM4 */
.word	bad_irq_loop			/* I2C1_EV */
.word	bad_irq_loop			/* I2C1_ER */
.word	bad_irq_loop			/* I2C2_EV */
.word	bad_irq_loop			/* I2C2_ER */
.word	bad_irq_loop			/* SPI1 */
.word	bad_irq_loop			/* SPI2 */
.word	bad_irq_loop			/* USART1 */
.word	bad_irq_loop			/* USART2 */
.word	bad_irq_loop			/* USART3 */
.word	bad_irq_loop			/* EXTI15_10 */
.word	bad_irq_loop			/* RTCAlarm */
.word	bad_irq_loop			/* USBWakeup */

/* start and end address for section bss. see also linker script */
.word __bss_start
.word __bss_end
//.word _sdata
//.word _edata

.thumb_func
bad_irq_loop:
	b bad_irq_loop

.thumb_func
_reset_irq_handler:

// Copy initialize data from flash to ram
/*	ldr r0, =_sdata
	ldr r1, =_edata

_data_copy_loop:
	cmp r0, r1
	beq _init_bss

	b _data_copy_loop*/

// Initialize section bss with 0
_init_bss:
	ldr r0, =__bss_start	// Load start address of section bss
	ldr r1, =__bss_end		// Load end address of section bss
	movs r2, #0				// Value to store in bss section. MOVS produces an shorter opcode (16 bit) than MOV (32 bit).

_init_bss_loop:
	cmp r0, r1				// Check if we reached the end of section bss
	beq _start_main			// -> end of section bss reached -> go to _start_main
	str r2, [r0], #4		// Store value of r2 at address in r0. After storing, add 4 to the address in register r0
	b _init_bss_loop

// Call main function
_start_main:
	bl main
_forever:
	b _forever
