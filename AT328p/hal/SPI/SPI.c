//	Includes ------------------------------------------
#include "..\..\global.h"
#include "..\..\hardware\hardware.h"
#include "..\..\TIME\TIME_iface.h"
#include "SPI_iface.h"

void spi_cs_set_Sens(unsigned char sensor);
void spi_cs_reset_Sens(unsigned char sensor);
unsigned char pga113_gain(unsigned char step);

unsigned char spi3w_RWByte(unsigned char Data);

void InitSPI(void);

//-----------------------------------------
//
void InitSPI(void)
{
	// SERCOM0-Bus-Clock Enable
	REG_PM_APBCMASK |= PM_APBCMASK_SERCOM0;

	// SERCOM0-Core-Clock Enable
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN + GCLK_CLKCTRL_GEN_GCLK1 + GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE);

	//while(REG_SERCOM5_USART_STATUS & SERCOM_USART_STATUS_SYNCBUSY);
	while(REG_SERCOM0_USART_STATUS & SERCOM_USART_SYNCBUSY_CTRLB);

	// PORT_SPI_MOSI + PORT_SPI_CLK als Output
	REG_PORT_DIRSET0 |= (PORT_SPI_MOSI) + (PORT_SPI_CLK);

	// Multiplexing von PORT_SPI_MISO + PORT_SPI_MOSI + PORT_SPI_CLK
	//PMUX(2) ==> the peripheral functions C for SERCOM0
	REG_PORT_WRCONFIG0 = PORT_WRCONFIG_WRPINCFG + PORT_WRCONFIG_WRPMUX + PORT_WRCONFIG_PMUX(2) + PORT_WRCONFIG_PMUXEN + 
						PORT_WRCONFIG_PINMASK((PORT_SPI_MISO) + (PORT_SPI_MOSI) + (PORT_SPI_CLK));

	// SPI-Master MODE(3)
	// SPI_Mode 0 ==> CPOL=0, CPHA =0
	// PAD_0 ==> MISO, PAD_2 ==> MOSI, PAD_3 ==> CLK
	// MSB first
	REG_SERCOM0_SPI_CTRLA =	SERCOM_SPI_CTRLA_MODE(3) + SERCOM_SPI_CTRLA_DOPO(1);

	// 250 kBaud for 16 MHz
	// BAUD = Fref /2*Fbaud - 1
	REG_SERCOM0_SPI_BAUD = 31;

	// 8 Data-Bits
	REG_SERCOM0_SPI_CTRLB = SERCOM_SPI_CTRLB_RXEN;

	// SPI-Enable
	REG_SERCOM0_SPI_CTRLA |= SERCOM_SPI_CTRLA_ENABLE;

	// Chip Select as IO-Port with High-Level 
	REG_PORT_OUTSET0 = ((SPI_Sens0) + (SPI_Sens1) + (SPI_Sens2) + (SPI_Sens3));
	REG_PORT_DIRSET0 = ((SPI_Sens0) + (SPI_Sens1) + (SPI_Sens2) + (SPI_Sens3));
}

//-----------------------------------------
// ---- Sensor Select-Deselect ------------
void spi_cs_set_Sens(unsigned char sensor)
{
	switch(sensor)
	{
		case 0: SPI_Sens0_SELECT(); break;
		case 1: SPI_Sens1_SELECT(); break;
		case 2: SPI_Sens2_SELECT(); break;
		case 3: SPI_Sens3_SELECT(); break;
		default: break;
	}
}
//-----------------------------------------
void spi_cs_reset_Sens(unsigned char sensor)
{
	switch(sensor)
	{
		case 0: SPI_Sens0_DESELECT(); break;
		case 1: SPI_Sens1_DESELECT(); break;
		case 2: SPI_Sens2_DESELECT(); break;
		case 3: SPI_Sens3_DESELECT(); break;
		default: break;
	}
}
//-----------------------------------------
// ---- GAIN of PGA-Select ------------
unsigned char pga113_gain(unsigned char step)
{
	unsigned char gain;
	switch(step)
	{
		case 0x02: gain = GAIN_10x; break;
		case 0x04: gain = GAIN_100x; break;
		case 0x06: gain = GAIN_200x; break;
		case 0x00:
		default: gain = GAIN_1x; break;
	}
	return gain;
}
//-----------------------------------------
// dump one byte to SPI and read one Byte
unsigned char spi3w_RWByte(unsigned char Data)
{
	unsigned char Byte = 0;

	while (!(REG_SERCOM0_SPI_INTFLAG & SERCOM_SPI_INTFLAG_DRE));
	SPI_TX_BUFFER = Data;											// send Data

	while (!(REG_SERCOM0_SPI_INTFLAG & SERCOM_SPI_INTFLAG_RXC));
	Byte = SPI_RX_BUFFER;											// receive answer + clearing SPI_INTFLAG_RXC

	return Byte;
}
