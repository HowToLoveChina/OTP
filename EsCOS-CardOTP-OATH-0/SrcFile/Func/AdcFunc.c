/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009��2015 All Rights Reserved
 ********************************************************************
File: AdcFunc.c
History��
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
     //����ģ��ʱ��
     vScu_AnactrlClkEn();
	 //BGʹ��
	 vAdcBgEn();
	 //����ͨ��ѡ��
	 ANALOGCTRLADCCR=0x00;
	 ANALOGCTRLADCCR|=CH;	 
	 //ADCʹ��
	 vAdc_En();
	 //��ʱ5ms
	 vDelay200Us(25);

	 
	 /*vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);
	 vDelay200Us(250);*/


	 //vAdc_Start(); 	 
	 //��ʼ��ʱ��������һ��ADC����ADC��·���֣���������������Ҫ��������ʱ��
	 //vAdc_Dis();
}
/********************************************************************
Function name:       UINT16 uiAdc_GetData(void)
Input parameters:
    void
Return:
    UINT16 ��Adc Sample Value
Function:
    Get	Adc Sample Value
********************************************************************/
UINT16 uiAdc_GetData(void)
{
     vAdc_Start();

	 //������2MHzʱ������ʱ
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
	 //ADCʹ�ܹر�
	 vAdc_Dis();
	 vAdcBgDis();
	 //vScu_AnactrlClkDis(); //���ܹ�ģ��ģ��ʱ�ӣ�LCD��ʾ��Ҫ
}