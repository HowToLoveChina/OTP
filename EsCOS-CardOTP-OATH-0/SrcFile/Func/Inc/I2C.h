#ifndef __I2C_HOST_H__
#define __I2C_HOST_H__

#include "typedefine.h"
#include "scufunc.h"

#define M24LR04_EEPROM_ADDRESS_USER			0xA6 /* I2C DeviceSelect */
#define M24LR04_EEPROM_ADDRESS_SYSTEM		0xAE /* I2C DeviceSelect */

#define M24LR_PAGEWRITE_NBBYTE          0x04

#define SCL_HIGH()  	{IOMP0DOUT|=	(0x01<<1);}		
#define SCL_LOW()		  {IOMP0DOUT&=(~(0x01<<1));}	

#define SDA_HIGH()  	{IOMP0DOUT|=(0x01<<0);}		
#define SDA_LOW()		  {IOMP0DOUT&=~(0x01<<0);}	

#define SCL_PuEn()			{IOMP0PU|=0x02;}
#define SDA_PuEn()			{IOMP0PU|=0x01;}

#define SCL_OutEn()			{IOMP0DIR|=0x02;}
#define SDA_OutEn()			{IOMP0DIR|=0x01;}

#define SDA_InEn()			{IOMP0DIR&=0xFE;}

void I2C_DELAY(void);
void mDelay(UINT8 k);
void writeToROM(UINT8 datum[], UINT16 address, UINT16 num);
void readFromROM(UINT8 datum[], UINT16 address, UINT16 num);
void IIC_HostInit(void);


#endif
