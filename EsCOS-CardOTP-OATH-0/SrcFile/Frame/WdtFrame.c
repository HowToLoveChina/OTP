#define _WDTRAME_C_	
#include "WDTFRAME.H" 
#include "Debug.H"

void vWdtVerify(void)
{
     DebugPrintf("Call Function:vWdtVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	 switch(CMD_INS)
	 { 	 	  
		  case 0x01:
		       _WDT_FLAG_=0x00; 
			   vScu_WdtWakeupEn();
			   vScu_WdtIntEn();
			   if(CMD_P1==0x01)
			   {
			   	  vWdtInit(WDT_125MS);
			   }
			   else if(CMD_P1==0x02)
			   {
			   	  vWdtInit(WDT_500MS);
			   }
			   else if(CMD_P1==0x03)
			   {
			   	  vWdtInit(WDT_2S);
			   }
			   else
			   {
			   	   vWdtInit(WDT_8S);
			   }			   
			   EX0=1;
			   EA=1;

			   vScu_SuspendModeEn();

			   while(0x55!=_WDT_FLAG_);
			   vClrWdtCounter();
			   vScu_WdtClkDis();
			   _WDT_FLAG_=0x00;	
		       break;
		  default:
		       break;
	 
	 }
	 DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);

} 