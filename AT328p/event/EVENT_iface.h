#ifndef EVENT_IFACE_H_
#define EVENT_IFACE_H_

extern unsigned int EVENT_FLAGS;

#define EVENT_RESET(x) (EVENT_FLAGS &= ~x)

#define EVENT_SET(x) (EVENT_FLAGS |= x)

// define generic message, and eventually pointer to needed data
//#define EVENT_SET_MSG(x,y)

//#define EVENT_SET_MSG(x,y) EVENT_Generic_Message = x; EVENT_Generic_Pointer = y

#define EVENT_set_Sensor 		0x0001
#define EVENT_adcMCP3428 		0x0002
//#define EVENT_2 		0x0004
//#define EVENT_3		0x0008
//
//#define EVENT_4		0x0010
//#define EVENT_5		0x0020
//#define EVENT_6		0x0040
//#define EVENT_7		0x0080


//extern unsigned char EVENT_Generic_Message;
//extern unsigned char *EVENT_Generic_Pointer;

// main event function.
extern void EVENT_Do_Event(void);

//extern void (*EVENT_fCallback)(unsigned char isRequestActive);

#endif /*EVENT_IFACE_H_*/
