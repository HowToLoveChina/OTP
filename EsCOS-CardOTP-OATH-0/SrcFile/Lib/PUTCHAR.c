/********************************************************************
                          Nationz Co.,Ltd
		Copyright (c) 2009£­2015 All Rights Reserved 
*********************************************************************
File:          
History£º
Version                      Author                     Date
V0.1                      	 nationz               2013-3-18

Description:     
    Putchar Source Code File;
Function list:
    void putchar(char x);
Special statement:
********************************************************************/

#include "TypeDefine.h"

#define _HARDWAREPRINT_

/******************************************************************** 
Function name:   	putchar
Input parameters:
	x ,output char;
Return:
	void;	
Function: 
	Output char x via uart;
********************************************************************/
extern void vUartPutByte(UINT8 Byte);

#ifdef _HARDWAREPRINT_
void putchar(unsigned char x)
{
    vUartPutByte(x);
}

#else
UINT8 uart_delay_time = 0xa3;
void delay_xxx(unsigned char Count) 
{
    while(Count--);
}
void putchar(unsigned char x)
{
    
	unsigned char i;
	IOMP0DOUT|=0x02;	
	delay_xxx(uart_delay_time);
	IOMP0DOUT&=0xfd;	
	delay_xxx(uart_delay_time);
	for(i=0;i<8;i++)
	{
	    if(x&0x01)
		{
			IOMP0DOUT|=0x02;			
		}
		else
		{
			IOMP0DOUT&=0xfd;			
		}
		delay_xxx(uart_delay_time);
		x >>= 1; 	
	}
	IOMP0DOUT|=0x02;	
	delay_xxx(uart_delay_time);
	
}
#endif

/******************************************************************/

