#ifndef i2c_IFACE_H_
#define i2c_IFACE_H_

#define PORT_I2C_SCL	PORT_PA23
#define PORT_I2C_SDA	PORT_PA22

#define PORT_I2C_ADD1	PORT_PA21
#define PORT_I2C_ADD0	PORT_PA20

#define  I2CM_SEND_ACK()		REG_SERCOM3_I2CM_CTRLB &= ~(SERCOM_I2CM_CTRLB_ACKACT)
#define  I2CM_SEND_NACK()		REG_SERCOM3_I2CM_CTRLB |= SERCOM_I2CM_CTRLB_ACKACT

#define  I2CM_CMD_WRITE_NACK()		REG_SERCOM3_I2CM_CTRLB = SERCOM_I2CM_CTRLB_CMD(2)
#define  I2CM_CMD_READ_ACK()		REG_SERCOM3_I2CM_CTRLB = SERCOM_I2CM_CTRLB_CMD(2) + SERCOM_I2CM_CTRLB_ACKACT
#define  I2CM_CMD_READ_NACK_STOP()	REG_SERCOM3_I2CM_CTRLB = SERCOM_I2CM_CTRLB_CMD(3)

#define I2CM_IDLE()			((REG_SERCOM3_I2CM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) == 0x10) // 
#define I2CM_OWNER()		((REG_SERCOM3_I2CM_STATUS & SERCOM_I2CM_STATUS_BUSSTATE_Msk) == 0x20) //I2C_Master has to do 

#define I2C_DATA_BUFFER 	(REG_SERCOM3_I2CM_DATA)
#define I2C_ADDR_BUFFER 	(REG_SERCOM3_I2CM_ADDR)

#define SLAVE_ADDR				 0xD0	// MPC1428
#define STANDARD_MODE_FAST_MODE  0x0
#define FAST_MODE_PLUS           0X01
#define HIGHSPEED_MODE           0X02
#define BUF_SIZE				 32

unsigned char i2c_rx_buf[BUF_SIZE];
unsigned char i2c_tx_buf[BUF_SIZE];
unsigned char i2c_buf_lengh;
unsigned char i_i2c_buf;

typedef struct
{
	unsigned send_tx :1;		// Data send 
	unsigned read_rx :1;		// Data read
	unsigned bit2 :1;		// 
	unsigned bit3 :1;		// 
	unsigned bit4 :1;		//
	unsigned bit5 :1;
	unsigned bit6 :1;
	unsigned bit7 :1;

} i2c_flg;
i2c_flg i2cFLAG;

//
extern void InitI2C(void);

#endif /*i2c_IFACE_H_*/
