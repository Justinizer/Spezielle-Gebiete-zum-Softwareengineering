#ifndef TIME_IFACE_H_
#define TIME_IFACE_H_

#define TIME_SEK()			SekCounter
#define CLOCK_SetTime(x)	(SekCounter = (unsigned long)x)
#define CLOCK_GetTime(x)	(x = SekCounter)
#define CLOCK_Inc() 		SekCounter++

#define wait_ms(x)			DelayMS = x; while(DelayMS)
#define DELAY_ms(x)			DelayMS = x; while(DelayMS)

extern volatile unsigned int DelayMS;
extern volatile unsigned long SekCounter;

extern volatile unsigned int TT_MS_Counter;
extern volatile unsigned int TT_SEK_Counter;
extern volatile unsigned int TT_MIN_Counter;


typedef struct
{
	unsigned TT_10ms:1;
	unsigned TT_20ms:1;
	unsigned TT_100ms:1;
	unsigned TT_200ms:1;
	unsigned TT_500ms:1;
	unsigned TT_1Sek:1;
	unsigned TT1H:1;
	unsigned TT1MIN:1;
} TimeFlags;

extern volatile TimeFlags sTimeFlags;
// declare functions..

extern void TIME_RefreshTime(void);
//extern void CLOCK_Convert(unsigned long lSekCounter);

typedef struct
{
	unsigned char Sek;
	unsigned char Min;
	unsigned char Hrs;
	unsigned char Day;
	unsigned char Month;
	unsigned int Year;
	unsigned LeapYear:1;
	unsigned isSyncronised:1;
} sTIME;

extern sTIME CLOCK_Time;

#endif /*TIME_IFACE_H_*/
