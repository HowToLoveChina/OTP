#define _ISRFRAME_C_
#include "REG_Z8D16R_2.h"
#include "Debug.h"
#include "stdio.H"
#include "TimerFunc.H"	
#include "IomFunc.h" 
#include "SCUFRAME.h" 
//#include "TokenApp.h"
#include "WDTFRAME.h" 


extern void vSaveCurrent(void);
extern void vLoadCurrent(void);

//UINT8 iFlag;

void vEX0_ISR(void) interrupt 0 
{
	vSaveCurrent();	
	EX0=0;
	//WDT

	if(SCUIS1&0x01)
	{
	     //vClrWdtCounter();
		// _WDT_FLAG_=0x55;
		 SCUIS1|=0x01;		 
	}
	//POWER WAKE IO
	if(SCUIS1&0x04)
	{
		 SCUIS1|=0x04;
	}
	
	//SEC
	if(SCUIS1&0x02)
	{
		#if(YJZX)
		ANALOGCTRLSR1 = 0xff;
		ANALOGCTRLSR2 = 0xff;
		//while(1);
		gu1DetectErrFlag = 1;
		#endif
	}
	//DebugPrintf("vEX0_ISR Int\r\n");
	EX0=1; 	
	vLoadCurrent();
}


void vTF0_ISR(void) interrupt 1
{
	vSaveCurrent();		
	ET0=0;
	//TBC

	if(SCUIS0&0x40)
	{
	    //DebugPrintf("vTF0_ISR Int!TBCINTSTS=%02bx\r\n",TBCINTSTS);
		gSecCount_10S++;
		#if(YJZX)
		gFlag_1S++;
		#endif
		_TIMER_FLAG_++;
		//_TIMER_FLAG_1_++;
		//TBCINTSTS|=(0x0f|TBCINTSTS<<4);
		TBCINTSTS|=(0x0f);
		//gFlag_1S=0x5a;
	}
	
	ET0=1; 	
	vLoadCurrent();
} 

void vEX1_ISR(void) interrupt 2
{    
	vSaveCurrent();	
	EX1=0;

    #if 1
	//TIMER
	//ReceiveCompleteFlag = 1;			//test
	if(SCUIS0&0x10) //TIMER0
	{ 		
		TM1INTSTS|=0x01;
		//iFlag=0;
		
		//IOMP0DOUT^=(0x08);	 
		ReceiveCompleteFlag = 1;
				
	}
	if(SCUIS0&0x20)	//TIMER1
	{
	    TM1INTSTS|=0x02;
		//iFlag=1;
        	vTimer0_Stop();
		//IOMP0DOUT^=(0x08);		 
		//DebugPrintf("TM1INTSTS=%02bx\r\n",TM1INTSTS);
		vTimer1Start1(0,TIMER_MODE2,0x8000,TM1_32KEN);
		ReceiveCompleteFlag = 1;
		//_TIMER_FLAG_++;
	}
	if(SCUIS0&0x80)	//TIMER2
	{
	    TM2INTSTS|=0x01;
		//iFlag=2;

		//IOMP0DOUT^=(0x10);
		//ReceiveCompleteFlag = 1;
	}
	if(SCUIS1&0x80)	//TIMER3
	{
		//SCUIS0|=0x80;
		TM2INTSTS|=0x02;
		//iFlag=3;
			
		//IOMP0DOUT^=(0x10);
		//DebugPrintf("TM2INTSTS=%02bx\r\n",TM2INTSTS);
		//vTimer2Start(2,2,0x0001,TM2_64KEN);
		//DebugPrintf("Timer%d Int",(UINT16)iFlag);		
	}
	//DebugPrintf("Timer%02bx Int",iFlag); 	
	#endif

	EX1=1;	
	vLoadCurrent();
}
void vTF1_ISR(void) interrupt 3
{
	vSaveCurrent();				   
	ET1=0;
	//IOMP0DOUT&=(~0x08);	
	//Wakeup Timer
	
	if(SCUIS1&0x08)
	{
	    SCUIS1|=0x08;
		//_SCU_FLAG_++;
		//DebugPrintf("Wakeup Timer Int \r\n");
	}
	
	//Suspend Timeout
	if(SCUIS1&0x10)
	{
		SCUIS1|=0x10;
		//DebugPrintf("Suspend Timeout Int\r\n");
	}
	//DebugPrintf("vTF1_ISR Int\r\n");
	//IOMP0DOUT|=0x08;
	ET1=1;		
	vLoadCurrent();
}

void vSI_ISR(void) interrupt 4
{
	 vSaveCurrent();	 
	 ES=0;
	 //SM3
	 //Uart
	 if(SCUIS0&0x04)
	 {
	 	 g_UART_COM_BUF[g_UART_BUF_LEN++]=UARTBUF;
		 UARTSTAT|=0x1f;
		 if(5==g_UART_BUF_LEN)
		 {
		    //g_UART_RECEIVE_OK=0x55;
		 }
	 }
	 //GPIO
	 #if 0
	 if((SCUIS0&0x02))
	 {
	     _IOM_FLAG_=IOMP1ISR;
		 
		 if(IOMP1ISR&0x0f) //ÐÐ
		 {
		      IOMP1INTS1=0x00;			  
		 	  //DebugPrintf("IOMP1ISR111=%02bx\r\n",_IOM_FLAG_);
		 }
		 else if(IOMP1ISR&0xf0)//ÁÐ
		 {
		      IOMP1INTS2=0x00; 			 
		 	  //DebugPrintf("IOMP1ISR222=%02bx\r\n",_IOM_FLAG_);
		 }

	 	 IOMP1ISR|= 0xff;
		 _KBC_INT_FLAG_=0X55;
		 
		 //DebugPrintf("IOMP1ISR=%02bx\r\n",IOMP1ISR);
	 }
	 #endif
	 
	 //ADC
	 //DebugPrintf("vSI_ISR Int\r\n");
	 ES=1;	
     vLoadCurrent();
}