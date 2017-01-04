/*----------------------------------------------------------------
Acess the M24LR04E
----------------------------------------------------------------*/
#include "NFCFunc.h"
#include <intrins.h> 


void I2C_DELAY(void)
{
	_nop_();_nop_();_nop_();_nop_();
}
//----------------------------------------------------------------
// delay 
//----------------------------------------------------------------
void mDelay(UINT8 k)
{
	UINT16 i ;
	
	for(; k>0; k--)
	{
		for(i=0; i<78; i++);
	}
}
//----------------------------------------------------------------
//I2C start condition
//----------------------------------------------------------------
void I2C_Start(void)
{
	SDA_HIGH();
//	I2C_DELAY();	
	
	SCL_HIGH();
//	I2C_DELAY();
	
	SDA_LOW();
//	I2C_DELAY();	
		
}
//----------------------------------------------------------------
//I2C stop condition
//----------------------------------------------------------------
void I2C_Stop(void)
{
	SDA_LOW();
//	I2C_DELAY();
	
	SCL_HIGH();
//	I2C_DELAY();
	
	SDA_HIGH();
//	I2C_DELAY();
}

//----------------------------------------------------------------
//
//----------------------------------------------------------------
void sendNoAck(void)
{
	SDA_HIGH();
//	I2C_DELAY();
	
	SCL_HIGH();
//	I2C_DELAY();
	
	SCL_LOW();
//	I2C_DELAY();		
}
//----------------------------------------------------------------
// 0 = noACK; 1 = ACK ;
//----------------------------------------------------------------

UINT8 RecvACK(void)
{
	UINT8 data_temp;

	SCL_LOW();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();		
	SDA_InEn();
    data_temp = IOMP1DIN&SDA_PIN; 
	
	SCL_HIGH(); 
	
  data_temp = ~(IOMP1DIN&SDA_PIN); // GPIO14 ACK LOW active 
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
//	_nop_();_nop_();_nop_();_nop_();_nop_();	
	SCL_LOW(); 

//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//	_nop_();_nop_();	

	SDA_OutEn();
	
  return data_temp;		
}

//----------------------------------------------------------------
// a positive clock edge clock a bit into the ROM
//----------------------------------------------------------------
void writeByte(UINT8 datum)
{
	UINT8 bitCnt;
	
	for(bitCnt=0; bitCnt<8; bitCnt++)
	{		
		SCL_LOW();
		//I2C_DELAY();
		
		if ((datum&0x80) == 0x80) //if the MSb is 1
		{
			SDA_HIGH();
		}
		else
		{
			SDA_LOW();
		}
//		I2C_DELAY();
		
		SCL_HIGH();
//		I2C_DELAY();			
		
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
TarAddr ��ַ����4�ı���
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
//		mDelay(5);

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
		//	_nop_();			
			
			SDA_InEn();		//����		
			tempbit = (IOMP1DIN&0x40); //SDA ;				
			SDA_OutEn();
			
			if (tempbit)
				temp |= 0x01 ;
			else
				temp &= 0xfe ;
			
			SCL_HIGH();				
		
			if(bitCnt<7)
				temp <<= 1 ;
		
		}
	
//		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//		_nop_();_nop_();		
		
		SCL_LOW();		
		
		*(datum_P+i) = temp;
		
		SDA_LOW();
		
		SCL_HIGH();
//		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();	
//		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();		
		
		SCL_LOW();
//		I2C_DELAY();		
	}
	sendNoAck() ;
	
	I2C_Stop() ;
}

/********************************************************************
IIC Init
********************************************************************/
//void IIC_HostInit(void)
void NFC_Init(void)
{
	vScu_SetSysClkOsc2M();
	
	vScu_IomClkEn();

	SCL_OutEn();			//GP14	
	SCL_PuEn();				//����

	SDA_OutEn();			//GP15
	SDA_PuEn();				//����

	BUSY_InEn();		// GP13 
}
/********************************************************************
// Fuction : Read nfc busy status
// Param: void
// Return: TRUE: NFC is busy  
// 		   FALSE: NFC not busy
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
UINT8 Read_NFC_Busy(void)
{
	
	UINT8 busy_flag=0;
		
	readFromROM(&busy_flag, NFC_BUSY_ADDR, NFC_FLAG_NUM);
	
	if(NFC_IS_BUSY == busy_flag)
	{
	
		return TRUE;
	}
	else	
		 return FALSE;
}
/********************************************************************
// Fuction : Set nfc busy status
// Param: void
// Return: void
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
void Set_NFC_Busy(void)
{
  UINT8 busy_flag= NFC_IS_BUSY; 
  writeToROM(&busy_flag, NFC_BUSY_ADDR, NFC_FLAG_NUM);
}
/********************************************************************
// Fuction : Clear nfc busy status
// Param: void
// Return: void
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
void Clear_NFC_Busy(void)
{
  UINT8 busy_flag= NFC_NOT_BUSY; 
  writeToROM(&busy_flag, NFC_BUSY_ADDR, NFC_FLAG_NUM);

}
/********************************************************************
// Fuction : Read nfc busy pin status
// Param: void
// Return: TRUE: NFC is busy  
// 		   FALSE: NFC is not busy
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
UINT8 NFC_Busy_Status(void)
{
  UINT8 nfc_busy_status=0;
  
  nfc_busy_status = (IOMP1DIN&BUSY_PIN); //Busy pin hight is not busy
  
  if(NFC_BUSY_STA == nfc_busy_status)
  	{
	  return TRUE;	// NFC is busy
  	}
  else
  	{
	  return FALSE;	// NFC is not busy
  	}

}
/********************************************************************
// Fuction : Mcu already paper data and send the flag to nfc device
// Param: void
// Return: void
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
void MCU_Data_EN(void)
{
	UINT8 MCU_Data_status= NFC_MCU_DATA_EN; 
  	writeToROM(&MCU_Data_status, NFC_DATA_EN_ADDR, NFC_DATA_FLAG_LEN);
}
/********************************************************************
// Fuction : Read the pc data paper flag
// Param: void
// Return: 
			TRUE: PC aleady paper data
			FALSE:PC is not ready to data 
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
UINT8 PC_Data_EN(void)
{
  UINT8 pc_data_status=0;
  readFromROM(&pc_data_status, NFC_DATA_EN_ADDR, NFC_DATA_FLAG_LEN);// read 0x007E data
  
  if(NFC_PC_DATA_EN == pc_data_status) 
  	{
	  return TRUE;
  	}
  else
  	{
	  return FALSE;		
  	}

}

//void MCU_Data_Recevied(void)
//{

//  UINT8 MCU_Data_status= NFC_DATA_REC; 
//  writeToROM(&MCU_Data_status, NFC_DATA_EN_ADDR, NFC_DATA_FLAG_LEN);
//}

/********************************************************************
// Fuction : Read nfc data
// Param1: [OUT]  Nfc data output  
// Param2: [IN]   read nfc data from  start address
// Param3: [IN]   read nfc data lens
// Return:void
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
void Read_NFC(UINT8 datum[], UINT16 address, UINT16 num)
{
// vScu_SetSysClkOsc2M();
 while(NFC_Busy_Status()); // wait for nfc is not busy
// while(Read_NFC_Busy());
 Set_NFC_Busy();										 
 readFromROM(datum, address, num);
 Clear_NFC_Busy();
}
/********************************************************************
// Fuction : write nfc data
// Param1: [IN]   Nfc data input  
// Param2: [IN]   write nfc data from  start address
// Param3: [IN]   write nfc data lens
// Return:void
// Author: Colin
// Data: 2016/12/9 
********************************************************************/
void Write_NFC(UINT8 *pData, UINT16 TarAddr, UINT16 NbByte)
{	
	//vScu_SetSysClkOsc2M();
	while(NFC_Busy_Status());// wait for nfc is not busy
//  while(Read_NFC_Busy());
	Set_NFC_Busy();
	writeToROM(pData, TarAddr, NbByte);
	MCU_Data_EN();
 	Clear_NFC_Busy();	
}