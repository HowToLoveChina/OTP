//#ifndef _OTPFRAME_H_
#define _OTPFRMAE_H_
#include "REG_Z8D16R_2.h"

//12Сʱ�Զ�����
#define _UNLOCK_TIME_CONST_  3600UL
#define _DYNDISP_NUM_   8U

#define	SEEDKEY_LEN						16//PIN���ȣ�Bytes��
#define	SEED_LEN					32//���ӳ��ȣ�Bytes��
#define MACKEY_LEN                     16 
#define KEY_LEN                     16
#define SN_LEN                      15
#define PW_SM_LEN                       16
#define TIME_LEN                    8   //ʱ�����ӳ���  
#define AC_LEN              		12   //�����볤��
#define PW_LEN                      6
#define PWLOCK_LIMITED				6
#define SCREENLEN					8		//13
#define PAY_PARA_LEN                 40

#define BASE_2011MINUTES             21563520uL //��BASE_MINUTES_2011,stm8��������BASE_MINUTES_2011���������ֱ�Ӹ�ֵ
#define TIMEOUT_SEC				60		//120

#define FRAME_HEAD						0x55
#define FRAME_LENGTH					0x40
#define EEPROM_ADDRESS				0x0000


//extern UINT8  g_UART_COM_BUF[64];
extern u1 u1LowPowerFlag;
extern u1 LongPressFlag; 
extern UINT8 idata g_ModfyPwFlag;
extern UINT8 volatile gSecCount_10S,_TIMER_FLAG_,gFlag_1S; 
//extern UINT8 bOpenHash,bCloseLCD,bAlwaysbdisplay;
extern UINT8 idata gUnlockPinErr_Count;
extern UINT8 idata gsuperpw; 
extern UINT8 idata gUseCase;
//extern UINT8 gCursor_Flag;//gCursor_Count;
extern UINT8 idata gPinErr_Count ;
//extern UINT8 idata gPinErr_6TimeFlag;
extern UINT8 idata gUserInfo[SEED_LEN];			//����
extern UINT8 idata gToken_Save[8];
extern UINT32 idata gToken_Time1 ;
extern UINT32 idata gToken_Time2;
extern UINT32 idata gTimer_Count_Base,gPinErr_Time;
//extern UINT8 gPin_Key[8];
//extern UINT8 gErr[8];  
extern UINT8 idata g_u1SeedKey[SEEDKEY_LEN];		//seedkey
extern UINT8 idata g_u1MacKey[MACKEY_LEN];		//MACkey
extern UINT8 idata g_u1Key[KEY_LEN];              //u1KEY
extern UINT8 idata g_u1SN[16];              //���к�
extern UINT8 idata g_SNLen;

extern UINT8 g_AutoUnlockFlag;
extern UINT8 idata g_AutoUnlockCnt;
extern UINT8 idata gCalibflag;
extern UINT8 idata PowerOnFlag;
extern u1 u1FirstSetFlag;
extern UINT8 gu1DetectErrFlag;
//extern UINT8 EndFlag;

extern void vOTP_RunModeProcess(void);
extern void vOTP_UnlockPinProcess(void);
extern void vOTP_SetPinProcess(void);
extern void vOTP_InputPinProcess(void);
extern void vOTP_ActiveProcess(void);
//extern void vOTP_SeedImportProcess(void);
extern void vOTP_InvalidProcess(void);
extern void vOTP_InputSN_Time(void);
extern void vOTP_CRC_SN_TIME(void);
//extern void vOTP_Ewatch(void);
extern void vOTP_KEY_CHECK(void);
extern void vOTP_InputMacKey_SeedKey(void);
extern void vOTP_InputHardware_Check(void);
extern void VOTP_TimeToken(void);
extern void vOTP_PAYProcess(void);
extern void vOTP_KEY_CALIB(void);
extern void vOTP_InitProcess(void);

//#endif
