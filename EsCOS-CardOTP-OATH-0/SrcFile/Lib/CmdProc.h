#ifndef _CMDPROC_H_
#define _CMDPROC_H_
#include "UARTFRAME.H"
#include "SCUFUNC.H"
#include "SCUFRAME.H"
#include "TBCFRAME.H"
#include "OTPFRAME.H"
#include "TIMERFRAME.H"
#include "LCDFRAME.H"
#include "WDTFRAME.H"
#include "RNGFRAME.H"
#include "ADCFRAME.H"
#include "IOMFRAME.H"
#include "OTPFRAME.H"
#include "SM3FRAME.H"
#include "Debug.h"


//Type Define
typedef void (* CMD_HANDLE)(void);
typedef struct{
	UINT8 cmd;
	CMD_HANDLE cmd_handle;
}CMD_HANDLE_STRUCTURE;

#define CMD_CLA  g_UART_COM_BUF[0]
#define CMD_INS  g_UART_COM_BUF[1]
#define CMD_P1	 g_UART_COM_BUF[2]
#define CMD_P2	 g_UART_COM_BUF[3]
#define CMD_P3	 g_UART_COM_BUF[4] 



#ifdef 	_CMDPROC_C_



void vCmdHandle(void); 

#else
extern void vCmdHandle(void);
#endif
#endif