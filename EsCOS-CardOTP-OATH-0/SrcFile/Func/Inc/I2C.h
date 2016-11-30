#ifndef __I2C_HOST_H__
#define __I2C_HOST_H__

#define __NFC_WITHOUT_BUSY_PIN__

#include "typedefine.h"
#include "scufunc.h"

#define M24LR04_EEPROM_ADDRESS_USER			0xA6 /* I2C DeviceSelect */
#define M24LR04_EEPROM_ADDRESS_SYSTEM		0xAE /* I2C DeviceSelect */

#define M24LR_PAGEWRITE_NBBYTE          0x04

//#define SCL_HIGH()  	{IOMP0DOUT|=(0x01<<1);}		
//#define SCL_LOW()		  {IOMP0DOUT&=(~(0x01<<1));}	
//
//#define SDA_HIGH()  	{IOMP0DOUT|=(0x01<<0);}		
//#define SDA_LOW()		  {IOMP0DOUT&=~(0x01<<0);}	
//
//#define SCL_PuEn()			{IOMP0PU|=0x02;}
//#define SDA_PuEn()			{IOMP0PU|=0x01;}
//
//#define SCL_OutEn()			{IOMP0DIR|=0x02;}
//#define SDA_OutEn()			{IOMP0DIR|=0x01;}
//
//#define SDA_InEn()			{IOMP0DIR&=0xFE;}

#define SCL_HIGH()  	{IOMP1DOUT|=	(0x01<<(14-8));}		
#define SCL_LOW()		  {IOMP1DOUT&=(~(0x01<<(14-8)));}	

#define SDA_HIGH()  	{IOMP1DOUT|=(0x01<<(15-8));}		
#define SDA_LOW()		  {IOMP1DOUT&=~(0x01<<(15-8));}	

#define SCL_PuEn()			{IOMP1PU|=0x40;}
#define SDA_PuEn()			{IOMP1PU|=0x80;}

#define SCL_OutEn()			{IOMP1DIR|=0x40;}
#define SDA_OutEn()			{IOMP1DIR|=0x80;}

#define SDA_InEn()			{IOMP1DIR&=0x7F;}

void I2C_DELAY(void);
void mDelay(UINT8 k);
void writeToROM(UINT8 datum[], UINT16 address, UINT16 num);
void readFromROM(UINT8 datum[], UINT16 address, UINT16 num);
void IIC_HostInit(void);

#ifdef __NFC_WITHOUT_BUSY_PIN__
#define NFC_BUSY_ADDR	(0x007F<<2) //508
#define NFC_IS_BUSY		0x11
#define NFC_NOT_BUSY	0x00
#define NFC_FLAG_NUM	0x01
#define NFC_ERROR		0xEF

#define NFC_DATA_EN_ADDR	(NFC_BUSY_ADDR+1) //509
#define NFC_PC_DATA_EN		0xAA
#define NFC_MCU_DATA_EN		0xBB
#define NFC_DATA_REC		0xCC		// mcu data recevied
#define NFC_DATA_FLAG_LEN	0x01

BOOL Read_NFC_Busy(void);
void Set_NFC_Busy(void);
void Clear_NFC_Busy(void);
void Read_NFC(UINT8 datum[], UINT16 address, UINT16 num);
void Write_NFC(UINT8 *pData, UINT16 TarAddr, UINT16 NbByte);

void MCU_Data_EN(void);
UINT8 PC_Data_EN(void);
void MCU_Data_Recevied(void);

#endif

#endif
