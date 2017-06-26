#ifndef SPI_IFACE_H_
#define SPI_IFACE_H_

#define PORT_SPI_MISO	PORT_PA08
#define PORT_SPI_MOSI	PORT_PA10
#define PORT_SPI_CLK	PORT_PA11

#define SPI_Sens0				PORT_PA09
#define SPI_Sens0_SELECT()		REG_PORT_OUTCLR0 = SPI_Sens0
#define SPI_Sens0_DESELECT()	REG_PORT_OUTSET0 = SPI_Sens0

#define SPI_Sens1				PORT_PA05
#define SPI_Sens1_SELECT()		REG_PORT_OUTCLR0 = SPI_Sens1
#define SPI_Sens1_DESELECT()	REG_PORT_OUTSET0 = SPI_Sens1

#define SPI_Sens2				PORT_PA06
#define SPI_Sens2_SELECT()		REG_PORT_OUTCLR0 = SPI_Sens2
#define SPI_Sens2_DESELECT()	REG_PORT_OUTSET0 = SPI_Sens2

#define SPI_Sens3				PORT_PA07
#define SPI_Sens3_SELECT()		REG_PORT_OUTCLR0 = SPI_Sens3
#define SPI_Sens3_DESELECT()	REG_PORT_OUTSET0 = SPI_Sens3

#define SPI_TX_BUFFER 	(REG_SERCOM0_SPI_DATA)
#define SPI_RX_BUFFER 	(REG_SERCOM0_SPI_DATA)

// defines of Amplifier PGA113
enum
{
	CMD_READ	= 0x6A,
	CMD_WRITE	= 0x2A,
	
	CHANNEL_0	= 0x00,
	CHANNEL_1	= 0x01,
	CAL1_CH0	= 0x0C,
	CAL2_CH0	= 0x0D,
	CAL3_CH0	= 0x0E,
	CAL4_CH0	= 0x0F,
	
	GAIN_1x		= 0x00,
	GAIN_2x		= 0x10,
	GAIN_5x		= 0x20,
	GAIN_10x	= 0x30,
	GAIN_20x	= 0x40,
	GAIN_50x	= 0x50,
	GAIN_100x	= 0x60,
	GAIN_200x	= 0x70,
	
	PGA_NEXT	= 0xFF
}pga113;

//
extern void InitSPI(void);

// CS for Sens(x)
extern void spi_cs_set_Sens(unsigned char sensor);
extern void spi_cs_reset_Sens(unsigned char sensor);
// GAIN of PGA113
extern unsigned char pga113_gain(unsigned char step);

// write a byte to SPI, and read one.
extern unsigned char spi3w_RWByte(unsigned char Data);

#endif /*SPI_IFACE_H_*/
