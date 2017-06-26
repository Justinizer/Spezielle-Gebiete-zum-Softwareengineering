/*
 * hardware.c
 *
 * Created: 25.01.2017
 *  Author: esd
 */

#include "..\global.h"
#include "hardware.h"
#include "..\EVENT\EVENT_iface.h"

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */

// ---------------------------------------------------------------------------------------------------------------
// Clocks setting
// ---------------------------------------------------------------------------------------------------------------
void Clock_Configuration(void)
{
	all_Type Temp;

	Temp.uint32 = REG_SYSCTRL_OSC8M;
	Temp.uint32 &= ~SYSCTRL_OSC8M_PRESC_Msk;
	REG_SYSCTRL_OSC8M = Temp.uint32;			// RC-OSC = 8MHz

	// XOSC enabled, GAIN = 2 <> XOSC <= 8MHz
	// XOSC enabled, GAIN = 4 <> XOSC <= 30MHz
	// System_Clock 16MHz
	REG_GCLK_GENDIV = GCLK_GENDIV_ID(0) + GCLK_GENDIV_DIV(2);
	REG_SYSCTRL_XOSC = SYSCTRL_XOSC_XTALEN + SYSCTRL_XOSC_ENABLE +SYSCTRL_XOSC_GAIN(4);
	
	//REG_SYSCTRL_XOSC = SYSCTRL_XOSC_AMPGC;

	// GCLKGEN1 enabled, using XOSC, als Peripherie Clock, divisionfactor = 2 <> (16 MHz)
	//REG_GCLK_GENDIV = GCLK_GENDIV_ID_GCLK1 + GCLK_GENDIV_DIV(2);
	//REG_GCLK_GENCTRL = GCLK_GENCTRL_ID_GCLK1 + GCLK_GENCTRL_SRC_XOSC + GCLK_GENCTRL_GENEN;
	REG_GCLK_GENDIV = GCLK_GENDIV_ID(1) + GCLK_GENDIV_DIV(2);
	REG_GCLK_GENCTRL = GCLK_GENCTRL_ID(1) + GCLK_GENCTRL_SRC_XOSC + GCLK_GENCTRL_GENEN;

	// GCLKGEN0 enabled, using XOSC
	//REG_GCLK_GENCTRL = GCLK_GENCTRL_ID_GCLK0 + GCLK_GENCTRL_SRC_XOSC + GCLK_GENCTRL_GENEN;
	REG_GCLK_GENCTRL = GCLK_GENCTRL_ID(0) + GCLK_GENCTRL_SRC_XOSC + GCLK_GENCTRL_GENEN;

	// GCLKGEN2 enabled, using XOSC, als WDT Clock, divisionfactor = 16 <> (500 kHz)
	//REG_GCLK_GENDIV = GCLK_GENDIV_ID_GCLK2 + GCLK_GENDIV_DIV(16);
	//REG_GCLK_GENCTRL = GCLK_GENCTRL_ID_GCLK2 + GCLK_GENCTRL_SRC_XOSC + GCLK_GENCTRL_GENEN;

	//// ---------------------------------------------------------------------------------------------------------------
	//// ---------------------------------------------------------------------------------------------------------------
	//// WDT-Core-Clock Enable, using GCLK2 = 500 kHz
	//REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN + GCLK_CLKCTRL_GEN_GCLK2 + GCLK_CLKCTRL_ID_WDT;
	//while(REG_WDT_STATUS);					// Synchronisation ==> Warten
//
}
//
// ---------------------------------------------------------------------------------------------------------------
// IO-Ports Initialization
// ---------------------------------------------------------------------------------------------------------------
void IO_Init(void)
{
	//REG_PORT_WRCONFIG0 = PORT_WRCONFIG_WRPINCFG + PORT_WRCONFIG_INEN + OptTaster_PORT;						// Input Enable
//
	// Input Enable for High-Ports (Px16...Px31)! HWSEL-Bit funktioniert nicht richtig! Darum >>16 schieben.	
	REG_PORT_WRCONFIG1 = PORT_WRCONFIG_HWSEL +PORT_WRCONFIG_WRPINCFG + PORT_WRCONFIG_INEN + PORT_WRCONFIG_PINMASK((BUTTON_INP)>>16);// - 16));	
	//REG_PORT_PINCFG1 = (PORT_PINCFG_OFFSET+30) + PORT_PINCFG_INEN;	//NOK
	//REG_PORT_WRCONFIG1 = 0xC0024000;					//OK
	//PORT->Group[1].PINCFG[30].reg = PORT_PINCFG_INEN;	//OK
	
	 
//
	//REG_PORT_WRCONFIG1 = PORT_WRCONFIG_WRPINCFG + PORT_WRCONFIG_DRVSTR + Telemetry_PORT;					// high current
	REG_PORT_DIRSET1 |= (MUX_LED_SHCP) + (MUX_LED_STCP);													// set as Output

	//REG_PORT_OUTSET0 = EEPROM_CS;
	//REG_PORT_DIRSET0 |= EEPROM_CS + Test_Punkt_1 + Test_Punkt_2;		// set as Output
	REG_PORT_DIRSET0 |= (MUX_LED_DS) + (EXT_GPIO);							// set as Output
}
//
//void WDT_Init(void)
//{
	//REG_WDT_CONFIG = WDT_CONFIG_PER_16K;
	//Switch_on_Watchdog();
//}
//
// ---------------------------------------------------------------------------------------------------------------
// LEDs-Change after Power-ON-Reset
// ---------------------------------------------------------------------------------------------------------------
void LEDs_change(void)
{
	switch (timePOR)
	{
		case 8:
		case 4:
		rowLEDs0 = 0x0F;
		rowLEDs1 = 0x09;
		rowLEDs2 = 0x09;
		rowLEDs3 = 0x0F;
		break;
		
		case 7:
		case 3:
		rowLEDs0 = 0x06;
		rowLEDs1 = 0x09;
		rowLEDs2 = 0x09;
		rowLEDs3 = 0x06;
		break;
		
		case 6:
		case 2:
		rowLEDs0 = 0x00;
		rowLEDs1 = 0x06;
		rowLEDs2 = 0x06;
		rowLEDs3 = 0x00;
		break;
		
		default:
		rowLEDs0 = 0x00;
		rowLEDs1 = 0x00;
		rowLEDs2 = 0x00;
		rowLEDs3 = 0x00;
		break;
	}
}
//
// ---------------------------------------------------------------------------------------------------------------
// LEDs-Field and Taster MUX
// ---------------------------------------------------------------------------------------------------------------
void LEDmux(void)
{
unsigned char	i, tempLEDs;

	if (++stateMUX > 3) {stateMUX =0;}

	switch (stateMUX) 
	{
		
		case 0:
			tempLEDs = rowLEDs0;
			tempLEDs |= 0xF0;		// Data_Mask Low-Nibble in 74HC595
			tempLEDs &= ~(0x10);	// MUX0 (Clear Bit 4 in 74HC595)				
			Button_Check(&sBUT3);	// actual is MUX3, only after End of function is MUX0			
			break;

		case 1:
			tempLEDs = rowLEDs1;	
			tempLEDs |= 0xF0;	// Data_Mask Low-Nibble in 74HC595
			tempLEDs &= ~(0x20);	// MUX1 (Clear Bit 5 in 74HC595)
			Button_Check(&sBUT0);	// actual is MUX0, only after End of function is MUX1
			break;

		case 2:
			tempLEDs = rowLEDs2;
			tempLEDs |= 0xF0;	// Data_Mask Low-Nibble in 74HC595
			tempLEDs &= ~(0x40);	// MUX2 (Clear Bit 6 in 74HC595)
			Button_Check(&sBUT1);	// actual is MUX1, only after End of function is MUX2
			break;

		case 3:
			tempLEDs = rowLEDs3;
			tempLEDs |= 0xF0;	// Data_Mask Low-Nibble in 74HC595
			tempLEDs &= ~(0x80);	// MUX3 (Clear Bit 7 in 74HC595)
			Button_Check(&sBUT2);	// actual is MUX2, only after End of function is MUX3
			break;

		default:
			tempLEDs = 0xF0;	// All LEDs off
			break;
	}

	// Data-Shift serial Input for 74HC595
	for (i = 0; i < 8; i++)
	{
		// Set Data for serial input
		if (tempLEDs & 0x80) {MUX_LED_Data_SET();}
		else {MUX_LED_Data_CLR();}

		MUX_LED_Clock_SET();	// Clock Serial
		__NOP();
		__NOP();
		__NOP();
		__NOP();
		MUX_LED_Clock_CLR();
		tempLEDs <<= 1;				// Shift right next Bit
	}
	MUX_LED_Data_CLR();
	
	// Latch-Clock to parallel output
	MUX_LED_Latch_SET();		
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	MUX_LED_Latch_CLR();	
}
//
// ---------------------------------------------------------------------------------------------------------------
// Button Check
// ---------------------------------------------------------------------------------------------------------------
void Button_Check(sBUTTON *sTaster)
{
	if (BUTTON_PUSH())
	{
		if (!(sTaster->push))
		{
			if (++sTaster->countA >= BUT_STROK) 
			{
				sTaster->push = 1;
				//sTaster->state = NOT_CHNG;		// Clear in Menus
			}		
		}
	} 
	else if(sTaster->push)
	{
		if (--sTaster->countA == 0) sTaster->push = 0;		
	}				
}
//
// ---------------------------------------------------------------------------------------------------------------
// Button State: State of the Keys and Set the Sens Menu
// ---------------------------------------------------------------------------------------------------------------
void Button_State(sBUTTON *sTast, unsigned char i)
{
	//unsigned char	temp_key;
	//temp_key = sTast->state;	// old state of key
	
	//---- Button status ------------------------------------
	if (sTast->push)
	{
		if (!(sTast->shot_on))
		{
			if ( ++sTast->countB >= BUT_SHOT)
			{
				sTast->shot_on = 1;
				sTast->state = SHOT_ON;
			}
		}
		else if(!(sTast->long_on))
		{
			if (++sTast->countB >= BUT_LONG) 
			{
				sTast->long_on = 1;
				sTast->state = LONG_ON;
			}
		}
	}
	else if(sTast->countB)
	{
		if (sTast->long_on) sTast->state = LONG_OFF;	// LONG_OFF not used
		else if (sTast->shot_on) sTast->state = SHOT_OFF;
		else sTast->state = NOT_CHNG;

		sTast->countB =0;
		sTast->shot_on =0;
		sTast->shot_off =0;
		sTast->long_on =0;
	}
	//
	//---- SET Menu for Sensors ------------------------------------
		if (sTast->state == LONG_ON)
		{			
			mSens[i] ^= SENS_ACTIV;	
			mSens[i] |= SENS_CHNG;		
			
			// LEDs
			if (mSens[i] & SENS_ACTIV)
			{
				if (i==0) LED00_SET();
				else if (i==1) LED10_SET();
				else if (i==2) LED20_SET();
				else LED30_SET();
			} 
			else
			{
				mSens[i] &= ~SENS_GAIN; 
				if (i==0) rowLEDs0 &= ~0x07;
				else if (i==1)  rowLEDs1 &= ~0x07;
				else if (i==2)  rowLEDs2 &= ~0x07;
				else  rowLEDs3 &= ~0x07;
			}

			EVENT_SET(EVENT_set_Sensor);
			sTast->state =0;
		}
		else if (sTast->state == SHOT_OFF)
		{
			if (mSens[i] & SENS_ACTIV)
			{				
				if ((mSens[i] & SENS_GAIN) == SENS_GAIN) mSens[i] &= ~(SENS_GAIN);	// Clear [Bits 1,2] if [Bits 1,2 = 11] 
				else mSens[i] += 0x02;
				
				mSens[i] |= SENS_CHNG;
				//LEDs
				if (i==0){
					rowLEDs0 = ((rowLEDs0 & 0x09) + (mSens[0] & SENS_GAIN));
				}
				else if (i==1) {
					rowLEDs1 = ((rowLEDs1 & 0x09) + (mSens[1] & SENS_GAIN));
				}
				else if (i==2) {
					rowLEDs2 = ((rowLEDs2 & 0x09) + (mSens[2] & SENS_GAIN));
				}
				else {
					rowLEDs3 = ((rowLEDs3 & 0x09) + (mSens[3] & SENS_GAIN));
				}
				//else rowLEDs3 =0;

				EVENT_SET(EVENT_set_Sensor);				
			}
			sTast->state = 0;
		}
 
	//---------------------------------------------------
	//if (temp_key != sTast->state)	// change of Key-State
	//LED33_TGL();
}
//
// ---------------------------------------------------------------------------------------------------------------
// Menu for Sensors
// ---------------------------------------------------------------------------------------------------------------
//void sens_Act(void)
//{
	////unsigned char i;	
	////unsigned char tempBuf[4];
	//
	//
////
	//
//
////
//}
////