/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009£­2015 All Rights Reserved
 ********************************************************************
File: WdtFunc.c
History£º
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     WdtFunc Source Code File;
Function list:
     void WdtInit(UINT8 InitTimer)£»        
Special statement:
     NULL
 *******************************************************************/
#define _WDTFUNC_C_
#include "WDTFUNC.H"
#include "SCUFUNC.H"
/********************************************************************
Function name:       WdtInit
Input parameters:
    UINT8 InitTimer£ºInit Time Value
	OPT VALUE:
	    WDT_8S/WDT_2S/WDT_125MS/WDT_500MS
	 
Return:
    void
Function:
    Enable WDT Module.
********************************************************************/
void vWdtInit(UINT8 InitTimer)
{	
     vScu_WdtClkEn();
     WDTCON=0x5a;
	 WDTCON=0xa5;
	 WDTMOD=InitTimer;
}
/********************************************************************
Function name:       void vClrWdtCounter(void)
Input parameters:
    void
	 
Return:
    void
Function:
    Clear Wdt Counter when Wdt Overflow 
********************************************************************/
void vClrWdtCounter(void)
{
	do{
		WDTCON = 0x55;   // WDP: ->0  
	}while(WDTCON&0x01);

	WDTCON = 0x5A;		// WDP : 1 -> 0 
	WDTCON = 0xA5;		// WDP : 0 -> 1
}

