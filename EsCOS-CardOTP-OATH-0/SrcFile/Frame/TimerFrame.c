#define _TIMERFRAME_C_
#include "TIMERFRAME.H"
#include "SCUFUNC.H"
#include "DEBUG.H"



void vTimerVerify(void)
{
    vScu_Timer1ClkEn();
	DebugPrintf("Call Function:vTimerVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	switch(CMD_INS)
	{		
		  case 0x02:	//07 02 00 00 00 
		      
			  vTimer1Start(CMD_P1,0x01,(UINT16)CMD_P2,CMD_P3);         
              IOMP0DOUT^=(0x08);
	                    
              break;
		  case 0x03:	//07 03 xx xx xx 
		      
			  vTimer2Start(CMD_P1,0x01,(UINT16)CMD_P2,CMD_P3<<4);             
              IOMP0DOUT^=(0x10);
	                    
              break;
		  case 0x04:	//07 04 xx xx xx 
		      
			  vTimer1Start(TIMER1,TIMER_MODE0,(UINT16)CMD_P2,CMD_P3); 
		      IOMP0DOUT^=(0x08);
			  
	                    
              break;
		  case 0x05:	//07 05 xx xx xx    
			  
			  vTimer2Start(TIMER3,TIMER_MODE0,(UINT16)CMD_P2,CMD_P3<<4);             
              IOMP0DOUT^=(0x10);
	                    
              break;
		  case 0x06:	//07 06 xx xx xx    
			  
			  vTimer1Start(1,TIMER_MODE2,0x8000,CMD_P3); 
			  IOMP0DOUT^=(0x08);

			  
	                    
              break;
		  case 0x07:	//07 07 xx xx xx    
			  
			  vTimer2Start(TIMER3,TIMER_MODE2,0x8000,CMD_P3<<4); 
			  IOMP0DOUT^=(0x10);
			 // while(1);
			  
	                    
              break;

		 default:
		      //vTimer1Start(1,TIMER_MODE2,0xFFFF,CMD_P3); 
			  //IOMP0DOUT^=(0x08);
		      
			  //vTimer2Start(TIMER3,TIMER_MODE2,0x8000,CMD_P3<<4); 
			  //IOMP0DOUT^=(0x10);
		      break;
	}
	//DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
}
