/*----------------------------------------------------------------
Acess the M24LR04E
----------------------------------------------------------------*/
#include "I2C.h"
#include <intrins.h> 

void I2C_DELAY(void)
{
	_nop_();_nop_();_nop_();_nop_();
}
//----------------------------------------------------------------
// delay 100us
//----------------------------------------------------------------
void mDelay(UINT8 k)
{
	UINT16 i ;
	
	for(; k>0; k--)
	{
		for(i=0; i<1993; i++);
	}
}
//----------------------------------------------------------------
//OK
//----------------------------------------------------------------
void I2C_Start(void)
{
	SDA_HIGH();
	I2C_DELAY();	
	
	SCL_HIGH();
	I2C_DELAY();
	
	SDA_LOW();
	I2C_DELAY();	
		
}
//----------------------------------------------------------------
//OK
//----------------------------------------------------------------
void I2C_Stop(void)
{
	SDA_LOW();
	I2C_DELAY();
	
	SCL_HIGH();
	I2C_DELAY();
	
	SDA_HIGH();
	I2C_DELAY();
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void sendNoAck(void)
{
	SDA_HIGH();
	I2C_DELAY();
	
	SCL_HIGH();
	I2C_DELAY();
	
	SCL_LOW();
	I2C_DELAY();		
}
//----------------------------------------------------------------
// 0 = noACK; 1 = ACK ;
//----------------------------------------------------------------

UINT8 RecvACK(void)
{
	UINT8 data_temp;

	SCL_LOW();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();		
	SDA_InEn();
  data_temp = IOMP1DIN&0x80;
	
	SCL_HIGH(); 
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();	
	SCL_LOW();   
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
	_nop_();_nop_();	
	
	SDA_OutEn();
	
  return data_temp;		
}

//----------------------------------------------------------------
//OK
// a positive clock edge clock a bit into the ROM
//----------------------------------------------------------------
void writeByte(UINT8 datum)
{
	UINT8 bitCnt;
	
	for(bitCnt=0; bitCnt<8; bitCnt++)
	{		
		SCL_LOW();
		I2C_DELAY();
		
		if ((datum&0x80) == 0x80) //if the MSb is 1
		{
			SDA_HIGH();
		}
		else
		{
			SDA_LOW();
		}
		I2C_DELAY();
		
		SCL_HIGH();
		I2C_DELAY();
	  _nop_();_nop_();_nop_();_nop_();_nop_();			
		
		datum<<=1 ; 
	}	
	
	RecvACK();
}

void writeFourBytesToROM(UINT8 datum[], UINT16 address, UINT16 num)
{
	UINT16 i;
	UINT8 *datum_P ;
	datum_P = datum ;
	
	I2C_Start() ;
	
	writeByte(M24LR04_EEPROM_ADDRESS_USER) ;
	
  writeByte((UINT8)(address >> 8)); // MSB 
	
	writeByte((UINT8)(address)); /*address*/	
	
	for(i=0; i<num; i++)
	{
		writeByte(*(datum_P+i)) ;
		
	}
	
	I2C_Stop() ;	
}

/*~~~~~~~~~~~~~~~~~~~~~~~ API ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*-----------------------------------------------------------------
write some bytes to sequential address
TarAddr ��ַ����4�ı����
-----------------------------------------------------------------*/
void writeToROM(UINT8 *pData, UINT16 TarAddr, UINT16 NbByte)
{
	UINT8 align_mem_offset;
  UINT16 bytes_to_write = NbByte;
  UINT16 mem_addr = TarAddr;
  UINT8 *pdata_index = (UINT8 *)pData;	
	
  do
  {
    if( bytes_to_write > M24LR_PAGEWRITE_NBBYTE )
    {
      align_mem_offset = M24LR_PAGEWRITE_NBBYTE - (mem_addr % M24LR_PAGEWRITE_NBBYTE);
    }
    else
    {
      align_mem_offset = bytes_to_write;
    }
    writeFourBytesToROM(pdata_index, mem_addr, align_mem_offset);
		mDelay(10);

    pdata_index += align_mem_offset;
    mem_addr += align_mem_offset;
    bytes_to_write -= align_mem_offset;
		
  }while(bytes_to_write > 0);		
}

/*-----------------------------------------------------------------
read some bytes from ROM`s sequential address
TarAddr ��ַ����4�ı���
-----------------------------------------------------------------*/
void readFromROM(UINT8 datum[], UINT16 address, UINT16 num)
{
	UINT16 i;
	UINT8 *datum_P ;	
	
	UINT8 tempbit = 1 ;
	UINT8 temp = 0 ;
	UINT8 bitCnt ;

	datum_P = datum;
			
	I2C_Start() ;
	
	writeByte(M24LR04_EEPROM_ADDRESS_USER);
	
	writeByte((UINT8)(address >> 8)); // MSB 

	writeByte((UINT8)address); /*address*/
	
	I2C_Start();
	writeByte(M24LR04_EEPROM_ADDRESS_USER+1);
	
	for(i=0; i<num; i++)
	{

		SDA_HIGH(); // release the bus,ready to receive byte
		
		for(bitCnt=0; bitCnt<8; bitCnt++)
		{
			SCL_LOW(); 
			_nop_();			
			
			SDA_InEn();		//����		
			tempbit = (IOMP1DIN&0x80); //SDA ;				
			SDA_OutEn();
			
			if (tempbit)
				temp |= 0x01 ;
			else
				temp &= 0xfe ;
			
			SCL_HIGH();				
		
			if(bitCnt<7)
				temp <<= 1 ;
		
		}
	
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
		_nop_();_nop_();		
		
		SCL_LOW();		
		
		*(datum_P+i) = temp;
		
		SDA_LOW();
		
		SCL_HIGH();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();		
		
		SCL_LOW();
		I2C_DELAY();		
	}
	sendNoAck() ;
	
	I2C_Stop() ;
}

/********************************************************************
IIC Init
********************************************************************/
void IIC_HostInit(void)
{
	vScu_SetSysClkOsc2M();
	
	vScu_IomClkEn();

	SCL_OutEn();			//GP14	
	SCL_PuEn();				//����

	SDA_OutEn();			//GP15
	SDA_PuEn();				//����
}

