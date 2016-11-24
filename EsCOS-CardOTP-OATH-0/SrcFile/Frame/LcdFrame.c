#define _LCDRAME_C_	
#include "LCDFRAME.H" 
#include "Debug.H"


/*****************************************************************************
  Routine Name  : Timer_Counter_Disp
  Form          : void Timer_Counter_Disp(ULONG Time)
  Parameters    : Time: the time counter of displaying
  Return value  : None
  Description   : LCD display timer counter
*****************************************************************************/
void vTimer_Counter_Disp(UINT32 Time)
{
	sprintf(BCD,"%10lu",Time);
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	                    	                    
#endif					
    vLcd_String_Clear(); 						
	vLcd_String_Display(BCD,10);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif		

}

void vFloat_Disp(float Count)
{
	sprintf(BCD,"%5.2f",Count);
	//BCD[2]='_';
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	                    	                    
#endif					
    vLcd_String_Clear(); 						
	vLcd_String_Display(BCD,6);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif		

}


#if 0
void vLcdVerify(void)
{
     
#if 1
	 UINT8 i;
	 DebugPrintf("Call Function:vLcdVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	 i=(CMD_INS-0x30);
	 while(i--)
	 { 
           vLcdInit(((CMD_P1-0x30)<<6),LCD_DUTY_1O4,((CMD_P2-0x30)<<1));
		   vLcd_Clear_All();
#ifdef _FPGA_DEMO_EN_
    vLcd_SetStopMode();		                         	                       
#endif
		   vLcd_String_Display("-o-o-o-o-",9);
#ifdef _FPGA_DEMO_EN_  
    vLcd_SetDisplayMode();		
#endif
		   vDelay(5000);
		   vDelay(5000);		   
		   vLcd_Clear_All();
#ifdef _FPGA_DEMO_EN_
    vLcd_SetStopMode();		                         	                       
#endif
		   vLcd_String_Display("oAbCdEFo",8); 		   

#ifdef _FPGA_DEMO_EN_  
           vLcd_SetDisplayMode(); 	       		
#endif
           vDelay(5000);
		   vDelay(5000);
		   
#ifdef _FPGA_DEMO_EN_
           vLcd_SetStopMode();		                         	                       
#endif
		   vLcd_String_Display("0123456789",10);		   

#ifdef _FPGA_DEMO_EN_  
          vLcd_SetDisplayMode();	       		
#endif
          vDelay(5000);
		  vDelay(5000);
		  
	}
#endif
    DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
} 
#endif