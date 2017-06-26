//	Includes ------------------------------------------
#include "..\..\global.h"
#include "..\..\hardware\hardware.h"
#include "..\..\TIME\TIME_iface.h"
#include "i2c_iface.h"

//
//unsigned char adc_CFG[4] = {CH0_S14_G1, CH1_S14_G1, CH2_S14_G1, CH3_S14_G1};

void InitI2C(void);
void adc_setCFG(void);
void adc_readRESULT(void);

void InitI2C(void)
{
	/* SERCOM clock and peripheral bus clock initialization */
	// SERCOM3-Bus-Clock Enable
	REG_PM_APBCMASK |= PM_APBCMASK_SERCOM3;
	// SERCOM3-Core-Clock Enable
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN + GCLK_CLKCTRL_GEN_GCLK1 + GCLK_CLKCTRL_ID(SERCOM3_GCLK_ID_CORE);
	while(REG_SERCOM3_USART_STATUS & SERCOM_USART_SYNCBUSY_CTRLB);

//
	// Multiplexing for PORT_I2C_SDA[PAD0] + PORT_I2C_CLK[PAD1]
	//PMUX(2) ==> the peripheral functions C for SERCOM3
	REG_PORT_WRCONFIG0=	PORT_WRCONFIG_HWSEL + PORT_WRCONFIG_WRPINCFG + PORT_WRCONFIG_WRPMUX + PORT_WRCONFIG_PMUX(2) + 
						PORT_WRCONFIG_PMUXEN + PORT_WRCONFIG_PINMASK(((PORT_I2C_SCL) + (PORT_I2C_SDA))>>16);
//
	// I2C-Master MODE(5)
	// SCL - LowTimeOut: if SCL=> Low, then Stop condition after 25ms
	// I2C-Bus inactive Time Out
	REG_SERCOM3_I2CM_CTRLA = SERCOM_I2CM_CTRLA_MODE(5) + SERCOM_I2CM_CTRLA_SPEED(STANDARD_MODE_FAST_MODE); // + SERCOM_I2CM_CTRLA_LOWTOUTEN + SERCOM_I2CM_CTRLA_INACTOUT(3); //
	REG_SERCOM3_I2CM_CTRLB = SERCOM_I2CM_CTRLB_SMEN;
	while (REG_SERCOM3_I2CM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP);
//
	// 250 kBaud for 16 MHz
	// BAUD = Fref /2*Fbaud - 1
	REG_SERCOM3_I2CM_BAUD = 31;
	while (REG_SERCOM3_I2CM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP);
//
	/* I2CM-Enable */
	REG_SERCOM3_I2CM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE;
	while (REG_SERCOM3_I2CM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP);
//
	/* bus state is forced into idle state */
	SERCOM3->I2CM.STATUS.bit.BUSSTATE = 0x1;
	while (REG_SERCOM3_I2CM_SYNCBUSY & SERCOM_I2CM_SYNCBUSY_SYSOP);
//
	/* Both master on bus and slave on bus interrupt is enabled */
	SERCOM3->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB;
	/* SERCOM3 handler enabled */
	NVIC->ISER[0] = (uint32_t)(1 << (SERCOM3_IRQn & 0x0000001f));
//
}



// ---- SERCOM3 I2C handler ------------
void SERCOM3_Handler(void)
{
	/* Master on bus interrupt checking */
	if (SERCOM3->I2CM.INTFLAG.bit.MB)
	{
		if (i_i2c_buf == i2c_buf_lengh)
		{   /* After transferring the last byte stop condition will be sent */
			SERCOM3->I2CM.CTRLB.bit.CMD = 0x3;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			//
			//i2cFLAG.send_tx =0;
		}
		else
		{ /* placing the data from transmitting buffer to DATA register*/
			SERCOM3->I2CM.DATA.reg = i2c_tx_buf[i_i2c_buf++];
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
		}
	}	
	/* Slave on bus interrupt checking */
	if (SERCOM3->I2CM.INTFLAG.bit.SB)
	{
		if (i_i2c_buf >= (i2c_buf_lengh-1))
		{ /* NACK should be sent before reading the last byte */
			SERCOM3->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			SERCOM3->I2CM.CTRLB.bit.CMD = 0x3;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			i2c_rx_buf[i_i2c_buf++] = SERCOM3->I2CM.DATA.reg;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			//
			//i2cFLAG.read_rx =0;
		}
		else
		{
			SERCOM3->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			i2c_rx_buf[i_i2c_buf++] = SERCOM3->I2CM.DATA.reg;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);
			/* sending ACK after reading each byte */
			SERCOM3->I2CM.CTRLB.bit.CMD = 0x2;
			while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP);			
		}
	}
}

//-----------------------------------------

