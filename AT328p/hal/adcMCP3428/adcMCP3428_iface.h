#ifndef adcMCP3428_IFACE_H_
#define adcMCP3428_IFACE_H_

// adcMCP3428 Adresse_Pins
#define I2C_ADD1_SET0()			REG_PORT_OUTCLR0 = PORT_I2C_ADD1
#define I2C_ADD0_SET0()			REG_PORT_OUTCLR0 = PORT_I2C_ADD0

// Variable for ADC extern MCP3428
//
unsigned char adc_CFG[4];
unsigned char adc_RESULT[4];
unsigned char adc_i_cfg;

// defines of ADC MCP3428
// Bit[7] - in One-Shot-Mode: Write "1"; Read "0", if Conversion done
// Bit[65] - Channel Number 00, 01, 10, 11
// Bit[4] - Conversion Mode: 1-Continuous, 0-One-Shot Conversion Mode
// Bit[32] - Sample Rate Selection Bit: 00-240 SPS (12 bits); 01 = 60 SPS (14 bits); 10 = 15 SPS (16 bits)
// Bit[10] - Gain Selection Bits: 00 = x1 (Default); 01 = x2; 10 = x4; 11 = x8;
/* MCP3428 in One-Shot Conversion Mode*/
enum
{
	CH0_S14_G0	= 0x84,	
	CH0_S14_G1	= 0x85,
	CH0_S14_G2	= 0x86,
	CH0_S14_G8	= 0x87,

	CH1_S14_G0	= 0xA4,
	CH1_S14_G1	= 0xA5,
	CH1_S14_G2	= 0xA6,
	CH1_S14_G8	= 0xA7,
	
	CH2_S14_G0	= 0xC4,
	CH2_S14_G1	= 0xC5,
	CH2_S14_G2	= 0xC6,
	CH2_S14_G8	= 0xC7,
	
	CH3_S14_G0	= 0xE4,
	CH3_S14_G1	= 0xE5,
	CH3_S14_G2	= 0xE6,
	CH3_S14_G8	= 0xE7,	
	
	CHx_NEXT	= 0xFF
}mpc3428;

//
extern void InitMCP3428(void);

// ADC
extern void measur_adcMCP (void);
extern void adc_setCFG(void);
extern void adc_readRESULT(void);

#endif /*adcMPC3428_IFACE_H_*/
