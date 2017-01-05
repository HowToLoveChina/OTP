/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: UartFunc.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     UartFunc Source Code File;
Function list:
    void vUartInit(void);
    UINT8 ucUartGetByte(void);
    void vUartPutByte(UINT8 Byte); 
	void vUartSendData(UINT8 xdata *OutData,UINT16 Len); 
	void vUartRecvData(UINT8 xdata *InData,UINT16 Len);      
Special statement:
     NULL
 *******************************************************************/
#define _UARTFUNC_C_
#include "IOMFUNC.H"
#include "SCUFUNC.H"
#include "UartFunc.h"
#include "string.H"
#include "SM4.h"
#include "OTPFRAME.H"
#include "UARTFRAME.H"
#include "SM4Process.h"
#include "TimerFunc.h"
#include "LcdFunc.h"
#include "TBCFUNC.H"
#include "NFCFunc.h"

//u1  g_UART_COM_BUF[64];
u1 ReceiveCompleteFlag;
//u1 UARTBufferIndex;
//u1 u1UsartBuffer[50];
u1 u1UsartBuffer[64];

const	u1	code AText[]	={"OCRA-1:HOTP-SHA1-8:QA16-PSHA1-T1M"};
const   u1  code g_u1PriKey[16] = {0x53,0x66,0x47,0xcf,0x5a,0xb0,0x51,0x1d,0x81,0x03,0xfe,0xe9,0x74,0x5f,0x17,0x34};

UINT32 ulGetTbcCountVlaue(void)
{
	u1 u1TBCDR3Cnt;
	u1 u1TBCDR2Cnt;
	u1 u1TBCDR1Cnt;
	u1 u1TBCDR0Cnt;
	u4 u4Temp; 

_START:
	u1TBCDR3Cnt = TBCDR3;
	u1TBCDR2Cnt = TBCDR2;

	if(u1TBCDR3Cnt == TBCDR3)
	{
		u1TBCDR1Cnt = TBCDR1;
		if(u1TBCDR2Cnt == TBCDR2)
		{
			u1TBCDR0Cnt = TBCDR0;
			if(u1TBCDR1Cnt != TBCDR1)
			{
				u1TBCDR0Cnt = TBCDR0;
			}
		}
		else
		{
			goto _START;
		}
	}
	else
	{
		goto _START;
	}

	u4Temp = ((((UINT32)u1TBCDR3Cnt<<24)&0xff000000)|(((UINT32)u1TBCDR2Cnt<<16)&0x00ff0000)|(((UINT32)u1TBCDR1Cnt << 8) & 0x0000FF00) | u1TBCDR0Cnt);
	return u4Temp;		
}

void GETOTPTime(UINT8 *Time)
{
	u2 k;
	u4 u4CrruentTime;
	u1 u1Second;
	u4CrruentTime = Base_Time();	
	//u4CrruentTime = 71563520uL;
	u4CrruentTime += gTimer_Count_Base ;
	u1Second = u4CrruentTime%60;
	u4CrruentTime /=60;
	if(u4CrruentTime>BASE_2011MINUTES)
	{
	  u4CrruentTime -= BASE_2011MINUTES;
	}
	else
	{
		u4CrruentTime = 0x00;
	}
	#if 1
	k	=2011;
	while( u4CrruentTime > 525600ul)
	{
		u4CrruentTime	-= 525600ul;  //365*24*60
		if((k&0x03)==0)
		{
			u4CrruentTime	-=1440ul; //24*60
		}
		k++;
	}
	#endif

	*Time = k%2000;
	*(Time+1) = 1;

	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 2;
	}
	else
	{
		goto __Day;
	}

	if((k&0x03)==0)
	{
		if(u4CrruentTime > 41760ul)
		{
			u4CrruentTime	-=41760ul; //29*24*60
			*(Time+1) = 3;
		}
		else
		{
			goto __Day;
		}
	}
	else
	{
		if(u4CrruentTime > 40320ul)
		{
			u4CrruentTime	-=40320ul; //28*24*60
			*(Time+1) = 3;
		}
		else
		{
			goto __Day;
		}
	}


	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 4;
	}
	else
	{
		goto __Day;
	}
	
	if(u4CrruentTime>43200ul)
	{
		u4CrruentTime	-=43200ul;    //30*24*60
		*(Time+1) = 5;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 6;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>43200ul)
	{
		u4CrruentTime	-=43200ul;    //30*24*60
		*(Time+1) = 7;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 8;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 9;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>43200ul)
	{
		u4CrruentTime	-=43200ul;    //30*24*60
		*(Time+1) = 10;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>44640ul)
	{
		u4CrruentTime	-=44640ul;    //31*24*60
		*(Time+1) = 11;
	}
	else
	{
		goto __Day;
	}

	if(u4CrruentTime>43200ul)
	{
		u4CrruentTime	-=43200ul;    //30*24*60
		*(Time+1) = 12;
	}

	__Day:

	*(Time+2) = (u1)(u4CrruentTime/1440)+1;			//day
	u4CrruentTime  = u4CrruentTime%1440;		//-= (*(Time+2)-1) * 1440;
	*(Time+3) = u4CrruentTime/60;					//hours
	*(Time+4) = u4CrruentTime%60;					//min
	*(Time+5) = u1Second;
		
}


//#define _UART_INTER_EN_
/********************************************************************
Function name:       vUartInit
Input parameters:
    void
Return:
    void
Function:
    Enable UART Module.
********************************************************************/
void vUartInit(void)
{
    vScu_IomClkEn();
	vIom_UartEn();
	vScu_UartClkEn();
	UARTCON=0x00;
	UARTSTAT=0xff;

	
#ifdef _UART_INTER_EN_
	//中断接收使能	
	UARTCON|=UARTTS;
	UARTCON|=0x01;
	//唤醒使能
	vScu_UartWakeupEn();

	vScu_UartIntEn();
	ES=1;
	EA=1;
#endif
}
/********************************************************************
Function name:       ucUartGetByte
Input parameters:
    void
Return:
    UINT8 ：Data from Uart
Function:
    Get One Byte from UART Interface.
********************************************************************/

UINT8 ucUartGetByte(void)
{
	UARTCON|=UARTTS;
	UARTCON|=0x01;
	while(!(UARTSTAT&RXFULSTAT));
	UARTSTAT|=RXFULSTAT;
	UARTCON=0x00;
	return ((UINT8)UARTBUF);
}

/********************************************************************
Function name:       vUartPutByte
Input parameters:
    UINT8 Byte：Send Data
Return:
    void
Function:
    Send One Byte to UART Interface.
********************************************************************/
void vUartPutByte(UINT8 Byte)
{  
#ifdef _UART_INTER_EN_
	//中断接收关闭	
	vScu_UartIntDis();
	ES=0;
#endif 
    UARTCON=0x00;
	 
	UARTCON&=0xfd;
	UARTBUF=Byte;
	UARTCON|=0x01;
	while(UARTCON&0x01);
	UARTCON=0x00; 
	
#ifdef _UART_INTER_EN_
	//中断接收使能	
	UARTCON|=UARTTS;
	UARTCON|=0x01;
	vScu_UartIntEn();
	ES=1;
#endif
		
}
/********************************************************************
Function name:       vUartSendData
Input parameters:
    UINT8 xdata *OutData：Send Data 
	UINT16 Len : Send Data length
Return:
    void
Function:
    Send Specify length Data to UART Interface.
********************************************************************/

void vUartSendData(UINT8 *OutData,UINT16 Len)
{
    UINT16 TmpLen;
	TmpLen=Len;				//Len-1;
	UARTCON&=0xfd;
	//UARTBUF=*OutData++;		//test
	//UARTCON|=0x01;			//test
	while(TmpLen--)
	{	
		UARTBUF=*OutData++;	
		UARTCON|=0x01;			//test
		while((UARTSTAT&TXFULSTAT));
		//UARTBUF=*OutData++;	//test	
	}
	while(UARTCON&0x01);
	UARTCON=0x00; 
}

#if 0
/********************************************************************
Function name:       vUartRecvData
Input parameters:
    UINT8 xdata *InData：Pointer of the Receive Data Saved
	UINT16 Len : Receive Data length
Return:
    void
Function:
    Receive Specify length Data from UART Interface.
********************************************************************/
void vUartRecvData(UINT8 xdata *InData,UINT16 Len)
{
	UARTCON|=UARTTS;
	UARTCON|=0x01;
	while(Len--)
	{
		while(!(UARTSTAT&RXFULSTAT)); 	    	    
		*InData++=UARTBUF;
		UARTSTAT|=RXFULSTAT;				//老版本的是去掉的
	}
	UARTCON=0x00;
}
#endif

#if 0 // Use Uart
u2 ReceiveData_Poll(void)					//用TBC来做超时
{
	u1 i;
	u1 u1MacChk;
	u1 u1RevDataFlag;
	u2 u2Status;
	u1	pu1IV[16];
	u2	u2Len;
	u1 pu1MacKey[16];
	u1 u1Index;
	//u1 u1ReceData;

	memset(g_UART_COM_BUF, 0x00, 64);
	
	//vScu_TbcIntEn();				//超时用TBC
	gSecCount_10S=0;
	
	//timer0 配置
	ReceiveCompleteFlag = 0;
	#if 0
	vTimer0_Stop();
	//vScu_TimerClkEn();
	vScu_Timer1ClkEn();		
	//vTimer1Start(0,TIMER_MODE2,0x8000,TM1_32KEN);
	 vTimerClkSel(0,TM2_32KEN);	
			   
	TM1MOD = 0x00;				// timer mod
	TM0CDATA= 0xFE;	
	vTimer0_CounterClr() ;
	TM0CON = 0x01;				//time2 start
	//vTimer2_Start();
	
	 vScu_Timer0IntEn();
	 vScu_Timer0WakeupEn();
	#endif
	vTimer0_Stop();
  	vTimer1Start(0,TIMER_MODE2,0x00fe,TM1_32KEN); 	//定时1S
	 
	EX1=1;
	EA=1;
	
	UARTCON|=UARTTS;						//选择接受模式
	UARTCON|=0x01;							//使能串口

#if 1
	i = 0;
	while((!(UARTSTAT&RXFULSTAT))&&(gSecCount_10S < 120))				//120s超时
	{
		TM0CON = 0x00;				//time2 stop
		//TM0CDATA= 0xf8;
		vTimer0_CounterClr() ;
		TM0CON = 0x01;				//time2 start
	}
	if(gSecCount_10S == 120)
	{
		u2Status =RSP_REV_TIMEOUT;			  
		vScu_TbcIntDis();
		return u2Status;
	}
	
	while((ReceiveCompleteFlag == 0)&&(i < 64))			//错误标志没有处理
	{
		g_UART_COM_BUF[i++]=UARTBUF;			// must here 
		TM0CON = 0x00;				//time2 stop
		//TM0CDATA= 0xf8;
		vTimer0_CounterClr() ;
		TM0CON = 0x01;				//time2 start
		
		while(!(UARTSTAT&RXFULSTAT))
		{
			if(ReceiveCompleteFlag == 1)
			{
				break;
			}
		}	
	}
	
	UARTCON=0x00;

	TM0CON = 0x00;				//time2 stop
	vTimer0_CounterClr() ;
	vScu_Timer0IntDis();
	vScu_Timer1ClkDis();	
	vScu_TbcIntDis();

	//if((ReceiveCompleteFlag == 1)||(UARTSTAT&0x07))
	//{
	//	return RSP_RECEIVE_ERR;
	//}

	u1Index = 0;
	u1RevDataFlag = 0;
	while((u1Index < i))
	{
		if(SYN_BYTE == g_UART_COM_BUF[u1Index++])
		{
			u1RevDataFlag++;
		}
		else
		{
			u1RevDataFlag = 0;
		}

		if(u1RevDataFlag == 3)
		{
			break;
		}
	}

	if(u1Index == i)return RSP_DATA_ERR;				//防止数据传错

	u1MacChk = 0;
	for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
	memset(pu1IV, 0x00, 0x10);
	u2Len = g_UART_COM_BUF[OFFSET_LEN + u1Index]+0x02;
	AlgSymmMacFun2(&g_UART_COM_BUF[u1Index], &u2Len, pu1MacKey,pu1IV);
	u2Len = g_UART_COM_BUF[OFFSET_LEN + u1Index]+0x02;
	if (0 != memcmp(g_UART_COM_BUF+u2Len+u1Index,pu1IV,4)) u1MacChk =0x01;

	if(u1MacChk)
	{
		u2Status =RSP_CHK_FAIL;
	}
	else
	{
		u2Status =RSP_SET_SUCCESS;
		for(i = 0; i< u2Len+4; i++)
		{
			g_UART_COM_BUF[i] = g_UART_COM_BUF[i+u1Index];
		}
	}

	return u2Status;
#endif

}
#else
u2 ReceiveData_Poll(void)					//Use NFC 
{
	u1 u1MacChk;
	u1 u1RevDataFlag;
	u2 u2Status;
	u1	pu1IV[16];
	u2	u2Len;
	u1 pu1MacKey[16];
	u1 u1Index;
	u1 i;
	//u1 u1ReceData;
	// add nfc busy judge by colin 2016/12/12
	if(NFC_Busy_Status() == TRUE)	// NFC is busy
	{
		return RSP_RECEIVE_ERR;
	}

	
	// add pc data judge by colin 2016/11/30
	if(PC_Data_EN()==FALSE)
	{
		return RSP_RECEIVE_ERR;
	}
	
	
	memset(g_UART_COM_BUF, 0x00, FRAME_LENGTH);

	#ifdef __NFC_WITH_BUSY_PIN__
	Read_NFC(g_UART_COM_BUF, EEPROM_ADDRESS, FRAME_LENGTH);
	#endif
	
	u1Index = 0;
	u1RevDataFlag = 0;
	while((u1Index < HEAD_LEN))
	{
		if( FRAME_HEAD != g_UART_COM_BUF[u1Index++] )
		{			
			return RSP_DATA_ERR;
		}
	}

//	if(u1Index == i)return RSP_DATA_ERR;				//防止数据传错

	u1MacChk = 0;
	for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];	
	memset(pu1IV, 0x00, 0x10);
	u2Len = g_UART_COM_BUF[OFFSET_LEN + u1Index]+OP_HEAD_LEN;
	AlgSymmMacFun2(&g_UART_COM_BUF[u1Index], &u2Len, pu1MacKey,pu1IV);
	u2Len = g_UART_COM_BUF[OFFSET_LEN + u1Index]+OP_HEAD_LEN;
	if (0 != memcmp(g_UART_COM_BUF+u2Len+u1Index,pu1IV,4)) u1MacChk =0x01;

	if(u1MacChk)
	{
		u2Status =RSP_CHK_FAIL;
	}
	else
	{
		u2Status =RSP_SET_SUCCESS;
		for(i = 0; i< u2Len+4; i++)
		{
			g_UART_COM_BUF[i] = g_UART_COM_BUF[i+u1Index];
		}
	}

	return u2Status;

}

#endif


void USART_TxRsp(u2 u2Rsp,u1 u1Opcode)
{
	//u2 u2CRC16Val;
	//u1 u1UsartBuffer[27];
	u1 pu1MacKey[16];
	u1 pu1IV[16];
    u1 i;
	u2 u2Len;
	//u1 u1Temp;
	//u4 u4Val;
	u1 pu1Seed[32];
	//u1 u1Tmp[48];
	
	//ConfigSysClkHigh();

	memset(u1UsartBuffer,0xFF,64);
	u1UsartBuffer[0] = 0x55;
	u1UsartBuffer[1] = 0x55;
	u1UsartBuffer[2] = 0x55;
	u1UsartBuffer[3] = u1Opcode;

	//vScu_SetSysClkOsc2M();
	if(u2Rsp == RSP_SET_SUCCESS)
	{
		if(u1Opcode == OPCODE_READ_CALIB)
		{
			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x03 == g_UART_COM_BUF[OFFSET_DATA + 1]))
			{
				u1UsartBuffer[4] = 0x02;
				if(gCalibflag == 1)
				{
    				if(Bak_PPMValue&0x8000)
    				{
    					u1UsartBuffer[5]=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))>>8);	
    	    			u1UsartBuffer[6]=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))); 
    				}
    				else
    				{
    			 		u1UsartBuffer[5]=(UINT8)((0x7FFF+Bak_PPMValue)>>8);	
    	    			u1UsartBuffer[6]=(UINT8)((0x7FFF+Bak_PPMValue)); 
    				}
				}
				else
				{
					u1UsartBuffer[5] = 0xFF;
					u1UsartBuffer[6] = 0xFF;
				}
				u1UsartBuffer[7] = (u1)(u2Rsp>>8);
				u1UsartBuffer[8] = (u1)u2Rsp;
				
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x06;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[9] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[10] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[11] = pu1IV[2];
				u1UsartBuffer[12] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,13);
				#else
					vUartSendData(u1UsartBuffer,13);
				#endif
			}
			else
			{
				u2Rsp = RSP_DATA_ERR;
				u1UsartBuffer[4] = 0x00;
				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
				u1UsartBuffer[6] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x04;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[9] = pu1IV[2];
				u1UsartBuffer[10] = pu1IV[3];
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
				#else
					vUartSendData(u1UsartBuffer,11);
				#endif
				
			}
		}
		else if(u1Opcode == OPCODE_READ_STATUS)
		{
//			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x01 == g_UART_COM_BUF[OFFSET_DATA + 1]))
//			{
				u1UsartBuffer[4] = 0x01;
				u1UsartBuffer[5] = gUseCase;			//gBlkCheckFlag.u1CheckStatus;
				u1UsartBuffer[6] = (u1)(u2Rsp>>8);
				u1UsartBuffer[7] = (u1)u2Rsp;
				
				for(i=0;i<16;i++)
				{
				 pu1MacKey[i] =g_u1PriKey[i];
				}

				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x05;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[8] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[9] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[10] = pu1IV[2];
				u1UsartBuffer[11] = pu1IV[3];
				

				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,12);
				#else
					vUartSendData(u1UsartBuffer,12);
				#endif
	//			  memset(u1UsartBuffer,0,64);
//			}
//			else
//			{
//				u2Rsp = RSP_DATA_ERR;
//				u1UsartBuffer[4] = 0x00;
//				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
//				u1UsartBuffer[6] = (u1)u2Rsp;
//				// 计算MAC
//				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
//				memset(pu1IV, 0x00, 0x10);
//				u2Len = 0x04;
//				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
//				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
//				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
//				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
//				u1UsartBuffer[9] = pu1IV[2];
//				u1UsartBuffer[10] = pu1IV[3];
//				//vUartSendData(u1UsartBuffer,11);
//			
//				#ifdef __NFC_WITH_BUSY_PIN__
//					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#else
//					writeToROM(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#endif
//			}
		}
		else if(u1Opcode==OPCODE_MK_SK)
		{
			u1UsartBuffer[4] = 0x04;

			//if(g_SNLen<16)
			//{
			//	for(i=0; i<16-g_SNLen; i++)			//bu ling
			//	{
			//		u1Tmp[i] = 0x30;
			//	}
			//}
			//memcpy(u1Tmp+i, g_u1SN, g_SNLen);
			//memcpy(u1UsartBuffer+21, g_u1SeedKey, KEY_LEN);
			vScu_SetSysClkOsc2M();
			memcpy(pu1Seed,gUserInfo,SEED_LEN);
			for(i=0;i<KEY_LEN;i++) pu1MacKey[i] =g_u1Key[i];
			SM4_Init(pu1MacKey);
			SM4_Crypto(pu1Seed,SEED_LEN,0x01,0x00,NULL,pu1Seed);		//解密 seed

			//memcpy(u1Tmp+16, pu1Seed, 32);
			
			
			for(i=0;i<16;i++) pu1MacKey[i] =g_u1MacKey[i];
			memset(pu1IV, 0x00, 0x10);
			u2Len = 32;
			//while(1);
			AlgSymmMacFun2(pu1Seed, &u2Len, pu1MacKey, pu1IV);
			u1UsartBuffer[5] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
			u1UsartBuffer[6] = pu1IV[1];//(u1)u2CRC16Val;
			u1UsartBuffer[7] = pu1IV[2];
			u1UsartBuffer[8] = pu1IV[3];
			//while(1);
			
			u1UsartBuffer[9] = (u1)(u2Rsp>>8);
			u1UsartBuffer[10] = (u1)u2Rsp;
			
			for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
			memset(pu1IV, 0x00, 0x10);
			u2Len = 0x08;
			AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
			//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
			u1UsartBuffer[11] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
			u1UsartBuffer[12] = pu1IV[1];//(u1)u2CRC16Val;
			u1UsartBuffer[13] = pu1IV[2];
			u1UsartBuffer[14] = pu1IV[3];
			
			
			#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,15);
			#else
					vUartSendData(u1UsartBuffer,15);
			#endif
		}
		else if(u1Opcode == OPCODE_READ_SN)
		{
			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x05 == g_UART_COM_BUF[OFFSET_DATA + 1]))
			{											
				u1UsartBuffer[4] = g_SNLen;
				for(i = 0; i < g_SNLen; i++)
				{
					u1UsartBuffer[5 + i] = g_u1SN[i] - 0x30;//用老版本的写入SN是要减去0x30的
				}
				
				u1UsartBuffer[g_SNLen+5] = (u1)(u2Rsp>>8);
				u1UsartBuffer[g_SNLen+6] = (u1)u2Rsp;
				
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = g_SNLen+4;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[g_SNLen+7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[g_SNLen+8] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[g_SNLen+9] = pu1IV[2];
				u1UsartBuffer[g_SNLen+10] = pu1IV[3];
				 
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,g_SNLen+11);
				#else
					vUartSendData(u1UsartBuffer,g_SNLen+11);
				#endif
			}
			else
			{
				u2Rsp = RSP_DATA_ERR;
				u1UsartBuffer[4] = 0x00;
				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
				u1UsartBuffer[6] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x04;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[9] = pu1IV[2];
				u1UsartBuffer[10] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
				#else
					vUartSendData(u1UsartBuffer,11);
				#endif
			}
		}
		else if(u1Opcode == OPCODE_READ_TIME)
		{
//			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x06 == g_UART_COM_BUF[OFFSET_DATA + 1]))
//			{
				u1UsartBuffer[4] = 0x06;
				//memcpy(&u1UsartBuffer[0x05],timeBuffer,0x06);
				vScu_SetSysClk32K();
				GETOTPTime(&u1UsartBuffer[0x05]);
				vScu_SetSysClkOsc2M();				
				u1UsartBuffer[11] = (u1)(u2Rsp>>8);
				u1UsartBuffer[12] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x0A;
				//AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, ALGID_SMS4, ALG_MODE_MAC, pu1IV, 0x00, 0x01);
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[13] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[14] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[15] = pu1IV[2];
				u1UsartBuffer[16] = pu1IV[3];
				//USART_Cmd(USART_PORT, ENABLE);
				//SendMutliBytes(u1UsartBuffer,17);
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,17);
				#else
					vUartSendData(u1UsartBuffer,17);
				#endif
//			}
//			else
//			{
//				u2Rsp = RSP_DATA_ERR;
//				u1UsartBuffer[4] = 0x00;
//				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
//				u1UsartBuffer[6] = (u1)u2Rsp;
//				// 计算MAC
//				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
//				memset(pu1IV, 0x00, 0x10);
//				u2Len = 0x04;
//				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
//				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
//				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
//				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
//				u1UsartBuffer[9] = pu1IV[2];
//				u1UsartBuffer[10] = pu1IV[3];
//				//vUartSendData(u1UsartBuffer,11);
//				
//				#ifdef __NFC_WITH_BUSY_PIN__
//					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#else
//					writeToROM(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#endif
//			}
		}
		else if(u1Opcode == OPCODE_READ_VERSION)
		{
//			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x08 == g_UART_COM_BUF[OFFSET_DATA + 1]))
//			{
				u1UsartBuffer[4] = 0x04;
				u1UsartBuffer[5] = 0x16;				//15.08.13.01
				u1UsartBuffer[6] = 0x09;
				u1UsartBuffer[7] = 0x14;
				u1UsartBuffer[8] = 0xA3;
				u1UsartBuffer[9] = (u1)(u2Rsp>>8);
				u1UsartBuffer[10] = (u1)u2Rsp;
				
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x08;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[11] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[12] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[13] = pu1IV[2];
				u1UsartBuffer[14] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,15);
				#else
					vUartSendData(u1UsartBuffer,15);
				#endif
				
//			}
//			else
//			{
//				u2Rsp = RSP_DATA_ERR;
//				u1UsartBuffer[4] = 0x00;
//				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
//				u1UsartBuffer[6] = (u1)u2Rsp;
//				// 计算MAC
//				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
//				memset(pu1IV, 0x00, 0x10);
//				u2Len = 0x04;
//				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
//				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
//				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
//				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
//				u1UsartBuffer[9] = pu1IV[2];
//				u1UsartBuffer[10] = pu1IV[3];
//				//vUartSendData(u1UsartBuffer,11);
//				
//				#ifdef __NFC_WITH_BUSY_PIN__
//					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#else
//					writeToROM(u1UsartBuffer,EEPROM_ADDRESS,11);
//				#endif
//			}
		}
		else if(u1Opcode == OPCODE_REQ_OK)
		{
			if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x02 == g_UART_COM_BUF[OFFSET_DATA + 1]))
			{
				u1UsartBuffer[4] = 0x00;
				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
				u1UsartBuffer[6] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x04;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[9] = pu1IV[2];
				u1UsartBuffer[10] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
				#else
					vUartSendData(u1UsartBuffer,11);
				#endif
				//u2SubSec1 = RTC_GetSubSecond();
			}
			else
			{
				u2Rsp = RSP_DATA_ERR;
				u1UsartBuffer[4] = 0x00;
				u1UsartBuffer[5] = (u1)(u2Rsp>>8);
				u1UsartBuffer[6] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x04;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[9] = pu1IV[2];
				u1UsartBuffer[10] = pu1IV[3];
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
				#else
					vUartSendData(u1UsartBuffer,11);
				#endif
				
			}
		}
		else if(u1Opcode == OPCODE_SET_CALIB)
		{
			if(gCalibflag==1)
			{
				u1UsartBuffer[4] = 0x02;
    				if(Bak_PPMValue&0x8000)
    				{
    					u1UsartBuffer[5]=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))>>8);	
    	    				u1UsartBuffer[6]=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))); 
    				}
    				else
    				{
    			 		u1UsartBuffer[5]=(UINT8)((0x7FFF+Bak_PPMValue)>>8);	
    	    				u1UsartBuffer[6]=(UINT8)((0x7FFF+Bak_PPMValue)); 
    				}
				u1UsartBuffer[7] = (u1)(u2Rsp>>8);
				u1UsartBuffer[8] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x06;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[9] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[10] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[11] = pu1IV[2];
				u1UsartBuffer[12] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,13);
				#else
					vUartSendData(u1UsartBuffer,13);
				#endif
			}
			else
			{
				u1UsartBuffer[4] = 0x02;
				u1UsartBuffer[5] = 0xFF;
				u1UsartBuffer[6] = 0xFF;
				u1UsartBuffer[7] = (u1)(u2Rsp>>8);
				u1UsartBuffer[8] = (u1)u2Rsp;
				// 计算MAC
				for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
				memset(pu1IV, 0x00, 0x10);
				u2Len = 0x06;
				AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
				//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
				u1UsartBuffer[9] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
				u1UsartBuffer[10] = pu1IV[1];//(u1)u2CRC16Val;
				u1UsartBuffer[11] = pu1IV[2];
				u1UsartBuffer[12] = pu1IV[3];
				
				
				#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,13);
				#else
					vUartSendData(u1UsartBuffer,13);
				#endif
			}
		}
		else if(u1Opcode == OPCPDE_READ_TEMP)					//SET_TEMP_BASIC
		{
			u1UsartBuffer[4] = 0x05;
			if(Basic_Temp_Cmd==1)
			{
				u1UsartBuffer[5]=(UINT8)(Basic_Temp_Value>>8);	
    	    			u1UsartBuffer[6]=(UINT8)(Basic_Temp_Value); 
				u1UsartBuffer[7]=Real_Temp_Value_H;	
    	    			u1UsartBuffer[8]=Real_Temp_Value_L; 
				#if(DEBUG)
				u1UsartBuffer[9] = 0;
				#else
				u1UsartBuffer[9] = Exral_Temp_Flag;
				#endif
			}
			else
			{
				u1UsartBuffer[5] = 0xFF;
				u1UsartBuffer[6] = 0xFF;
				u1UsartBuffer[7] = 0xFF;
				u1UsartBuffer[8] = 0xFF;
				u1UsartBuffer[9] = 0xff;
			}
			u1UsartBuffer[10] = (u1)(u2Rsp>>8);
			u1UsartBuffer[11] = (u1)u2Rsp;
			// 计算MAC
			for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
			memset(pu1IV, 0x00, 0x10);
			u2Len = 0x09;
			AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
			//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
			u1UsartBuffer[12] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
			u1UsartBuffer[13] = pu1IV[1];//(u1)u2CRC16Val;
			u1UsartBuffer[14] = pu1IV[2];
			u1UsartBuffer[15] = pu1IV[3];
			
			
			#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,16);
			#else
					vUartSendData(u1UsartBuffer,16);
			#endif
		}		
		else if(u1Opcode == OPCODE_CHANGE_STATUS)
		{
			u1UsartBuffer[4] = 0x01;
			u1UsartBuffer[5] = gUseCase;				//最新状态
			u1UsartBuffer[6] = (u1)(u2Rsp>>8);
			u1UsartBuffer[7] = (u1)u2Rsp;
			
			for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
			memset(pu1IV, 0x00, 0x10);
			u2Len = 0x05;
			AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
			//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
			u1UsartBuffer[8] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
			u1UsartBuffer[9] = pu1IV[1];//(u1)u2CRC16Val;
			u1UsartBuffer[10] = pu1IV[2];
			u1UsartBuffer[11] = pu1IV[3];
			
			
			#ifdef __NFC_WITH_BUSY_PIN__
					Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,12);
			#else
					vUartSendData(u1UsartBuffer,12);
			#endif
		}

		else
		{
			u1UsartBuffer[4] = 0x00;
			u1UsartBuffer[5] = (u1)(u2Rsp>>8);
			u1UsartBuffer[6] = (u1)u2Rsp;
			// 计算MAC
			for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
			memset(pu1IV, 0x00, 0x10);
			u2Len = 0x04;
			AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey, pu1IV);
			//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
			u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
			u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
			u1UsartBuffer[9] = pu1IV[2];
			u1UsartBuffer[10] = pu1IV[3];
			
			
			#ifdef __NFC_WITH_BUSY_PIN__
				Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
			#else
				vUartSendData(u1UsartBuffer,11);
			#endif
			//u2SubSec1 = RTC_GetSubSecond();
		}
	}
	else
	{
		u1UsartBuffer[4] = 0x00;
		u1UsartBuffer[5] = (u1)(u2Rsp>>8);
		u1UsartBuffer[6] = (u1)u2Rsp;
		// 计算MAC
		for(i=0;i<16;i++) pu1MacKey[i] =g_u1PriKey[i];
		memset(pu1IV, 0x00, 0x10);
		u2Len = 0x04;
		AlgSymmMacFun2(&u1UsartBuffer[3], &u2Len, pu1MacKey,pu1IV);
		//u2CRC16Val =CRC16Chk(&u1UsartBuffer[3],0x04);
		u1UsartBuffer[7] = pu1IV[0];//(u1)(u2CRC16Val>>0x08);
		u1UsartBuffer[8] = pu1IV[1];//(u1)u2CRC16Val;
		u1UsartBuffer[9] = pu1IV[2];
		u1UsartBuffer[10] = pu1IV[3];
		
		
		#ifdef __NFC_WITH_BUSY_PIN__
				Write_NFC(u1UsartBuffer,EEPROM_ADDRESS,11);
		#else
				vUartSendData(u1UsartBuffer,11);
		#endif
	}

	memset(u1UsartBuffer,0x00,sizeof(u1UsartBuffer));
	//vScu_SetSysClk32K();
}

