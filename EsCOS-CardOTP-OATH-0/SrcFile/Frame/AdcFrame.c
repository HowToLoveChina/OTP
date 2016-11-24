/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: AdcFunc.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-10-10

Description:
     SecFunc Source Code File;
Function list:
    UINT8 ucPowerDetect(void)        
Special statement:
     NULL
 *******************************************************************/
#define _ADCFRAME_C_
#include "ADCFRAME.h"
#include "OTPFUNC.h"
#include "DEBUG.H"
#include "UARTFRAME.H"
#include "UARTFUNC.H"

UINT8 ucPowerDetect(void)
{
     UINT16 TmpVal;
	 float Vol;
	 vAdcInit(CH_VDD);
	 TmpVal=uiAdc_GetData();
	 TmpVal=uiAdc_GetData();
	 vAdcClose();
	 Vol=((TmpVal*9)>>8)/10.0;
	 if(Vol<=2.6)
	 {
	 	  return 1;
	 }
	 else
	 {
	 	  return 0;
	 }

}

#if 0
void vAdcVerify(void)
{
#if 1
   UINT8 i=10;
   UINT16 TmpVal,ADJVAL;
   float Tmp;
   DebugPrintf("Call Function:vAdcVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
   switch(CMD_INS-0x30)
   {
   	   case 0x00:
	        vAdcInit(CH_VDD);
	        break;
	   case 0x01:
	        vAdcInit(CH_TEMP);
	        //开温度检测使能
	        vAdcTsEn();
			break;
	   case 0x02:
	        vAdcInit(CH_GPIO);
	        break;
	   case 0x03:
			vAdcInit(CH_TEMP);
	        //开温度检测使能
	        vAdcTsEn();
			do{
   		          TmpVal=uiAdc_GetData();
				  TmpVal=uiAdc_GetData();

				  ADJVAL=_TSADJ_VAL_;

				  //ADJVAL=0x316;//测试用
				  //vTimer_Counter_Disp((UINT32)TmpVal); 
				  DebugPrintf("ADJVAL=%#x\r\n",ADJVAL);
				  DebugPrintf("TmpVal=%#x\r\n",TmpVal);

				  Tmp=30.0+(((float)ADJVAL-TmpVal)*0.5247);	 				  
				  
				  //vFloat_Disp(Tmp);
				  DebugPrintf("uiAdc_GetData111=%08.2f\r\n",Tmp);
   
               }while(1); 
			break;
		case 0x04:
			vAdcInit(CH_VDD);
			do{
			      //vScu_SetSysClkOsc2M();
   			      TmpVal=uiAdc_GetData();
                  //vTimer_Counter_Disp((UINT32)TmpVal);

				  vScu_SetSysClk32K();
				  DebugPrintf("uiAdc_GetData111=%#x\r\n",TmpVal);
				  DebugPrintf("ANALOGCTRLSDLR=0x%02bx\r\n",ANALOGCTRLSDLR);
				  DebugPrintf("ANALOGCTRLSDHR=0x%02bx\r\n",ANALOGCTRLSDHR);
				  
   
               }while(1);
			 break;	 		

	   default:
	        
			break;
   
   }
   do{
        
   		TmpVal=uiAdc_GetData();
        //vTimer_Counter_Disp((UINT32)TmpVal);	
   		DebugPrintf("uiAdc_GetData222=%#x\r\n",TmpVal);
   }while(i--);	

   vAdcClose();

	
#endif
   	DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
}
#endif