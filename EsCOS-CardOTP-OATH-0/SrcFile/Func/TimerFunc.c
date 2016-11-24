/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: TimerFunc.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     TimerFunc Source Code File;
Function list:
     void vTimerStart(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue)；        
Special statement:
     NULL
 *******************************************************************/
#define _TIMERFUNC_C_
#include "TIMERFUNC.H"
#include "SCUFUNC.H"
#include "Debug.H"
/********************************************************************
Function name:       vTimer1Start
Input parameters:
    UINT8 TimerNum：Timer Select 
	UINT8 Mode：Timer Mode Config
	UINT16 InitValue：	Timer Initial 
Return:
    void
Function:
    Enable Timer Module.
********************************************************************/
#if 1
void vTimerClkSel(UINT8 TimerNum,UINT8 TimerClk)
{
	 switch(TimerClk)
	 {
	 	  case TM1_32KEN:
		  case TM2_32KEN: 		       
		  case TM1_64KEN:
		  case TM2_64KEN:
			   SCUOSCCTRL|=0x20;
			   break;
		  case TM1_500KEN:
		  case TM2_500KEN: 		       
		  case TM1_2MEN:
		  case TM2_2MEN: 		       
			   SCUOSCCTRL|=0x01;
			   break;
		   default:
		       break;		       
			   
	 }
	 if(TIMER0==TimerNum||TIMER1==TimerNum)
	 {
	 	 SCUTMRCG &= 0xf0;
	 	 SCUTMRCG |= TimerClk; 
	 }
	 else if(TIMER2==TimerNum||TIMER3==TimerNum)
	 {
	 	 SCUTMRCG &= 0x0f;
	     SCUTMRCG |= TimerClk;
	 }
	 
}
#endif

void vTimer1Start(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk)
{
     vScu_Timer1ClkEn();
	 vTimerClkSel(TimerNum,TimerClk);	 
	 switch(Mode)
	 {
	      
	 	  case TIMER_MODE0:	//两个8Bit的Timer
		       TM1MOD=0x00;
			   if(TIMER0==TimerNum)
			   {			      
			   	   TM0CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer0_CounterClr();
				   vTimer0_Start();
				   vScu_Timer0IntEn();

				   vScu_Timer0WakeupEn();
			   }
			   else if(TIMER1==TimerNum)
			   {			       
			   	   TM1CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer1_CounterClr();
				   vTimer1_Start();
				   vScu_Timer1IntEn();

				   vScu_Timer1WakeupEn();
			   }
			   break;
		  case TIMER_MODE1://一个自动Load的8Bit的Timer
		       TM1MOD=0x01;
			   if(TIMER0==TimerNum)
			   {
			   	   TM0CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer0_CounterClr();
				   vTimer0_Start();
				   vScu_Timer0IntEn();

				   vScu_Timer0WakeupEn();
			   }
			   else if(TIMER1==TimerNum)
			   {
			   	   TM1CDATA=(UINT8)InitValue;
				   //计数器清0
				   //vTimer0_CounterClr();
				   vTimer1_CounterClr();
				   
				   vTimer1_Start();
				   vScu_Timer1IntEn();
	
				   vScu_Timer1WakeupEn();
			   }			   
			   
			   break;
		  case TIMER_MODE2://一个16Bit的Timer
		       TM1MOD=0x02;	

		       TM0CDATA=(UINT8)InitValue;
			   TM1CDATA=(UINT8)(InitValue>>8);
			   //计数器清0
			   vTimer0_CounterClr();
			   vTimer1_CounterClr();

			   vTimer0_Start();
			   //vScu_Timer0IntEn();
			   vScu_Timer1IntEn();

			   vScu_Timer1WakeupEn();
			   break;
		  default :
		       break;
	 }
	 EX1=1;	 
	 EA=1;	 
}


void vTimerClkSel1(UINT8 TimerNum,UINT8 TimerClk)
{
	 switch(TimerClk)
	 {
	 	  case TM1_32KEN:
		  case TM2_32KEN: 		       
		  case TM1_64KEN:
		  case TM2_64KEN:
			   SCUOSCCTRL|=0x20;
			   break;
		  case TM1_500KEN:
		  case TM2_500KEN: 		       
		  case TM1_2MEN:
		  case TM2_2MEN: 		       
			   SCUOSCCTRL|=0x01;
			   break;
		   default:
		       break;		       
			   
	 }
	 if(TIMER0==TimerNum||TIMER1==TimerNum)
	 {
	 	 SCUTMRCG &= 0xf0;
	 	 SCUTMRCG |= TimerClk; 
	 }
	 else if(TIMER2==TimerNum||TIMER3==TimerNum)
	 {
	 	 SCUTMRCG &= 0x0f;
	     SCUTMRCG |= TimerClk;
	 }
	 
}


void vTimer1Start1(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk)
{
     vScu_Timer1ClkEn();
	 vTimerClkSel1(TimerNum,TimerClk);	 
	 switch(Mode)
	 {
	      
	 	  case TIMER_MODE0:	//两个8Bit的Timer
		       TM1MOD=0x00;
			   if(TIMER0==TimerNum)
			   {			      
			   	   TM0CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer0_CounterClr();
				   vTimer0_Start();
				   vScu_Timer0IntEn();

				   vScu_Timer0WakeupEn();
			   }
			   else if(TIMER1==TimerNum)
			   {			       
			   	   TM1CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer1_CounterClr();
				   vTimer1_Start();
				   vScu_Timer1IntEn();

				   vScu_Timer1WakeupEn();
			   }
			   break;
		  case TIMER_MODE1://一个自动Load的8Bit的Timer
		       TM1MOD=0x01;
			   if(TIMER0==TimerNum)
			   {
			   	   TM0CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer0_CounterClr();
				   vTimer0_Start();
				   vScu_Timer0IntEn();

				   vScu_Timer0WakeupEn();
			   }
			   else if(TIMER1==TimerNum)
			   {
			   	   TM1CDATA=(UINT8)InitValue;
				   //计数器清0
				   //vTimer0_CounterClr();
				   vTimer1_CounterClr();
				   
				   vTimer1_Start();
				   vScu_Timer1IntEn();
	
				   vScu_Timer1WakeupEn();
			   }			   
			   
			   break;
		  case TIMER_MODE2://一个16Bit的Timer
		       TM1MOD=0x02;	

		       TM0CDATA=(UINT8)InitValue;
			   TM1CDATA=(UINT8)(InitValue>>8);
			   //计数器清0
			   vTimer0_CounterClr();
			   vTimer1_CounterClr();

			   vTimer0_Start();
			   //vScu_Timer0IntEn();
			   vScu_Timer1IntEn();

			   vScu_Timer1WakeupEn();
			   break;
		  default :
		       break;
	 }
	 EX1=1;	 
	 EA=1;	 
}

/********************************************************************
Function name:       vTimer2Start
Input parameters:
    UINT8 TimerNum：Timer Select 
	UINT8 Mode：Timer Mode Config
	UINT16 InitValue：	Timer Initial 
Return:
    void
Function:
    Enable Timer Module.
********************************************************************/

#if 0
void vTimer2Start(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk)
{
     vScu_Timer2ClkEn();
	 vTimerClkSel(TimerNum,TimerClk);
	 switch(Mode)
	 {
	 	  case TIMER_MODE0:	//两个8Bit的Timer
		       TM2MOD=0x00;
			   if(TIMER2==TimerNum)
			   {			   
			   	   TM2CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer2_CounterClr();
				   vTimer2_Start();
				   vScu_Timer2IntEn();

				   vScu_Timer2WakeupEn();
			   }
			   else if(TIMER3==TimerNum)
			   {
			       
			   	   TM3CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer3_CounterClr();
				   vTimer3_Start();
				   vScu_Timer3IntEn();

				   vScu_Timer3WakeupEn();
			   }
			   break;
		  case TIMER_MODE1://一个自动Load的8Bit的Timer
		       TM2MOD=0x01;	              
			   if(TIMER2==TimerNum)
			   {
			   	   TM2CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer2_CounterClr();
				   vTimer2_Start();
				   vScu_Timer2IntEn();

				   vScu_Timer2WakeupEn();
			   }
			   else if(TIMER3==TimerNum)
			   {
			   	   TM3CDATA=(UINT8)InitValue;
				   //计数器清0
				   vTimer3_CounterClr();
				   vTimer3_Start();
				   vScu_Timer3IntEn();

				   vScu_Timer3WakeupEn();
			   }			   
			   break;
		  case TIMER_MODE2://一个16Bit的Timer
		       TM2MOD=0x02;			   

		       TM2CDATA=(UINT8)InitValue;
			   TM3CDATA=(UINT8)(InitValue>>8);
			   //计数器清0
			   vTimer2_CounterClr();
			   vTimer3_CounterClr();

			   vTimer2_Start();
			   //vScu_Timer0IntEn();
			   vScu_Timer3IntEn();

			   vScu_Timer3WakeupEn();
			   break;
		  default :
		       break;
	 }
	 EX1=1;
	 
	 EA=1;	 
}

#endif


