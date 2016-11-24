#ifndef _OTPFUNC_H_
#define _OTPFUNC_H_
#include "REG_Z8D16R_2.h"

//Z8D16R-2的Info区地址不随OTP空间配置模式而改变，采用直接物理地址访问
#define _TSADJ_VAL_      (UINT16)((UINT16)((CBYTE[0XBFFD]&0x03)<<8)| (CBYTE[0XBFFC]))

#define OP_SN             			0x01
#define OP_MACKEY_SEEDKEY			0x02
#define OP_GEN_SEED   			    0x03
#define OP_SET_PASSWORD    	0x04
#define OP_MODIFY_PASSWORD    		0x05
#define OP_VERIFY_PASSWORD 			0x06
#define OP_CHALLENGE_TOKEN	 		0x07
#define OP_TIME_TOKEN				0x08
#define OP_PAY_TOKEN           		0x09
#define OP_UNLOCK_CODE     		    0x0a
#define OP_UNLOCK_TOKEN     		0x0b
#define OP_CHECK_KEY			0x0c

#define TIME_TOKEN_LEN				8
#define CHAL_LEN       8		//?a??ì???????3¤?è
#define	UL_LEN						8	//?a????3¤?è￡¨êy??￡?

#define	OTPSECU_OP_OK				0//2ù×÷3é1|
#define	OTPSECU_OP_ERR			1//2ù×÷ê§°ü

#define OTPSTATUS_HARDWARE			0
#define OTPSTATUS_PARACHECK			1
#define	OTPSTATUS_CALIB				2
#define	OTPSTATUS_SN_TIME			3   
#define	OTPSTATUS_MACKEY				4
#define OTPSTATUS_CRCSNTIME			5

#define	OTPSTATUS_ACTIVATE			6
#define	OTPSTATUS_PWSET				7
#define	OTPSTATUS_PWVERIFY			8
#define	OTPSTATUS_SELECTMODE		9
#define	OTPSTATUS_TIMETOKEN			10
#define	OTPSTATUS_CHALLENGE			11
#define	OTPSTATUS_PAY				12  
#define	OTPSTATUS_MODIFYPW			13
#define	OTPSTATUS_UNLOCK			14
#define	OTPSTATUS_INVALID			15

#define OTP_MIN_LEN                 4
#define OTP_MAX_LEN                 10
#define OTP_HASH_LEN_SHA1           20

#define OTP_SMALL_WINDOW	0x00
#define OTP_MIDDLE_WINDOW	0x01
#define OTP_BIG_WINDOW		0x02

#define AUTO_UNLOCK_TIME         (86400)		//(7200)
#define AUTO_UNLOCK_LIMITED         0x02

extern u1 idata g_u1RandCnt;
//extern u1 g_ModfyPwFlag;
#ifdef _OTPFUNC_C_
//UINT8 ucOtpByteRead(UINT16 Addr);
//void vOtpRead(UINT16 Addr,UINT16 Len,UINT8 *DataBuf);
//void vOtpRead_ChipSn(UINT8  *DataBuf);
long BYTE2LONG(u1* pu1Data);
u1 CosOperation(u1 Mode,u1 *pu1DataBuf);
#else
//extern UINT8 ucOtpByteRead(UINT16 Addr);
//extern void vOtpRead(UINT16 Addr,UINT16 Len,UINT8 *DataBuf);
//extern void vOtpRead_ChipSn(UINT8  *DataBuf);
extern long BYTE2LONG(u1* pu1Data);
extern u1 CosOperation(u1 Mode,u1 *pu1DataBuf); 
#endif
#endif