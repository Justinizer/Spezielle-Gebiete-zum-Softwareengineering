#include "..\global.h"
#include "..\hardware\hardware.h"
#include "..\TIME\TIME_iface.h"
#include "..\HAL\SPI\SPI_iface.h"
#include "EVENT_iface.h"


unsigned int EVENT_FLAGS = 0;

// void (*EVENT_fCallback)(unsigned char isRequestActive);

//unsigned char EVENT_Generic_Message = 0;
//unsigned char *EVENT_Generic_Pointer = 0;
//unsigned char cMSBLock = 0;

// extern const unsigned char SML_PARSE_TEST_TLG1[];

void EVENT_Do_Event(void)
{
	unsigned char i, temp_gain, temp_sens;
	
	if (EVENT_FLAGS & EVENT_set_Sensor) 
	{
		temp_sens =0;
		for (i=0; i<4; i++)
		{
			if (mSens[i] & SENS_CHNG)
			{
				spi_cs_set_Sens(i);
				spi3w_RWByte(CMD_WRITE);
				temp_gain = pga113_gain(mSens[i] & SENS_GAIN);
				spi3w_RWByte(temp_gain);
				spi_cs_reset_Sens(i);
				mSens[i] &= ~(SENS_CHNG);
			}

			if (mSens[i] & SENS_ACTIV) temp_sens |= (1<<i);
			else temp_sens &= ~(1<<i);
		}
		
		adcActive_Sens = temp_sens;	
		if (adcActive_Sens) EVENT_SET(EVENT_adcMCP3428);
		else EVENT_RESET(EVENT_adcMCP3428);

		//LED33_TGL();
		EVENT_RESET(EVENT_set_Sensor);
	}

	//if (EVENT_FLAGS & EVENT_adcMCP3428)
	//{
		//
		//LED33_TGL();
		//EVENT_RESET(EVENT_adcMCP3428);
	//}


		//Switch_on_Watchdog();

}
