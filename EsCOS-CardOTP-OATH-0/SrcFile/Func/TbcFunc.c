/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009-2015 All Rights Reserved
 ********************************************************************
File: TbcFunc.c
History:
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     TbcFunc Source Code File;
Function list:
    void vTbcInit(void);
    UINT32 ulGetTbcCountVlaue(void);
    void vTBC_ADJ(float ppm);
    float fCalFreqDrift(float Temp);    
Special statement:
     NULL
 *******************************************************************/
#define _TBCFUNC_C_
#include "TBCFUNC.H"
#include "SCUFUNC.H"
#include "OTPFUNC.H"
#include "math.h"
#include "ADCFUNC.H"
#include "UARTFUNC.H"

/*************************����¶ȱ�***************************/ 
float code g_TempValue[Temp_LEN] = {  -25.0, -20.0, -15.0, -10.0, -7.0, -3.0,  \
                                          0.0,   5.0,   7.0,  10.0,  15.0,  20.0,  25.0,  30.0, 35.0, 38.0, \
                                          40.0, 43.0,  47.0,  50.0,  53.0,  57.0,  60.0,  63.0, 67.0, \
                                          70.0, 75.0,  80.0                                             
                                     };       

/*************************PPM����ֵ��***************************/                                     
float code g_CompValue[Temp_LEN] = {	  -76.0, -62.0, -49.0, -38.0, -31.5, -23.5, -19.0,  \
										  -12.0, -10.5, -7.5, -3.0, -1.0, 0.0, -1.5, -4.5, -7.0, -8.5,\
										  -11.5, -17.5, -22.0, -27.0, -34.5, -42.5, -48.5, -60.0, -70.0, -86.0, -105.0
									 };  

/*************************ʵ���¶Ȳ�����***************************/
float code g_RealValue[10][7] = {
									0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
									-3.0, -0.5, 0.0,  2.0,  2.0,  2.5,  3.0,
									-2.5, -1.0, 0.5,  1.5,  2.4,  3.0,  3.5,
									-3.5, -2.5, -0.5, 0.0,  1.4,  2.0,  2.5,
									-4.5, -3.5, -3.0, -1.5, -2.5, -2.0, -2.0,
									-5.0, -4.0, -3.5, -2.0, -2.5, -2.5, -2.5,
									-6.0, -4.5, -3.5, -3.0, -2.5, -2.5, -2.5,
									-7.0, -6.0, -5.0, -4.5, -4.0, -3.5, -3.5,
									-8.5, -7.5, -6.0, -5.0, -5.0, -4.5, -4.5,	
									-9.5, -8.5, -7.0, -6.5, -6.0, -5.5, -5.5	
								};

UINT8 idata _TEMP_INDEX_FLAG_        _at_ 0xb2;      //ѡ���ĸ��¶���Ϊ����һ��������¶ȵı�־λ
UINT8 idata _TEMP_COM_FLAG_          _at_ 0xb3;      //�¶�ƥ���־λ
float idata _LAST_COM_VALUE_         _at_ 0xb4;      //�ϴεõ��Ĳ���ֵ
UINT8 idata WT_TenMin_Flag           _at_ 0xb8;      //ʮ���Ӳ�����־λ
UINT8 idata WT_TenMin_Couter         _at_ 0xb9;      //ʮ���Ӳ���������
UINT8 idata WT_Onehour_Flag          _at_ 0xba;      //һСʱ��ʱ���ѱ�־λ
UINT8 idata Exral_Temp_Flag          _at_ 0xbb;		//�¶ȳ����ɿط�Χ
UINT8 idata Basic_Temp_Cmd           _at_ 0xbc;		//��׼�¶�У׼ָ��
UINT8 idata CompValue_Flag           _at_ 0xbd;		//ʵ���¶�У׼����
float idata Crystal_PPM              _at_ 0xbe;      //����PPM
UINT16 idata Bak_PPMValue            _at_ 0xc2;      //����PPMֵ����
UINT32 idata gBackupTBCInitVal       _at_ 0xc4;      //����TBC��һ�ε�ֵ
UINT16 idata Basic_Temp_Value        _at_ 0xc8;      //��׼�¶�

UINT8 idata Real_Temp_Value_H        _at_ 0xca;      //��һ��ʵ�ʲɼ��¶�
UINT8 idata Real_Temp_Value_L        _at_ 0xcb;      //��һ��ʵ�ʲɼ��¶�

float xdata Temporary[Temp_LEN];  //��ʱ�洢���¶�ֵ����

/********************************************************************
Function name:       vTbcInit
Input parameters:
    void
Return:
    void
Function:
    Enable TBC Module.
********************************************************************/
void vTbcInit(void)
{
	vScu_TbcClkEn();
	//vTbc_StartDis();//��TBC ��������
	//1HZ�ж�
	//vScu_TbcWakeupEn();

	vScu_TbcIntEn();
	//vTbc_Int1HzEn();
	//vTbc_StartEn();	
	
	ET0=1;
	EA=1;		
}

UINT32 Base_Time(void)
{
    UINT32 base_time;
    //vScu_SetSysClk32K();
    vTbc_ReadBakCounter();
    
    base_time = ulGetTbcCountVlaue();
    if(base_time >= gBackupTBCInitVal)
    {
        base_time = base_time - gBackupTBCInitVal;
    }
    else
    {
        base_time = 0xFFFFFFFF + base_time - gBackupTBCInitVal;
    }

    return base_time;
}

/********************************************************************
Function name:       ulGetTbcCountVlaue
Input parameters:
    void
Return:
    UINT32 :TBC Counter Value
Function:
    Get TBC Counter Value.
********************************************************************/
#if 0
UINT32 ulGetTbcCountVlaue(void)
{
	UINT32 TmpValue;
	return TBC_COUNT_VALUE;		
}
#endif

/********************************************************************
Function name:     vScu_WakeUpTimerWakeUp_1Hour
Input parameters:
    void
Return:
    void
Function:
    WakeUp 1Hour.
********************************************************************/
void vScu_WakeUpTimerWakeUp_1Hour(void)
{ 
	// Open Enable and TBC
	//vTbcInit();	 				
	
	vScu_SetTimerWakeup_1H();       // Wakeup Time 1H			 
	vScu_WtWakeupEn();              //�ж�״̬λʹ��
	vScu_TimerWakeupEn();	        // Enable Wakeup Timer
	
	ET1=1;
	EA=1;	
}

/********************************************************************
Function name:     vScu_WakeUpTimerWakeUp_1Min
Input parameters:
    void
Return:
    void
Function:
    WakeUp 10Min.
********************************************************************/
void vScu_WakeUpTimerWakeUp_1Min(void)
{ 
	// Open Enable and TBC
	//vTbcInit();	 			
		 
	vScu_SetTimerWakeup_1M();       // Wakeup Time 60S			 
	vScu_WtWakeupEn();              //�ж�״̬λʹ��			
	vScu_TimerWakeupEn();	        // Enable Wakeup Timer
	
	ET1=1;
	EA=1;	
}

/********************************************************************
Function name:       vTBC_ADJ
Input parameters:
    float ppm :The Frequence Drift of Crystal with Temprature
Return:
    void
Function:
    frequency compensation.
********************************************************************/
void vTBC_ADJ(float ppm)  
{
	INT16 AdjValue;    
	
	AdjValue=(ppm*2); 
    
	if(ppm>=0) //gain time
	{
		 TBCADJL=(UINT8)((0x7FFF+AdjValue));
	     TBCADJH=(UINT8)((0x7FFF+AdjValue)>>8);	     
	}
	else //losing time	����32.768K�ľ���Ƶ���¶����ߣ���25C�㸽��PPM��Ϊ��
	{	     
		 AdjValue=(~abs(AdjValue)+1); 
		 TBCADJL=(UINT8)((0x7FFF+AdjValue));
	     TBCADJH=(UINT8)((0x7FFF+AdjValue)>>8);	     	     
	}
}

/********************************************************************
Function name:       void vTEMPVALUE_FIND(void)
Input parameters:
    void  
Return:
    void
Function:
    Find the nearset temperature value	
********************************************************************/
UINT8 vTempValue_Find(float Temp)
{
    UINT8 u1Index1,temp_index,u1Index2;
    float Temp1,Temp2;
//    float xdata Temporary[Temp_LEN];  //��ʱ�洢���¶�ֵ����
    
    u1Index1 = 0xFF;
    u1Index2 = 0xFF;
    temp_index = 0;

    /***************************�ҳ���һ�¶�ֵ����************************************/

    Temp = (float)((int)(Temp*10))/10;     //�¶�ȡС�����һλ
    
    for(u1Index1 = 0; u1Index1 < Temp_LEN; u1Index1++)
    {
        if(g_TempValue[u1Index1] == Temp)
        {
            _TEMP_COM_FLAG_ = 1;    //�¶�ƥ�䵽������Ҫ�õ���һ�¶Ⱥ͵ڶ��¶Ƚ��м���
            return u1Index1;
        }
        else
        {
            _TEMP_COM_FLAG_ = 0;    //�¶�ûƥ�䵽����Ҫ�õ���һ�¶Ⱥ͵ڶ��¶Ƚ��м���
            
            Temporary[temp_index++] = fabs(g_TempValue[u1Index1]-Temp);  //����ֵ����ֵ�洢��������   
        }
    }

    for(u1Index1 = 0; u1Index1 < Temp_LEN; u1Index1++)  //�ҳ���һ�¶�����
    {
        if(Temporary[u1Index1] <= Temporary[0])
        {
            Temporary[0] = Temporary[u1Index1];
			u1Index2 = u1Index1;
        }
    }
    
    /***************************�ҳ��ڶ��¶�ֵ����************************************/
    if(u1Index2 == 0)                   //����Ϊ0���¶ȵĴ���    
    {
        _TEMP_INDEX_FLAG_ = 1;
    }
    else if(u1Index2 == Temp_LEN)       //����Ϊ���ֵTemp_LEN���¶ȵĴ��� 
    {
        _TEMP_INDEX_FLAG_ = 0;
    }
    else
    {
        Temp1 = Temporary[u1Index2-1];
		Temp2 = Temporary[u1Index2+1]; 

        if(Temp1 <= Temp2)
        {
            _TEMP_INDEX_FLAG_ = 0;      //ѡ��g_TempValue[u1Index-1]��Ϊ�ڶ��¶�
        }
        else
        {
            _TEMP_INDEX_FLAG_ = 1;      //ѡ��g_TempValue[u1Index+1]��Ϊ�ڶ��¶�
        }        
    }
    
    return u1Index2;
}


/********************************************************************
Function name:       void vCOPMVALUE_FIND(void)
Input parameters:
    void  
Return:
    void
Function:
    Find the nearset compensation value  

    ����⵽���¶� - ��һ�¶�       ��������ֵ - ��һ����ֵ
    --------------------------  =  -------------------------
       �ڶ��¶� - ��һ�¶�          �ڶ�����ֵ - ��һ����ֵ
********************************************************************/
float vCompValue_Find(float Temp,UINT8 index)
{
    float TbcValue;
    float TemVar1,TemVar2,TemVar3;
    float TemVar4,TemVar5,TemVar6;
    
    if(_TEMP_COM_FLAG_ == 1)
    {
        _TEMP_COM_FLAG_ = 0;
        return g_CompValue[index];
    }
    else
    {
        TemVar1 = g_TempValue[index];      //��һ�¶�
        TemVar4 = g_CompValue[index];      //��һ����ֵ
        
        if(index == 0)
        {           
            TemVar3 = g_TempValue[1];      //����0�ĵڶ��¶�             
            TemVar6 = g_CompValue[1];      //����0�ĵڶ�����ֵ
            
            TbcValue = TemVar4 + ((Temp-TemVar1)/(TemVar3-TemVar1))*(TemVar6-TemVar4);
        }
        else if(index == Temp_LEN)
        {
            TemVar2 = g_TempValue[Temp_LEN-1];    //����Temp_LEN-1�ĵڶ��¶�     
            TemVar5 = g_CompValue[Temp_LEN-1];    //����Temp_LEN-1�ĵڶ�����ֵ

            TbcValue = TemVar5 + ((Temp-TemVar2)/(TemVar1-TemVar2))*(TemVar4-TemVar5);
        }
        else
        {
            TemVar2 = g_TempValue[index-1];       //����index-1��Ϊ�ڶ��¶�     
            TemVar5 = g_CompValue[index-1];       //����index-1��Ϊ�ڶ�����ֵ

          
            TemVar3 = g_TempValue[index+1];       //����index+1��Ϊ�ڶ��¶�     
            TemVar6 = g_CompValue[index+1];       //����index+1��Ϊ�ڶ�����ֵ

            if(_TEMP_INDEX_FLAG_ == 1)  //ѡ��g_TempValue[u1Index+1]��Ϊ����һ�������¶�
            {
                _TEMP_INDEX_FLAG_ = 0;
                TbcValue = TemVar4 + ((Temp-TemVar1)/(TemVar3-TemVar1))*(TemVar6-TemVar4);
            }
            else                        //ѡ��g_TempValue[u1Index-1]��Ϊ����һ�������¶�
            {
                TbcValue = TemVar5 + ((Temp-TemVar2)/(TemVar1-TemVar2))*(TemVar4-TemVar5);
            }            
        }
        return TbcValue;
    }
}


/********************************************************************
Function name:       fCalFreqDrift
Input parameters:
    float Temp ��The Temprature	of Ambient Enviroment
Return:
    float �� The Frequence Drift of Crystal with Temprature
Function:
    cal the  Frequence Drift of Crystal with Temprature
Note��
	32.768 KHz Crystals Temperature curve��ppm=0.04*��T-25��^2 
********************************************************************/
#if 0
float fCalFreqDrift(float Temp)
{
	 //return (pow((Temp-25),2)/25.0);
	 return(((Temp-25)*(Temp-25))/25.0);
}
#endif

/********************************************************************
Function name:       Comp_Difference_10PPM
Input parameters:
    void  
Return:
    void
Function:
    Compensation difference is or not 10PPM  	
********************************************************************/
void Comp_Difference_10PPM(float value)
{
	if(fabs(value-_LAST_COM_VALUE_) <= 10.0)
	{
		vScu_SetSysClk32K(); 
	    vScu_WakeUpTimerWakeUp_1Hour();     //���ϴ���ȣ�����ֵ��ֵС�ڵ���10ppm,1СʱԤ��ֵ����
	    WT_Onehour_Flag = 1;
	    WT_TenMin_Flag = 0;
	}
	else
	{    
		vScu_SetSysClk32K(); 
	    vScu_WakeUpTimerWakeUp_1Min();     //���ϴ���ȣ�����ֵ��ֵ����10ppm,Ԥ��ֵ���10����
	    WT_TenMin_Flag = 1;
	    WT_Onehour_Flag = 0;
	}  
}

/********************************************************************
Function name:       Real_Tmp
Input parameters:
    IC Get Temperature
Return:
    Real Temperature
Function:
    Get Temperature in air 	
********************************************************************/

float Real_Tmp(float Tmp)
{
	UINT8 Index;

	Index = CompValue_Flag;

	if(Tmp <= -25.0)
	{
		return (Tmp+g_RealValue[Index][0]);	
	}
	else if((Tmp>-25.0) && (Tmp<=10.0))
	{
		return (Tmp+g_RealValue[Index][1]);		
	}
	else if((Tmp>10.0) && (Tmp<=20.0))
	{	
		return (Tmp+g_RealValue[Index][2]);	
	}
	else if((Tmp>20.0) && (Tmp<=30.0))
	{
		return (Tmp+g_RealValue[Index][3]);	
	}
	else if((Tmp>30.0) && (Tmp<=50.0))
	{
		return (Tmp+g_RealValue[Index][4]);	
	}
	else if((Tmp>50.0) && (Tmp<=67.0))
	{
		return (Tmp+g_RealValue[Index][5]);	    
	}
	else
	{
		return (Tmp+g_RealValue[Index][6]);	    
	}
}

void IC_Temperature_Test(void)
{
    float Tmp;
    float Basic_Tmp;
	UINT8 Basic_Tmp_Low;
	UINT16 TmpValue,ADJVAL;
	
    //vScu_SetSysClkOsc2M();
	//vScu_TbcClkEn();
	vAdcInit(CH_TEMP);
	//���¶ȼ��ʹ��
	vAdcTsEn();

	TmpValue= uiAdc_GetData(); 		
	TmpValue= uiAdc_GetData();


	ADJVAL = _TSADJ_VAL_;
	//ADJVAL=0x316;
	Tmp=(((float)ADJVAL-TmpValue)*0.5247);
	Tmp=30.0+Tmp;	
	Real_Temp_Value_H = (UINT8)(Tmp);
	Real_Temp_Value_L = (UINT8)((UINT16)(Tmp*100)%100);

	if(!Basic_Temp_Cmd)
	{
		Tmp = Tmp-27.5;				//����ǰ���¶��٣����ݲ�ͬ�����޸Ļ�׼�¶�ֵ��Ŀǰ��26.5��		
	}
	else
	{
		Basic_Tmp_Low = (u1)Basic_Temp_Value;
		Basic_Tmp = (u1)(Basic_Temp_Value>>8) + ((float)Basic_Tmp_Low/100);
		Tmp = Tmp-Basic_Tmp;		//��׼�¶�У׼
	}

	if(Tmp<-2.0)
	{
		CompValue_Flag = 0x01;
	}
	else if(Tmp>=-2.0 && Tmp<-1.0)
	{
		CompValue_Flag = 0x02;		
	}
	else if(Tmp>=-1.0 && Tmp<1.0)
	{
		CompValue_Flag = 0x03;	
	}
	else if(Tmp>=1.0 && Tmp<2.0)
	{
		CompValue_Flag = 0x04;		
	}
	else if(Tmp>=2.0 && Tmp<3.0)
	{	
		CompValue_Flag = 0x05;	
	}
	else if(Tmp>=3.0 && Tmp<4.0)
	{
		CompValue_Flag = 0x06;	
	}
	else if(Tmp>=4.0 && Tmp<5.0)
	{	
		CompValue_Flag = 0x07;	
	}
	else if(Tmp>=5.0 && Tmp<6.0)
	{	
		CompValue_Flag = 0x08;	
	}	
	else if(Tmp>=6.0 && Tmp<7.0)
	{
		CompValue_Flag = 0x09;   
	}
	else
	{
		CompValue_Flag = 0x00;
		Exral_Temp_Flag = 1;
	}

	//�ر��¶ȼ��ʹ��
	vAdcTsDis();

	vAdcClose();	
	
	//vScu_SetSysClk32K();
}


/********************************************************************
Function name:       vFreqAdj
Input parameters:
    void
Return:
    void
Function:
    Frequence Adjust

�¶Ȼ��㣺y��mv��=-1.675x���棩+A
          y=ADC��30�����ֵ��*900/1024��mv�� 

		  x=30+(_TSADJ_VAL_-ADC_VALUE)*(900/(1024*1.675))
		  �򻯵�ʽ��
		  x���棩=30+ (_TSADJ_VAL_-ADC_VALUE)*0.5247  
********************************************************************/
void vFreqAdj(UINT8 u1Flag)
{
    float Tmp,Tbc_Value;
	UINT16 TmpValue,ADJVAL;
	UINT8 Index;

    //vScu_SetSysClkOsc2M();
	//vScu_TbcClkEn();
	vAdcInit(CH_TEMP);
	//���¶ȼ��ʹ��
	vAdcTsEn();

	TmpValue= uiAdc_GetData(); 	
	
	TmpValue= uiAdc_GetData();


	ADJVAL = _TSADJ_VAL_;
	//ADJVAL=0x316;
	Tmp=(((float)ADJVAL-TmpValue)*0.5247);
	Tmp=30.0+Tmp;

	#if 0
	DebugPrintf("Before Comp IC Temperature is:%08.2f\r\n",Tmp);
	#endif

	Tmp = Real_Tmp(Tmp); 		//оƬ�����¶�ת����ʵ���¶� 
	
	Index = vTempValue_Find(Tmp);
	Tbc_Value = vCompValue_Find(Tmp,Index);	

	TBCADJL = 0xFF;
	TBCADJH = 0x7F;	

	if(Exral_Temp_Flag)
	{
		vTBC_ADJ(Crystal_PPM);
	}
	else
	{
		vTBC_ADJ(Tbc_Value+Crystal_PPM);
	}	

	//�ر��¶ȼ��ʹ��
	vAdcTsDis();

	vAdcClose();	

	//vScu_SetSysClk32K();

    if(u1Flag)
    {
        Comp_Difference_10PPM(Tbc_Value);       //�ж����β���ֵ���ϴεĲ�ֵ�Ƿ����10PPM
    }	

    _LAST_COM_VALUE_ = Tbc_Value;               //���������¶Ȳ�������ֵ  
    
}



