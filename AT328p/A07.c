/*
 * A07->A06.c
 */ 

#include "global.h"
#include "hardware\hardware.h"
#include "TIME\TIME_iface.h"
#include "HAL\SPI\SPI_iface.h"
#include "HAL\i2c\i2c_iface.h"
#include "EVENT\EVENT_iface.h"
#include "HAL\adcMCP3428\adcMCP3428_iface.h"

//----------
// Tests
unsigned char test_temp1, temp_flag;

//----------

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */

void SysTick_Handler(void)	// SysTick interrupt Handler.
{	
	TIME_RefreshTime();
	
	LEDmux();
//
}
//

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	Clock_Configuration();
	IO_Init();
	
	SysTick_Config(16000);	// 32MHz-Quartz/ DIV=2 -> 1ms SysTick_Handler (Interrupt)

	InitSPI();
	InitI2C();
	InitMCP3428();

	//-------
	// Tests
	//EXT_GPIO_on();
	//rowLEDs0 = 0x01;
	//rowLEDs1 = 0x00;
	//rowLEDs2 = 0x04;
	//rowLEDs3 = 0x08;
	//-------
	//----------
	// Variable
	timePOR = 8;	// LEDs change for 8Sec.
	//----------
	
    __enable_irq();
	while (TRUE) 
    {
        //TODO:: Please write your application code 

	//------------------------------------------------	
	//---- Time_Control ------------------------------	
		if (sTimeFlags.TT_20ms)
		{			
			// ---- Determinate all Keys state ----
			Button_State(&sBUT0, 0);
			Button_State(&sBUT1, 1);
			Button_State(&sBUT2, 2);
			Button_State(&sBUT3, 3);
			//
			//
			//
			//---- Test ----
			//else rowLEDs0 =0;
			//LED31_TGL();
			// --------------
			
			//----------------------------------
			/* Measurement ADC MPC3428, Read and Set */
			//
			if (!timePOR) measur_adcMCP();
			
			//----------------------------------
			//
			sTimeFlags.TT_20ms = 0;
		}

		if (sTimeFlags.TT_200ms)
		{	
			////----------------------------------
			///* Measurement ADC MPC3428, Read and Set */
			////
			//measur_adcMPC ();		
			
			//----------------------------------
			// Test: 
			//EXT_GPIO_tgl();
			// End Test

			sTimeFlags.TT_200ms = 0;
		}

		if (sTimeFlags.TT_1Sek)
		{
			//----------------------------------
			/* LEDs-Change for Power-On-Reset */ 
			if (timePOR)	
			{
				LEDs_change();
				timePOR--;
			}
			//
			////----------------------------------
					
			////----------------------------------
			// Test in Time Control---------------
			//LED30_TGL();
			//EXT_GPIO_tgl();
			//}
			// End_Test--------------------------
			sTimeFlags.TT_1Sek = 0;	
		}
	//---- End of Time_Control -----------------------
	//------------------------------------------------
	//---- Events ------------------------------------
	//
		EVENT_Do_Event();

		
		// Test Events-------------------------
		//if (sBUT1.push)	LED10_SET();
		//else LED10_CLR();
		// End_Test-----------------------------

		
		
	//---- End of Events -----------------------	
	
    }
}
