#ifndef _DEBUG_H_
#define	_DEBUG_H_
#include <intrins.h>
#include "STDIO.h"
#include "SCUFUNC.h"
#include "UARTFUNC.h"
#include "UARTFRAME.h"
#include "CMDPROC.h"

#define _Debug_

void vDelay(unsigned int ucCount); 
UINT8 ucSysClkChange(UINT8 CLK); 


#ifdef _Debug_
#define DebugPrintf  printf

#define DebugPrintBuf(cBuf, uiLen, cName)  do{\
    unsigned int xdata i;\
	DebugPrintf("%s = ", cName);\
	for(i = 0; i < uiLen; i++)\
	{\
		 DebugPrintf("%02bx", cBuf[i]);\
	}\
	DebugPrintf("\r\n");\
}while(0);


//¼Ä´æÆ÷É¨Ãè²âÊÔºêº¯Êý
#define REG_DEFVAL_TEST(regname, value)\
{\
    if(regname != (value))\
    {\
       DebugPrintf("%s\tdefault Err = 0x%02x !\r\n", (#regname), (unsigned int)regname);\
    }\
    else\
    {\
       DebugPrintf("%s\tdefault OK !\r\n", (#regname));\
    }\
}

#define REG_RW_TEST(regname, valueFF, valueAA, value55, value00)\
{\
    regname = 0xFF;\
    vDelay(0x01);\
    if(regname != (valueFF))\
    {\
       DebugPrintf("%s\t0xFF Err = 0x%02x !\r\n", (#regname), (unsigned int)regname);\
    }\
\
    regname = 0x00;\
    vDelay(0x01);\
    if(regname != (value00))\
    {\
       DebugPrintf("%s\t0x00 Err = 0x%02x !\r\n", (#regname), (unsigned int)regname);\
    }\
\
    regname = 0xAA;\
    vDelay(0x01);\
    if(regname != (valueAA))\
    {\
       DebugPrintf("%s\t0xAA Err = 0x%02x !\r\n", (#regname), (unsigned int)regname);\
    }\
\
    regname = 0x00;\
    vDelay(0x01);\
    \
    regname = 0x55;\
    vDelay(0x01);\
    if(regname != (value55))\
    {\
       DebugPrintf("%s\t0x55 Err = 0x%02x !\r\n", (#regname), (unsigned int)regname);\
    }\
\
    regname = 0x00;\
   \
    DebugPrintf("%s\tcheck OK !\r\n", (#regname)); \
    \
}

#else


#define DebugPrintf                  
#define DebugPrintBuf      

#define REG_DEFVAL_TEST               
#define REG_RW_TEST         

extern void vDelay(unsigned int ucCount);
extern UINT8 ucSysClkChange(UINT8 CLK); 
#endif
#endif


