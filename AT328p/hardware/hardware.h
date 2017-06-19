#ifndef _hardware_
#define _hardware_

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//--- Port Defines of IONS SAMD21 ---
//-------------------------------------------------------------------
#define EXT_GPIO				PORT_PA28
#define EXT_GPIO_on()			REG_PORT_OUTSET0 = EXT_GPIO
#define EXT_GPIO_off()			REG_PORT_OUTCLR0 = EXT_GPIO
#define EXT_GPIO_tgl()			REG_PORT_OUTTGL0 = EXT_GPIO
//
//-------------------------------------------------------------------
// --- MUX_LEDs & Buttons---
//-------------------------------------------------------------------
#define MUX_LED_DS				PORT_PA27	// Data 74HC595
#define MUX_LED_Data_SET()		REG_PORT_OUTSET0 = MUX_LED_DS
#define MUX_LED_Data_CLR()		REG_PORT_OUTCLR0 = MUX_LED_DS
#define MUX_LED_Data_TGL()		REG_PORT_OUTTGL0 = MUX_LED_DS

#define MUX_LED_SHCP			PORT_PB22	// Clock input 74HC595
#define MUX_LED_Clock_SET()		REG_PORT_OUTSET1 = MUX_LED_SHCP
#define MUX_LED_Clock_CLR()		REG_PORT_OUTCLR1 = MUX_LED_SHCP

#define MUX_LED_STCP			PORT_PB23	// Clock Latch 74HC595
#define MUX_LED_Latch_SET()		REG_PORT_OUTSET1 = MUX_LED_STCP
#define MUX_LED_Latch_CLR()		REG_PORT_OUTCLR1 = MUX_LED_STCP
//
#define BUTTON_INP				PORT_PB30
#define BUTTON_PUSH()			(!(REG_PORT_IN1 & BUTTON_INP))
//
// LEDS row_0
#define LED00_SET()				rowLEDs0 |= 0x01	// LED00 on
#define LED00_CLR()				rowLEDs0 &= ~(0x01)	// LED00 off
#define LED00_TGL()				rowLEDs0 ^= 0x01	// LED00 toggle
#define LED01_SET()				rowLEDs0 |= 0x02	// LED01 on
#define LED01_CLR()				rowLEDs0 &= ~(0x02)	// LED01 off
#define LED01_TGL()				rowLEDs0 ^= 0x02	// LED01 toggle
#define LED02_SET()				rowLEDs0 |= 0x04	// LED02 on
#define LED02_CLR()				rowLEDs0 &= ~(0x04)	// LED02 off
#define LED02_TGL()				rowLEDs0 ^= 0x04	// LED02 toggle
#define LED03_SET()				rowLEDs0 |= 0x08	// LED03 on
#define LED03_CLR()				rowLEDs0 &= ~(0x08)	// LED03 off
#define LED03_TGL()				rowLEDs0 ^= 0x08	// LED03 toggle
// LEDS row_1
#define LED10_SET()				rowLEDs1 |= 0x01	// LED10 on
#define LED10_CLR()				rowLEDs1 &= ~(0x01)	// LED10 off
#define LED10_TGL()				rowLEDs1 ^= 0x01	// LED10 toggle
#define LED11_SET()				rowLEDs1 |= 0x02	// LED11 on
#define LED11_CLR()				rowLEDs1 &= ~(0x02)	// LED11 off
#define LED11_TGL()				rowLEDs1 ^= 0x02	// LED11 toggle
#define LED12_SET()				rowLEDs1 |= 0x04	// LED12 on
#define LED12_CLR()				rowLEDs1 &= ~(0x04)	// LED12 off
#define LED12_TGL()				rowLEDs1 ^= 0x04	// LED12 toggle
#define LED13_SET()				rowLEDs1 |= 0x08	// LED13 on
#define LED13_CLR()				rowLEDs1 &= ~(0x08)	// LED13 off
#define LED13_TGL()				rowLEDs1 ^= 0x08	// LED13 toggle
// LEDS row_2
#define LED20_SET()				rowLEDs2 |= 0x01	// LED20 on
#define LED20_CLR()				rowLEDs2 &= ~(0x01)	// LED20 off
#define LED20_TGL()				rowLEDs2 ^= 0x01	// LED20 toggle
#define LED21_SET()				rowLEDs2 |= 0x02	// LED21 on
#define LED21_CLR()				rowLEDs2 &= ~(0x02)	// LED21 off
#define LED21_TGL()				rowLEDs2 ^= 0x02	// LED21 toggle
#define LED22_SET()				rowLEDs2 |= 0x04	// LED22 on
#define LED22_CLR()				rowLEDs2 &= ~(0x04)	// LED22 off
#define LED22_TGL()				rowLEDs2 ^= 0x04	// LED22 toggle
#define LED23_SET()				rowLEDs2 |= 0x08	// LED23 on
#define LED23_CLR()				rowLEDs2 &= ~(0x08)	// LED23 off
#define LED23_TGL()				rowLEDs2 ^= 0x08	// LED23 toggle
// LEDS row_3
#define LED30_SET()				rowLEDs3 |= 0x01	// LED30 on
#define LED30_CLR()				rowLEDs3 &= ~(0x01)	// LED30 off
#define LED30_TGL()				rowLEDs3 ^= 0x01	// LED30 toggle
#define LED31_SET()				rowLEDs3 |= 0x02	// LED31 on
#define LED31_CLR()				rowLEDs3 &= ~(0x02)	// LED31 off
#define LED31_TGL()				rowLEDs3 ^= 0x02	// LED31 toggle
#define LED32_SET()				rowLEDs3 |= 0x04	// LED32 on
#define LED32_CLR()				rowLEDs3 &= ~(0x04)	// LED32 off
#define LED32_TGL()				rowLEDs3 ^= 0x04	// LED32 toggle
#define LED33_SET()				rowLEDs3 |= 0x08	// LED33 on
#define LED33_CLR()				rowLEDs3 &= ~(0x08)	// LED33 off
#define LED33_TGL()				rowLEDs3 ^= 0x08	// LED33 toggle
// 
unsigned char rowLEDs0, rowLEDs1, rowLEDs2, rowLEDs3;	// Rows for 4x4 LEDs
unsigned char stateMUX;		// state of MUX-Phase
//-------------------------------------------------------------------
// POR - Power On Reset
//---------------------
unsigned char timePOR;	// LEDs change for 8sec.
//-------------------------------------------------------------------
// Buttons
//-------------------------------------------------------------------
#define BUT_STROK	10	// Keystroke, Button is push, in 4ms.Ticks, ca. 40ms
#define BUT_SHOT	20	// Shot push of Buttons, in 20ms.Ticks, ca.0,4s
#define BUT_LONG	100	// Shot push of Buttons, in 20ms.Ticks, ca. 2s

enum 
{
	NOT_CHNG	= 0,		// Button not change
	SHOT_ON		= 1,		// Button let go on
	SHOT_OFF	= 2,		// Button shot let go off
	LONG_ON		= 3,		// Button long let go on
	LONG_OFF	= 4			// Button long let go off

	//SHOT_LAST	=7		// 
}Tast;

typedef struct
{
	unsigned push :1;		// Button Push
	unsigned shot_on :1;	// Shot rise flank
	unsigned shot_off :1;	// Shot fail flank
	unsigned long_on :1;	// Long rise flank
	unsigned bit4 :1;		// 
	unsigned bit5 :1;
	unsigned bit6 :1;
	unsigned bit7 :1;
//
	unsigned char countA;	// Counter for MUX (4ms)
	unsigned char countB;	// Counter for Button state
	unsigned char state;	// State: [0-not change]; [1-shot_on]; [2-shot_off]; [3-long_on]; [4-long_off]; [7-done]; 
} sBUTTON;

sBUTTON sBUT0, sBUT1, sBUT2, sBUT3;
//
//-------------------------------------------------------------------
// Defines for Sensors
//-------------------------------------------------------------------
enum 
{
	SENS_ACTIV	= 0x01,		// [Bit 0] Sens is active
	SENS_GAIN	= 0x06,		// [Bit 1,2] Gain Value, [Bit 3] is undefined
	SENS_CHNG	= 0x10,		// [Bit 4] Sens_modus is change
	
	SENS_LAST	= 0x80		// 
} Sens_Mask;

unsigned char mSens[4];		// Sens_modus
unsigned char adcActive_Sens;

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
//--- Functions -----------------------------------------------------
extern void Clock_Configuration(void);
extern void IO_Init(void);
extern void LEDmux (void);
extern void LEDs_change (void);
extern void Button_Check(sBUTTON *sTaster);
extern void Button_State(sBUTTON *sTast, unsigned char i);
//
//
//void EVSYS_Init(void);

#endif
