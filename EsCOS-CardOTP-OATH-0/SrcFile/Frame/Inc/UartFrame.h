#ifndef _UARTFRAME_H_
#define _UARTFRAME_H_
#include "UARTFUNC.H"

#ifdef 	_UARTFRAME_C_
UINT8  g_UART_COM_BUF[50];		//64
UINT8 g_UART_BUF_LEN;
//UINT8 g_UART_RECEIVE_OK; 

#else
extern UINT8  g_UART_COM_BUF[];
extern UINT8 g_UART_BUF_LEN;
//extern UINT8 g_UART_RECEIVE_OK;
#endif
#endif