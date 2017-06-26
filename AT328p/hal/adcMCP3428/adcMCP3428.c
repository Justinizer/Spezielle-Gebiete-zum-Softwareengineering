//	Includes ------------------------------------------
#include "..\..\global.h"
#include "..\..\hardware\hardware.h"
#include "..\..\TIME\TIME_iface.h"
#include "..\i2c\i2c_iface.h"
#include "..\..\EVENT\EVENT_iface.h"
#include "adcMCP3428_iface.h"

//
unsigned char adc_CFG[4] = {CH0_S14_G1, CH1_S14_G1, CH2_S14_G1, CH3_S14_G1};

void InitMCP3428(void);
void adc_setCFG(void);
void adc_readRESULT(void);

void InitMCP3428(void)
{	
	I2C_ADD1_SET0();
	I2C_ADD0_SET0();	
}

//-----------------------------------------
// Set Config-Register for each ADC-Channel
void measur_adcMCP (void)
{
	unsigned char status_temp=0;
		
	if (EVENT_FLAGS & EVENT_adcMCP3428)
	{
		i2cFLAG.send_tx ^=1;
		/* next Channel */
		if (i2cFLAG.send_tx)
		{
			if (++adc_i_cfg >3) adc_i_cfg =0;
		}

		//
		if (adc_i_cfg ==0) 
		{
			if (adcActive_Sens & 0x01){
				status_temp = 1;
				LED03_TGL();
			}
			else LED03_CLR();
		}
		else if (adc_i_cfg ==1)
		{
			if (adcActive_Sens & 0x02){
				status_temp = 1;
				LED13_TGL();
			}
			else LED13_CLR();
		}		
		else if (adc_i_cfg ==2)
		{
			if (adcActive_Sens & 0x04){
				status_temp = 1;
				LED23_TGL();
			}
			else LED23_CLR();
		}		
		else
		{
			if (adcActive_Sens & 0x08){
				status_temp = 1;
				LED33_TGL();
			}
			else LED33_CLR();
		}
		//
		if (status_temp)
		{
			if (!i2cFLAG.send_tx) adc_readRESULT();
			else  adc_setCFG();
		}		
	}
	else
	{
		LED03_CLR();
		LED13_CLR();
		LED23_CLR();
		LED33_CLR();
	}
}
//-----------------------------------------
// Set Config-Register for each ADC-Channel 
void adc_setCFG(void)
{
	/* MPC3428 Write ConfReg */
	i2c_buf_lengh = 1;
	i_i2c_buf = 0;
	//i2cFLAG.send_tx =1;
	//i2cFLAG.read_rx =1;
	
	///* next Channel */
	//if (++adc_i_cfg >3) adc_i_cfg =0;
	i2c_tx_buf[0] = adc_CFG[adc_i_cfg];
	SERCOM3->I2CM.ADDR.reg = (SLAVE_ADDR);
	
	//LED13_TGL();
}

//-----------------------------------------
// Read Data for each ADC-Channel
void adc_readRESULT(void)
{
	/* MPC3428 Data is 3 Byte -> [D_high, D_low, ConfReg] */
	i2c_buf_lengh = 3;
	i_i2c_buf = 0;
	//i2cFLAG.read_rx =1;
	//i2cFLAG.send_tx =1;

	SERCOM3->I2CM.ADDR.reg = (SLAVE_ADDR+1);
	
	//LED13_TGL();
}
