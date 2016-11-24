/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: LcdFunc.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     LcdFunc Source Code File;
Function list:
     void vLcdInit(UINT8 BIAS,UINT8 DUTY,UINT8 FRM)；
	 void vLcd_Set_All(void)；
	 void vLcd_Clear_All(void)；
	 void vLcd_Bcd_Shift(UINT8 index)；
	 void vLcd_Bcd_Clear(UINT8 index)；
	 void vLcd_Bcd_Display(UINT8 index,UINT8 BCD)；
	 void vLcd_String_Display(UINT8 cString[],UINT8 cLen)；
	 void vLcd_Flag_Display(UINT8 index,UINT8 bOpen)；
	 void vLcd_Flag_Clear(void)；        
Special statement:
     NULL
 *******************************************************************/
#define _LCDFUNC_C_
#include "LCDFRAME.H"
#include "SCUFUNC.H" 
#include "intrins.H"
#include "stdio.H"
#include "LcdFunc.h"
/********************************************************************
Function name:       vLcdInit
Input parameters:
    UINT8 BIAS：Lcd Bias Volage Set 
	UINT8 DUTY：Lcd Duty Cycles Set
	UINT8 FRM：	Lcd Scan Freq Set
Return:
    void
Function:
    Enable LCD Module.
********************************************************************/
void vDelay1ms(UINT8 Count)
{
	 while(Count--)
	 {
	 	 _nop_();
		 _nop_();
		 _nop_();
		 _nop_();
	 }

}
#ifndef _DEV_BOARD_ 
void vLcdInit(UINT8 BIAS,UINT8 DUTY,UINT8 FRM)
{   
    vScu_AnactrlClkEn();

	LCDCR&=0x81;
	LCDCR|=BIAS;
	LCDCR|=DUTY;
	LCDCR|=FRM;		

	vLcd_BiasOn();
	//Delay 10ms
	//LCDCR=0x59;//for test
	vDelay1ms(10);


	//vLcd_SetDisplayMode();
	//LCDDSPMOD=0x02;

}
#else
void vLcdInit(UINT8 BIAS,UINT8 DUTY,UINT8 FRM)
{   
    vScu_AnactrlClkEn();

	LCDCR&=0x81;
	LCDCR|=BIAS;
	LCDCR|=DUTY;
	LCDCR|=FRM;		

	vLcd_BiasOn();	
	vDelay1ms(10);


	vLcd_SetDisplayMode();
	

}
#endif

/********************************************************************
Function name:       vLcd_Set_All
Input parameters:
    void
Return:
    void
Function:
    Light On all dots of Lcd Panel.
********************************************************************/
#if	0	//ndef _DEV_BOARD_ 
void vLcd_Set_All(void)
{
	UINT8 xdata i=0x22,*pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1412;
	while(i--)
	{
		*pLcdDispReg++=0x1f;
		//*pLcdDispReg++=0xff;
	}
}
//#else
void vLcd_Set_All(void)
{
	UINT8 xdata i=0x1E,*pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1416;//SEG4
	while(i--)
	{
		*pLcdDispReg++=0x1f;		
	}
}
#endif
/********************************************************************
Function name:       vLcd_Clear_All
Input parameters:
    void
Return:
    void
Function:
    Shut Off all dots of Lcd Panel.
********************************************************************/
#if 0
#ifndef _DEV_BOARD_ 
void vLcd_Clear_All(void)
{
	UINT8 xdata i=0x22,*pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1415;
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}
//void vLcd_Clear_All_Character(void)
void vLcd_String_Clear(void)			//ok
{
	UINT8 xdata i=0x22,*pLcdDispReg;
	//pLcdDispReg=(UINT8 xdata *)0x141a;		//	0x141a	//我司屏0x1419			//test0526
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x141a;		 //从右到左显示 	0x141a	//我司屏从0x1419	
	#else
	pLcdDispReg = (UINT8 xdata *)0x1419;		 //从右到左显示 	0x141a	//我司屏从0x1419	
	#endif
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}
#else
void vLcd_Clear_All(void)
{
	UINT8 xdata i=0x1E,*pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1415;
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}
//void vLcd_Clear_All_Character(void)
void vLcd_String_Clear(void)		//ok
{
	UINT8 xdata i=0x1a,*pLcdDispReg;
	//pLcdDispReg=(UINT8 xdata *)0x141a;			//0x141a		//我司屏0x1419			//test0526	
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x141a;		 //从右到左显示 	0x141a	//我司屏从0x1419	
	#else
	pLcdDispReg = (UINT8 xdata *)0x1419;		 //从右到左显示 	0x141a	//我司屏从0x1419	
	#endif
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}
#endif
#endif

/********************************************************************
Function name:       vLcd_Bcd_Shift
Input parameters:
    UINT8 index：Specifies the Cursor Position Index
Return:
    void
Function:
    Specifies the Cursor Position Flashing.
********************************************************************/
#if	0			//ndef _DEV_BOARD_ 
void vLcd_Bcd_Shift(UINT8 index)
{
	UINT8 xdata *pLcdDispReg;	
	pLcdDispReg = (unsigned char xdata*)0x1432; 	
	if(index<14)
	{
		*(pLcdDispReg - index*2-1) ^= 0x00;		
	}
}
//#else
void vLcd_Bcd_Shift(UINT8 index)
{
	UINT8 xdata *pLcdDispReg;	
	pLcdDispReg = (unsigned char xdata*)0x1432; 	
	if(index<14)
	{
		*(pLcdDispReg - index*2-1) ^= 0x00;		
	}
}
#endif
/********************************************************************
Function name:       vLcd_Bcd_Clear
Input parameters:
    UINT8 index：Specifies the Cursor Position Index
Return:
    void
Function:
    Specifies the Cursor Position Stop Flashing.
********************************************************************/
#if 0
#ifndef _DEV_BOARD_
void vLcd_Bcd_Clear(UINT8 index)
{
	UINT8 xdata *pLcdDispReg;	 		
	//pLcdDispReg = (unsigned char xdata*)0x1432;			//从右到左显示 	0x1432	//我司屏从0x142F			//test0526
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1432;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#else
	pLcdDispReg = (UINT8 xdata *)0x142F;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#endif
	if(index<14)
	{
		*(pLcdDispReg - index*2+1) = 0;			
		*(pLcdDispReg - index*2) = 0;		
	}
}
#else
void vLcd_Bcd_Clear(UINT8 index)
{
	UINT8 xdata *pLcdDispReg;	 		
	//pLcdDispReg = (unsigned char xdata*)0x1432;			//从右到左显示 	0x1432	//我司屏从0x142F			//test0526
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1432;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#else
	pLcdDispReg = (UINT8 xdata *)0x142F;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#endif
	//if(index<14)
	if(index<14)
	{
		*(pLcdDispReg - index*2+1) = 0;			
		*(pLcdDispReg - index*2) = 0;		
	}
}
#endif
#endif
/********************************************************************
Function name:       vLcd_Bcd_Display
Input parameters:
    UINT8 index：Specifies the Display Cursor Position Index
	UINT8 BCD：Index Content to  Dispaly(must be Binary-Coded Decimal)
Return:
    void
Function:
    Display the Specifies Content.
********************************************************************/
#ifdef _RIGHT_DISP_

#if 0
void vLcd_Bcd_Display1(UINT8 index,UINT8 BCD, UINT8 flag)			//是这个		ok
{
	UINT8 xdata *pLcdDispReg;
	//pLcdDispReg=(UINT8 xdata *)0x1432; //从右到左显示 	0x1432	//我司屏从0x142F		//test0526
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1432;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#else
	pLcdDispReg = (UINT8 xdata *)0x142F;		 //从右到左显示 	0x1432	//我司屏从0x142F	
	#endif
	//vScu_SetSysClkOsc2M();

	if(flag == 0)
	{
		if(index<14)
		{
			*(pLcdDispReg-index*2+1) = Num[BCD][1];		
			*(pLcdDispReg - index*2) = (Num[BCD][0]);
			
		}
	}
	else
	{
		if(index<14)
		{
			*(pLcdDispReg-index*2+1) = Num[BCD][1]|0x08;		
			*(pLcdDispReg - index*2) = (Num[BCD][0]);			//小数点
			
		}
	}
	//vScu_SetSysClk32K();
}
#endif

void vLcd_Bcd_Display2(UINT8 index,UINT8 BCD)//, UINT8 flag)  //seg0 是0x1412，我们从seg3开始，即0x1415
{
	UINT8 xdata *pLcdDispReg;
	//#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1431;			//0x1416
	//#endif

	if(index%2)						//单数    整开始
	{
		index = (index-1)/2;
		*(pLcdDispReg - index*7) = Num1[BCD][0];		
		*(pLcdDispReg - (index*7+1)) = (Num1[BCD][1]);
		*(pLcdDispReg - (index*7+2)) = (Num1[BCD][2]);
		*(pLcdDispReg - (index*7+3)) &= 0x05;
		*(pLcdDispReg - (index*7+3)) |= (Num1[BCD][3]);
	}
	else								//双数   半开始
	{
		index = (index-1)/2;
		*(pLcdDispReg - (index*7+3)) &= 0x0a;
		*(pLcdDispReg - (index*7+3)) |= Num2[BCD][0];			
		*(pLcdDispReg - (index*7+4)) = (Num2[BCD][1]);
		*(pLcdDispReg - (index*7+5)) = (Num2[BCD][2]);
		*(pLcdDispReg - (index*7+6)) = (Num2[BCD][3]);
	}
}


void vLcd_String_Display2(UINT8 cString[],UINT8 cLen)			//ok
{
	UINT8 i,j;
	//UINT8 Temp;
	//为演示效果切换时钟
	//vScu_SetSysClkOsc500K();
	vScu_SetSysClkOsc2M();				//这个会不会影响

	for(i=0;i<cLen;i++)
	{
		if(cString[i]&0x80) 				//有小数点
		{
			cString[i] = cString[i]&0x7f;
		}
		
		for(j=0;j<21;j++)			//text26	
		{	
			if(cString[i] ==  Num_Assic1[j])
			{
				vLcd_Bcd_Display2(9-cLen+i,j);			//(cLen-i-1)
				continue;
			}
		}
	}
	//切回32K
	vScu_SetSysClk32K();
}

void vLcd_String_Clear2(void)			//ok
{
	UINT8 xdata i=0x1d,*pLcdDispReg;					//0x1e
	
	pLcdDispReg = (UINT8 xdata *)0x1415;				//0x1416
	
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}

void vLcd_All_Clear2(void)			//ok
{
	UINT8 xdata i=0x1e,*pLcdDispReg;
	
	pLcdDispReg = (UINT8 xdata *)0x1415;
	
	while(i--)
	{
		*pLcdDispReg++=0x00;
	}
}

void vLcd_All_Display2(void)	
{
	UINT8 xdata i=0x1e,*pLcdDispReg;
	
	pLcdDispReg = (UINT8 xdata *)0x1415;
	
	while(i--)
	{
		*pLcdDispReg++=0x0f;
	}
}

void vLcd_Flag_Display2(UINT8 u1Flag)
{
	UINT8 xdata *pLcdDispReg;
	UINT8 xdata *pLcdDispReg1;
	pLcdDispReg = (UINT8 xdata *)0x1432;			//SEG3 0x1415--- SEG32  0x1432
	pLcdDispReg1 = (UINT8 xdata *)0x1415;			//SEG32 0x1432 -- SEG 3  0x1415

	if(u1Flag==1)
	{
		*(pLcdDispReg) = 0x08;
	}
	else if(u1Flag==2)
	{
		*(pLcdDispReg) = 0x0c;
	}
	else if(u1Flag==3)
	{
		*(pLcdDispReg) = 0x0e;
		
	}
	else if(u1Flag==4)
	{
		*(pLcdDispReg) = 0x0f;
		*(pLcdDispReg1) &= 0x0e;
		
	}
	else if(u1Flag==5)
	{
		*(pLcdDispReg) = 0x0f;
		*(pLcdDispReg1) = 0x01;
	}
	else
	{
		*(pLcdDispReg) = 0x00;
		*(pLcdDispReg1) &= 0x0e;
		
	}
}

void vLcd_Dot_Display(UINT8 u1DotFlag)
{
	UINT8 xdata *pLcdDispReg;

	pLcdDispReg = (UINT8 xdata *)0x1415;			//SEG32 0x1432 ---  SEG2 0x1415

	if(u1DotFlag == 1)
	{
		*pLcdDispReg &= 0x01;
		*pLcdDispReg |= 0x08;
	}
	else if(u1DotFlag == 2)
	{
		*pLcdDispReg &= 0x01;
		*pLcdDispReg |= 0x04;
	}
	else if(u1DotFlag == 3)
	{
		*pLcdDispReg &= 0x01;
		*pLcdDispReg |= 0x02;
	}
	else
	{
		*pLcdDispReg &= 0x01;
	}
}

#if	0			//ndef _DEV_BOARD_
void vLcd_Bcd_Display(UINT8 index,UINT8 BCD)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1432; //从右到左显示 	
	if(index<14)
	{	    
		  *(pLcdDispReg-index*2+1) = Num[BCD][1];		
		  *(pLcdDispReg - index*2) = (Num[BCD][0]); 		
		
	}
}

void vLcd_Point_Display(UINT8 index)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1432; //从右到左显示 	
	if(index<14)
	{	    
		  *(pLcdDispReg-index*2+1) |= 0x08;		   		
		
	}
}

//#else
void vLcd_Bcd_Display(UINT8 index,UINT8 BCD)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x1432; //从右到左显示 	
	if(index<14)
	{
		*(pLcdDispReg-index*2+1) = Num[BCD][1];		
		*(pLcdDispReg - index*2) = (Num[BCD][0]);
		
	}
}
#endif

//#else

#if	0				//ndef _DEV_BOARD_
void vLcd_Bcd_Display(UINT8 index,UINT8 BCD)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x141a; //从左到右显示 	
	if(index<14)
	{
		*(pLcdDispReg+index*2) = Num[BCD][0];		
		*(pLcdDispReg+index*2+1) = (Num[BCD][1]);
		
	}
}
//#else
void vLcd_Bcd_Display(UINT8 index,UINT8 BCD)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg=(UINT8 xdata *)0x141a; //从左到右显示 	
	if(index<14)
	{
		*(pLcdDispReg+index*2) = Num[BCD][0];		
		*(pLcdDispReg+index*2+1) = (Num[BCD][1]);
		
	}
}
#endif

#endif
/********************************************************************
Function name:       vLcd_String_Display
Input parameters:
    UINT8 cString[]：The Addr of Display Content
	UINT8 cLen：The Length of cString
Return:
    void
Function:
    Display the cString Content.
********************************************************************/
#if 0
void vLcd_String_Display(UINT8 cString[],UINT8 cLen)
{
	UINT8 i,j,k=0;
	//为演示效果切换时钟
	vScu_SetSysClkOsc500K();

	for(i=0;i<cLen;i++)
	{
		for(j=0;j<27;j++)		
		{
			if(cString[i] ==  Num_Assic[j])
			{						    
				vLcd_Bcd_Display((cLen-i-1),j);
				
				continue;
			}
		}
	}
	//切回32K
	vScu_SetSysClk32K();
}
#endif
#if 0
void vLcd_String_Display(UINT8 cString[],UINT8 cLen)			//ok
{
	UINT8 i,j;
	UINT8 Temp;
	//为演示效果切换时钟
	//vScu_SetSysClkOsc500K();
	vScu_SetSysClkOsc2M();				//这个会不会影响

	for(i=0;i<cLen;i++)
	{
		for(j=0;j<27;j++)			//text26	
		{
			if(cString[i]&0x80) 				//有小数点
			{
				Temp = cString[i]&0x7f;			//还原
				if(Temp ==  Num_Assic[j])
				{
					vLcd_Bcd_Display1((cLen-i-1),j,1);
					continue;
				}
			}
			else
			{
				if(cString[i] ==  Num_Assic[j])
				{
					vLcd_Bcd_Display1((cLen-i-1),j, 0);
					continue;
				}
			}
		}
	}
	//切回32K
	vScu_SetSysClk32K();
}
#endif
/********************************************************************
Function name:       vLcd_Flag_Display
Input parameters:
    UINT8 index：Specifies the Display Flag Index
	UINT8 bOpen：1：Display  On the Specifies Flag 
	             0：Display  Off the Specifies Flag
Return:
    void
Function:
    Display the Specifies Flag.
********************************************************************/
#if 0
#ifndef _DEV_BOARD_
void vLcd_Flag_Display(UINT8 index,UINT8 bOpen)			//ok
{
	UINT8 xdata *pLcdDispReg;
	//pLcdDispReg = (UINT8 xdata *)0x1415;	//0x1418			//我司屏0x1415		//test0526
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1418;		//0x1418		//我司屏0x1415	
	#else
	pLcdDispReg = (UINT8 xdata *)0x1415;		///我司屏0x1415			//test0526
	#endif
	
	if(index<16)
	{
		if(bOpen==0)//清除显示
		{
			*(pLcdDispReg+(Flag[index][0])) &= (~Flag[index][1]);
		}
		else
		{
			*(pLcdDispReg+(Flag[index][0])) |= Flag[index][1];
		}
	}
}
#if 0
void vLcd_Flag_Display_ForIsr(UINT8 index,UINT8 bOpen)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg = (UINT8 xdata *)0x1415;
	
	if(index<16)
	{
		if(bOpen==0)//清除显示
		{
			*(pLcdDispReg+(Flag[index][0])) &= (~Flag[index][1]);
		}
		else
		{
			*(pLcdDispReg+(Flag[index][0])) |= Flag[index][1];
		}
	}
}
#endif
#else
void vLcd_Flag_Display(UINT8 index,UINT8 bOpen)		//ok
{
	UINT8 xdata *pLcdDispReg;
	#if(DEBUG)
	pLcdDispReg = (UINT8 xdata *)0x1418;		//0x1418		//我司屏0x1415	
	#else
	pLcdDispReg = (UINT8 xdata *)0x1415;		///我司屏0x1415			//test0526
	#endif
	if(index<16)
	{
		if(bOpen==0)//清除显示
		{
			*(pLcdDispReg+Flag[index][0]) &= (~Flag[index][1]);
		}
		else
		{
			*(pLcdDispReg+Flag[index][0]) |= Flag[index][1];
		}
	}
}
#if 0
void vLcd_Flag_Display_ForIsr(UINT8 index,UINT8 bOpen)
{
	UINT8 xdata *pLcdDispReg;
	pLcdDispReg = (UINT8 xdata *)0x1415;
	
	if(index<16)
	{
		if(bOpen==0)//清除显示
		{
			*(pLcdDispReg+(Flag[index][0])) &= (~Flag[index][1]);
		}
		else
		{
			*(pLcdDispReg+(Flag[index][0])) |= Flag[index][1];
		}
	}
}
#endif
#endif
/********************************************************************
Function name:       vLcd_Flag_Clear
Input parameters:
    void
Return:
    void
Function:
    Display Off All Flag.
********************************************************************/ 
#if 1
void vLcd_Flag_Clear(void)		//ok
{	UINT8 i;
	for(i=0;i<16;i++)
	{
		vLcd_Flag_Display(i,0);
	}
}
#endif
#endif
