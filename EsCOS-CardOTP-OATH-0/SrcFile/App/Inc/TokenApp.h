#if	0			//ndef _TOKENAPP_H_
#define _TOKENAPP_H_
#include "IOMFRAME.H"




#ifdef _TOKENAPP_C_
UINT8 idata gActive _at_ 0x30;            //激活标志位
UINT8 idata gUseCase _at_ 0x31;           //OTP 状态切换标志
UINT32 idata gTimer_Count_Base _at_ 0x32; //	OTP系统基准偏移时间
UINT32 idata gTimer_Count _at_ 0x36; //	OTP系统时间
UINT32 idata gPinErr_Time _at_ 0x3A; // OTP被锁定的系统时间
UINT8 idata gPinErr_Count _at_ 0x3E;      //Pin码输入错误计数，最大错误次数不超过6次
UINT8 idata gPinErr_6TimeFlag _at_ 0x3F;  //Pin输入错误次数超过6次标志 
UINT8  idata gUserInfo[32] _at_ 0x40;      //种子密钥等用户信息，掉电保持 
UINT8 idata gPin_Save[8] _at_ 0x60;//OTP 开机PIN码，掉电保持
UINT8 idata KeySeed[16]  _at_  0x68;
UINT32 idata gBackupInitVal _at_ 0x84; //	备份计数器初始值

UINT8 gCursor_Flag;	      //LCD光标（左右移按键）标志
UINT8 gCursor_Count;      //光标移动计数
volatile UINT8 gSecCount_10S,gFlag_1S; //指示关机等待时间计数器，每10S显示标志关闭一个，时间标志全部消失OTP自动关机
UINT8 bOpenHash,bCloseLCD,bAlwaysbdisplay;
UINT8 gUnlockPinErr_Count;//手动解锁计数，超过一定次数，OTP失效或自毁
UINT8 gPin_Key[8];
UINT8 gErr[8]={"Error-00"};
unsigned long gEvent=5621;//FOR TEST 
UINT8 xdata BCD[10]={"13579246"};

UINT8 xdata DynBuf[32] _at_ 0x0B80; 
UINT8 xdata MessBuf[64] _at_ 0x0BA0; 


void vTokenApp(void);
void vToken_SeedImport(void);
void vToken_InputPin(void);
void vToken_SetPin(void);
void vToken_UnlockPin(void);
void vToken_RunMode(void);
void vToken_Active(void);
void vToken_Invalid(void);

#else
extern UINT8 BCD[10];
extern UINT8 volatile gSecCount_10S,gFlag_1S; 
extern UINT8 bOpenHash,bCloseLCD,bAlwaysbdisplay;
extern UINT8 gUnlockPinErr_Count;
extern UINT8 idata gActive;
extern UINT8 idata gUseCase;
extern UINT8 gCursor_Flag,gCursor_Count;
extern UINT8 idata gPinErr_Count,gPinErr_6TimeFlag;
extern UINT8 idata gUserInfo[32];
extern UINT8 idata gPin_Save[8];
extern UINT32 idata gTimer_Count_Base,gPinErr_Time;
extern UINT32 idata gBackupInitVal ;
extern UINT8 gPin_Key[8];
extern UINT8 gErr[8];

extern UINT8 xdata DynBuf[]; 
extern UINT8 xdata MessBuf[];


extern void vTokenApp(void);
extern void vToken_SeedImport(void);
extern void vToken_InputPin(void);
extern void vToken_SetPin(void);
extern void vToken_UnlockPin(void);
extern void vToken_RunMode(void);
extern void vToken_Active(void);
extern void vToken_Invalid(void);


#endif	
#endif