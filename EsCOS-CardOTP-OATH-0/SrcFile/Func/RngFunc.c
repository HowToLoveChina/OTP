/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: RngFunc.c
History：
Version                      Author                     Date
V0.1                         nationz                  2013-04-2

Description:
     RngFunc Source Code File;
Function list:
    void vRngStartInit(UINT8 IPMODE);
    UINT8 ucGetRngByte(void);
    void vRngData(UINT8 *OutData,UINT16 Len);        
Special statement:
     NULL
 *******************************************************************/
#define _RNGFUNC_C_
#include "RNGFUNC.h"
#include "SCUFUNC.h"
#include "ADCFUNC.h"
/********************************************************************
Function name:       vRngStartInit
Input parameters:
    UINT8 IPMODE:RNG IP Select
Return:
    void
Function:
    Init Rng Module.
********************************************************************/
void vRngStartInit(UINT8 IPMODE,UINT8 IPCLK)
{
      vScu_AnactrlClkEn();  
	  vAdcBgEn();
      //配置RNG IP 时钟前需开启2M时钟
      vScu_Osc2MEn();
	  vScu_RngcClkEn();
	  //配置RNG IP时钟前，需要关闭
	  vRng_IpDis();	 
	
	  SCURNGC&=RNGCLKSELMASK;
	  SCURNGC|=(IPCLK&(~RNGCLKSELMASK));
     
	  RNGMODE=IPMODE;
	  vRng_IpEn();
	 //vRng_Start();
}
/********************************************************************
Function name:       ucGetRngByte
Input parameters:
    void
Return:
    UINT8 :RNG DATA
Function:
    Get One Byte Rng Data.
********************************************************************/
UINT8 ucGetRngByte(void)
{
	 vRng_Start();
	 while(RNGSTART&0x01);
	 if(RNGINT&0x01)
	 {
	     RNGINT|=0x01;
	 }
	 return ((UINT8)RNGDATA);
}
/********************************************************************
Function name:       vRngData
Input parameters:
    UINT8 *OutData:	Rng Data Saved Buffer
	UINT16 Len:	Length of Rng Data you want to Get
Return:
    void
Function:
    Get the Specify Length of Rng Data.
********************************************************************/
void vRngData(UINT8 *OutData,UINT16 Len)
{
     //UINT16 TmpLen;
	 //TmpLen	=Len;
	 while(Len--)
	 {
	 	 vRng_Start();
	     while(RNGSTART&0x01);
		 RNGINT|=0x01;
		 *OutData++=RNGDATA;
	 }

}
/********************************************************************
Function name:       void vRngClose(void)
Input parameters:
    void
Return:
    void
Function:
    Close Rng Module
********************************************************************/
void vRngClose(void)
{
	 //vScu_Osc2MDis();
	 vRng_IpDis();
	 vScu_RngcClkDis();	 	 
}