#include "..\global.h"
#include "..\hardware\hardware.h"
#include "TIME_iface.h"

//#include "..\ERROR\ERROR_iface.h"

volatile unsigned long SekCounter = 1;	// Worktime
volatile unsigned int DelayMS;

volatile unsigned int TT_MS_Counter;
volatile unsigned int TT_SEK_Counter;
volatile unsigned int TT_MIN_Counter;

volatile TimeFlags sTimeFlags = { 0, 0, 0, 0, 0, 0, 0, 0 };

// call this function every ms...
// this function refreshes all data in sTimeStructure...

void TIME_RefreshTime(void)
{
	// sTimeFlag Structure handling	
	if (DelayMS) DelayMS--;

	TT_MS_Counter++;	

	if ((TT_MS_Counter % 10) == 0)
		sTimeFlags.TT_10ms = 1;

	if ((TT_MS_Counter % 20) == 0)
		sTimeFlags.TT_20ms = 1;
		//LED32_TGL();
	
	if ((TT_MS_Counter % 100) == 0)
		sTimeFlags.TT_100ms = 1;
	
	if ((TT_MS_Counter % 200) == 0)
		//EXT_GPIO_tgl();
		//LED32_TGL();
		sTimeFlags.TT_200ms = 1;
	
	if (TT_MS_Counter > 999) // 1000 msec, time base.
	{ 
		sTimeFlags.TT_1Sek = 1;
		SekCounter++;
		TT_SEK_Counter++;

		if (TT_SEK_Counter > 59) 
		{
			TT_SEK_Counter = 0;
			sTimeFlags.TT1MIN = 1;
		}

		if (SekCounter % 3600 == 0)
			sTimeFlags.TT1H = 1;

		TT_MS_Counter = 0;
	}	
}



