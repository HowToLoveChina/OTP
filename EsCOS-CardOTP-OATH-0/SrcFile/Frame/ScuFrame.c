#define _SCURAME_C_	
#include "SCUFRAME.H"
#include "SCUFUNC.H"
#include "TbcFUNC.H"
#include "IOMFUNC.H"
#include "string.H"
#include "LCDFUNC.H"

#include "Debug.H"

UINT8 xdata TestBuf[2048] _at_ 0x0380;

UINT8 xdata TestBuf1K[512] _at_ 0x0000;

unsigned char bdata CharData[10] _at_ 0x20;


void vScuVerify(void) 
{

    DebugPrintf("Call Function:vScuVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	switch(CMD_INS)
	{
		 case 0x00:	  //01 00 00 00 00 				  
		                    
              break;
		  
		 default:
		      vScu_StorageModeEn();
		      break;
	}
	DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
}



