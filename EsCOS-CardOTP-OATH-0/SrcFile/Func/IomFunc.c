/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009£­2015 All Rights Reserved
 ********************************************************************
File: IomFunc.c
History£º
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     IomFunc Source Code File;
Function list:
    void vKeyBoardIOInit(void);        
Special statement:
     NULL
 *******************************************************************/
#define _IOMFUNC_C_
#include "IOMFUNC.H"
#include "SCUFUNC.H" 


#if 0
/********************************************************************
Function name:       vIom_GpioConfig
Input parameters:
    UINT8 GPn:   GPIO Number
	UINT8 InOut: 1 for OUT ,0 for IN
Return:
    void
Function:
    Config GPn to GPIO Mode and set the direction for it
********************************************************************/
void vIom_GpioConfig(UINT8 GPn,UINT8 InOut)
{
     vScu_IomClkEn();
	 switch(GPn)
	 {
	 	 case GP0:
		 case GP1:
			  vIom_UartDis();
			  break;
		 case GP2:
		      vIom_LsclkOutDis();
			  break;		 
		 case GP7:		      
		 case GP8:
		 case GP9:
		 case GP10:
		 case GP11:
		 case GP12:
		 case GP13:
		 case GP14:
		 case GP15:			      
			  break; 		 
		 default:
		      break;
		 
	 }
	 if(InOut)//OUT
	 {
	     if(GPn<=GP7)
		 {
	         IOMP0DIR|=(0x01<<GPn);
		 }
		 else if(7<GPn<16)
		 {
		 	 IOMP1DIR|=(0x01<<(GPn-8));
		 }
	 }
	 else
	 {
	     if(GPn<=GP7)
		 {
	 	     IOMP0DIR&=(~(0x01<<GPn));
		 }
		 else if(7<GPn<16)
		 {
		 	 IOMP1DIR&=~(0x01<<(GPn-8));
		 }
	 }
}
/********************************************************************
Function name:       uIom_GetGpioVal
Input parameters:
    UINT8 GPn:   GPIO Number
	
Return:
    UINT8 :  Value of GPn
Function:
    get the GPn Input Value
********************************************************************/
UINT8 uIom_GetGpioVal(UINT8 GPn)
{
	 vScu_IomClkEn();
	 if(GPn<=GP7)
	 {
	 	 return (IOMP0DIN&(0x01<<GPn));
	 }
	 else if(GPn<16)
	 {
	 	 return (IOMP1DIN&(0x01<<(GPn-8)));
	 }
	 else//Error
	 {
	 	 return 0xff;//·µ»Ø´íÎó
	 } 	 
}
/********************************************************************
Function name:       vIom_SetGpioVal
Input parameters:
    UINT8 GPn:   GPIO Number
	UINT8 Val:   value to be set, 0 or 1;
Return:
    void
Function:
    set the GPn Output Value
********************************************************************/
void vIom_SetGpioVal(UINT8 GPn,UINT8 Val)
{
	 vScu_IomClkEn();
	 if(GPn<=GP7)
	 {
	     if(Val)
		 {
	 	    IOMP0DOUT|=	(0x01<<GPn);
		 }
		 else
		 {
		 	IOMP0DOUT&=(~(0x01<<GPn));
		 }
	 }
	 else if(7<GPn<16)
	 {
	 	 if(Val)
		 {
	 	    IOMP1DOUT|=	(0x01<<(GPn-8));
		 }
		 else
		 {
		 	IOMP1DOUT&=(~(0x01<<(GPn-8)));
		 }
	 }
}
#endif