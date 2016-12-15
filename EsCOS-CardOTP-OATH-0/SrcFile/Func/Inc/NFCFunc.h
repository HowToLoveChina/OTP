#ifndef __I2C_HOST_H__
#define __I2C_HOST_H__

#define __NFC_WITH_BUSY_PIN__
#define __USE_NFC__

#include "typedefine.h"
#include "scufunc.h"

#define M24LR04_EEPROM_ADDRESS_USER			0xA6 /* I2C DeviceSelect */
#define M24LR04_EEPROM_ADDRESS_SYSTEM		0xAE /* I2C DeviceSelect */

#define M24LR_PAGEWRITE_NBBYTE          0x04

#define SCL_PIN			(0x01<<7)	// GPIO15
#define SDA_PIN			(0x01<<6)	// GPIO14
#define BUSY_PIN		(0x01<<5)	// GPIO13

#define SCL_HIGH()  	  	{IOMP1DOUT |= (SCL_PIN);}		
#define SCL_LOW()		  	{IOMP1DOUT &= (~SCL_PIN);}	

#define SDA_HIGH()			{IOMP1DOUT |= (SDA_PIN);}		
#define SDA_LOW()		  	{IOMP1DOUT &= (~SDA_PIN);}	

#define SCL_PuEn()			{IOMP1PU |= SCL_PIN;}
#define SDA_PuEn()			{IOMP1PU |= SDA_PIN;}

#define SCL_OutEn()			{IOMP1DIR |= SCL_PIN;}
#define SDA_OutEn()			{IOMP1DIR |= SDA_PIN;}

#define SDA_InEn()			{IOMP1DIR &= (~SDA_PIN);}
#define BUSY_InEn()			{IOMP1DIR &= (~BUSY_PIN);}

void I2C_DELAY(void);
void mDelay(UINT8 k);
void writeToROM(UINT8 datum[], UINT16 address, UINT16 num);
void readFromROM(UINT8 datum[], UINT16 address, UINT16 num);
void IIC_HostInit(void);

#ifdef __NFC_WITH_BUSY_PIN__
#define NFC_BUSY_ADDR	(0x007F<<2) //508
#define NFC_IS_BUSY		0x11
#define NFC_NOT_BUSY	0x00
#define NFC_FLAG_NUM	0x01
#define NFC_ERROR		0xEF

#define NFC_DATA_EN_ADDR	(0x007E<<2) //504
#define NFC_PC_DATA_EN		0xAA
#define NFC_MCU_DATA_EN		0xBB
#define NFC_DATA_REC		0xCC		// mcu data recevied
#define NFC_DATA_FLAG_LEN	0x01

#define NFC_BUSY_STA		0			// Busy pin low is nfc busy
BOOL Read_NFC_Busy(void);
void Set_NFC_Busy(void);
void Clear_NFC_Busy(void);
void Read_NFC(UINT8 datum[], UINT16 address, UINT16 num);
void Write_NFC(UINT8 *pData, UINT16 TarAddr, UINT16 NbByte);


UINT8 NFC_Busy_Status(void);
void MCU_Data_EN(void);
UINT8 PC_Data_EN(void);
void MCU_Data_Recevied(void);

#endif

#endif
