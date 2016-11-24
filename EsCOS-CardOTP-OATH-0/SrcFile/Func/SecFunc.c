/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009��2015 All Rights Reserved
 ********************************************************************
File: AdcFunc.c
History��
Version                      Author                     Date
V0.1                         nationz              2013-10-10

Description:
     SecFunc Source Code File;
Function list:
    void vSec_FreqExcepDetectEn(void);
    float vSec_TmpDetect(void);
    void vSecIntProcess(void);        
Special statement:
     NULL
 *******************************************************************/
#define _SECFUNC_C_
#include "SECFUNC.h"
#include "SCUFUNC.h"
#include "ADCFUNC.h"
#include "TbcFunc.h"
#include "OtpFunc.h"
#include "LcdFunc.h"
/********************************************************************
Function name:       void vSec_FreqExcepDetectEn(void)
Input parameters:
    void
Return:
    void
Function:
    frequence exception detect
********************************************************************/

u1 vSec_TMPERDetect(void)		//�¶�
{
	float Tmp;
	UINT16 TmpValue,ADJVAL;

	//vScu_SetSysClkOsc2M();
	vAdcInit(CH_TEMP);
	//���¶ȼ��ʹ��
	vAdcTsEn();

	TmpValue= uiAdc_GetData(); 	
	
	TmpValue= uiAdc_GetData();


	ADJVAL = _TSADJ_VAL_;
	//ADJVAL=0x316;
	Tmp=(((float)ADJVAL-TmpValue)*0.5247);
	Tmp=30.0+Tmp;

	Tmp = Real_Tmp(Tmp); 

	vAdcTsDis();

	vAdcClose();	

	//vScu_SetSysClk32K();

	if(Tmp>100)				//(Tmp<-19||Tmp>70)
	{
		#if(DEBUG)
		return 0;
		#else
		return 1;
		#endif
	}

	return 0;
	
}

void vSec_FreqExcepDetectEn(void)		//Ƶ�ʼ�⹥��
{
	vScu_Osc2MEn();
	vScu_AnactrlClkEn();
	vSec_FdDectect_En();

	vScu_SecIntEn();
	vAdcFStopIntEn();

	vAdcFDEn();
}
/********************************************************************
Function name:       void vSec_LightDetectEn(UINT8 LDVAL)
Input parameters:
    UINT8 LDVAL: �����ֵ����
Return:
    void
Function:
    �������ʹ��
********************************************************************/
void vSec_LightDetectEn(UINT8 LDVAL)			//���⹥��011
{	 
     //����ģ��ʱ��
     vScu_AnactrlClkEn();
	 //BGʹ��
	 vAdcBgEn();
	 //���ù����ֵ
	 ANALOGCTRLTRIMR2=(LDVAL&0x07);
	 //���ʹ��
	 vAdcLDEn();

	 vAdcLDIntEn();
}
/********************************************************************
Function name:       float vSec_TmpDetect(void)
Input parameters:
    void
Return:
    float ��return the current temperature value
Function:
    temperature  detect
********************************************************************/
#if 0
float vSec_TmpDetect(void)
{
    UINT16 TmpY,AdcValue;
	float Tmp;
	vAdcInit(CH_TEMP);
	//vAdcTsEn();
	vSec_TsDectect_En();
	//ͨ��NVR���ĵ���ֵ���㣨Y = -1.675X + A��mV����
	TmpY=(XBYTE[0XBFFC]|XBYTE[0XBFFD]);	
	AdcValue= uiAdc_GetData();//��һ�ε�ֵ��Ч����
	AdcValue= uiAdc_GetData();

	Tmp=(2010.0-(AdcValue-TmpY)*40.0)/67.0;

	vAdcClose();
	vSec_TsDectect_Dis();
	return(Tmp); 
}

void vSecIntProcess(void)
{	
	ANALOGCTRLSR1=0xff;
	SCUIS1|=0x02;
} 
#endif
