//#ifdef _UARTFUNC_H_
#define _UARTFUNC_H_
#include "REG_Z8D16R_2.h"

#define USART_BUFFER_LEN 0x40

#define USART_START         0x01
#define USART_ONGOING       0x02
#define USART_COMPLETED     0x03
#define SYN_LEN         0x03
#define SYN_BYTE        0x55

#define OFFSET_OP       0x00
#define OFFSET_LEN      0x01
#define OFFSET_DATA     0x02

#define RSP_SET_SUCCESS     0x9000
#define RSP_TIME_ERR     	0x9001
#define RSP_CHK_FAIL     	0x9002
#define RSP_REV_TIMEOUT    	0x9003
#define RSP_OPCODE_ERR    	0x9004
#define RSP_COS_ERR    		0x9005
#define RSP_DATA_ERR		0x9006
#define RSP_RECEIVE_ERR		0x9007

#define OPCODE_SN_TIME      	0xA1
#define OPCODE_MK_SK        	0xA2
#define OPCODE_READ_STATUS		0xA3
#define OPCODE_REQ_OK			0xA4
#define OPCODE_READ_CALIB		0xA5
#define OPCODE_CANCEL			0xA6
#define OPCODE_READ_SN      	0xA7
#define OPCODE_READ_TIME		0xA8
#define OPCODE_READ_KEY			0xA9
#define OPCODE_READ_VERSION		0xAA
#define OPCODE_CHANGE_STATUS   	0xAB
#define OPCODE_SET_CALIB        0xAC
#define SET_TEMP_BASIC          0xAD
#define OPCPDE_KEY_CHECK		0xAE
#define OPCPDE_READ_TEMP 		0xAF


#if 1     //我司正式版
#define	KEY_0			0x30
#define	KEY_1			0x31
#define	KEY_2			0x32
#define	KEY_3			0x33
#define	KEY_4			0x34
#define	KEY_5			0x35
#define	KEY_6			0x36
#define	KEY_7			0x37
#define	KEY_8			0x38
#define	KEY_9			0x39
#define KEY_DOT         0x3a
#define	KEY_DEL 		0x3b
#define	KEY_OK  		0x3c
#define	KEY_LEFT 		0x3d
#define	KEY_RIGHT 		0x3e
//#define	KEY_LOGIN 		0x3f
#endif

#if 0     //开发板
#define	KEY_0			0x30
#define	KEY_1			0x31
#define	KEY_2			0x32
#define	KEY_3			0x33
#define	KEY_4			0x34
#define	KEY_5			0x35
#define	KEY_6			0x36
#define	KEY_7			0x37
#define	KEY_8			0x38
#define	KEY_9			0x39
#define KEY_DOT         0x3e
#define	KEY_DEL 		0x3a
#define	KEY_OK  		0x3b
#define	KEY_LEFT 		0x3c
#define	KEY_RIGHT 		0x3d
//#define	KEY_LOGIN 		0x3f
#endif

#define DOT_OFFSET      0x80 

//#define UARTEN     (1<<0)
#define UARTTS     (1<<1)

#define FRESTAT	   (1<<0)
#define OVRSTAT	   (1<<1)
#define PARSATA	   (1<<2)
#define RXFULSTAT  (1<<3)
#define TXFULSTAT  (1<<4)

extern u1 ReceiveCompleteFlag;
//extern u1 UARTBufferIndex;
extern const	u1	code AText[];
extern const   u1  code g_u1PriKey[16];
//extern u1   g_UART_COM_BUF[64];

extern void GETOTPTime(UINT8 *Time);
extern void vUartInit(void);
extern UINT8 ucUartGetByte(void);
extern void vUartPutByte(UINT8 Byte);
//extern void vUartRecvData(UINT8 xdata *InData,UINT16 Len);
extern void vUartSendData(UINT8 *OutData,UINT16 Len);
extern u2 ReceiveData_Poll(void);
extern void USART_TxRsp(u2 u2Rsp,u1 u1Opcode);
UINT32 ulGetTbcCountVlaue(void);
