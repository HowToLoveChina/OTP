/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: AdcFunc.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-05-10

Description:
     AdcFunc Source Code File;
Function list:
    void vAdcInit(UINT8 CH);
    UINT16 uiAdc_GetData(void);
    void vAdcClose(void);        
Special statement:
     NULL
 *******************************************************************/
#define _ADCFUNC_C_
#include "ADCFUNC.h"
#include "SCUFUNC.h"
#include "intrins.H"
/********************************************************************
Function name:       void vAdcInit(UINT8 CH)
Input parameters:
    UINT8 CH:Sample Chanell Select
Return:
    void
Function:
    Init Adc Module.
********************************************************************/

void vDelay200Us(UINT8 Tcount)
{	
  	do{ 
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(Tcount--);
}
void vAdcInit(UINT8 CH)
{
     //开启模块时钟
     vScu_AnactrlClkEn();
	 //BG使能
	 vAdcBgEn();
	 //采样通道选择
	 ANALOGCTRLADCCR=0x00;
	 ANALOGCTRLADCCR|=CH;	 
	 //ADC使能
	 vAdc_En();
	 //延时5ms
	 vDelay200Us(25);

	 
	 /*vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);*/


	 //vAdc_Start(); 	 
	 //初始化时，先启动一下ADC，让ADC电路保持，后续采样不再需要建立启动时间
	 //vAdc_Dis();
}
/********************************************************************
Function name:       UINT16 uiAdc_GetData(void)
Input parameters:
    void
Return:
    UINT16 ：Adc Sample Value
Function:
    Get	Adc Sample Value
********************************************************************/
UINT16 uiAdc_GetData(void)
{
     vAdc_Start();

	 //若采用2MHz时钟需延时
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);

	 while(!(ANALOGCTRLSR1&ADC_DONE))
	 ANALOGCTRLSR1|=ADC_DONE;
	 vAdc_Stop();
	 return ADC_VALUE;
}
/********************************************************************
Function name:       void vAdcClose(void)
Input parameters:
    void
Return:
    void
Function:
    Close Adc Module
********************************************************************/
void vAdcClose(void)
{
      	 
	 vAdc_Stop();
	 //ADC使能关闭
	 vAdc_Dis();
	 vAdcBgDis();
	 //vScu_AnactrlClkDis(); //不能关模拟模块时钟，LCD显示需要
}