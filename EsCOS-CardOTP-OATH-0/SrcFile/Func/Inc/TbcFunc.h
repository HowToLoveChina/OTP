#ifndef _TBCFUNC_H_
#define _TBCFUNC_H_
#include "REG_Z8D16R_2.h"



#define INT1HZEN           (1<<4)
#define INT2HZEN           (1<<5)
#define INT10SEN           (1<<6)
#define INT60SEN           (1<<7)

#define vTbc_Int1HzEn()      {TBCINTSTS|=0x0f;TBCINTSTS|=INT1HZEN;}
#define vTbc_Int2HzEn()      {TBCINTSTS|=0x0f;TBCINTSTS|=INT2HZEN;}
#define vTbc_Int10SEn()      {TBCINTSTS|=0x0f;TBCINTSTS|=INT10SEN;}
#define vTbc_Int60SEn()      {TBCINTSTS|=0x0f;TBCINTSTS|=INT60SEN;}

#define vTbc_Int1HzDis()      {TBCINTSTS&=(~INT1HZEN);}
#define vTbc_Int2HzDis()      {TBCINTSTS&=(~INT2HZEN);}
#define vTbc_Int10SDis()      {TBCINTSTS&=(~INT10SEN);}
#define vTbc_Int60SDis()      {TBCINTSTS&=(~INT60SEN);}


#define vTbc_StartEn()         {TBCSTART|=0x01;}
#define vTbc_StartDis()        {TBCSTART&=0xfe;}
#define vTbc_ReadBakCounter()  {TBCSTART|=0x02;}
#define vTbc_ReadCurCounter()  {TBCSTART&=0xfd;}

#define Temp_LEN                    28      //温度表补偿值表长度
#define WakeUp_MIN					10      //唤醒分钟数

#define	TIMESTEP			30

#define TBC_COUNT_VALUE                ((((UINT32)TBCDR3<<24)&0xff000000)|(((UINT32)TBCDR2<<16)&0x00ff0000)|(((UINT32)TBCDR1 << 8) & 0x0000FF00) | TBCDR0)


extern UINT8 idata _TEMP_INDEX_FLAG_; 
extern UINT8 idata _TEMP_COM_FLAG_;
extern float idata _LAST_COM_VALUE_; 
extern UINT8 idata WT_TenMin_Flag;         
extern UINT8 idata WT_TenMin_Couter;  
extern UINT8 idata WT_Onehour_Flag; 
extern UINT8 idata Exral_Temp_Flag;			   	//温度超过可控范围
extern UINT8 idata CompValue_Flag;				//实际温度校准索引  
extern float idata Crystal_PPM;                	//晶振PPM
extern UINT16 idata Bak_PPMValue;               //晶振PPM值备份
extern UINT16 idata Basic_Temp_Value;           //基准温度
extern UINT8 idata Basic_Temp_Cmd;             	//基准温度校准指令
extern UINT32 idata gBackupTBCInitVal;
extern UINT8 idata Real_Temp_Value_H;      //第一次实际采集温度
extern UINT8 idata Real_Temp_Value_L;      //第一次实际采集温度


#ifdef _TBCFUNC_C_
void vTbcInit(void);
UINT32 Base_Time(void);
//UINT32 ulGetTbcCountVlaue(void);
void vTBC_ADJ(float ppm);
float Real_Tmp(float Tmp);
//float fCalFreqDrift(float Temp);
void IC_Temperature_Test(void);
void vFreqAdj(UINT8 u1Flag);
void vScu_WakeUpTimerWakeUp_1Hour(void);
void vScu_WakeUpTimerWakeUp_1Min(void);
void Comp_Difference_10PPM(float value);
#else
extern void vTbcInit(void);
extern UINT32 Base_Time(void);
extern float Real_Tmp(float Tmp);
//extern UINT32 ulGetTbcCountVlaue(void);
extern void vTBC_ADJ(float ppm);
//extern float fCalFreqDrift(float Temp);
extern void IC_Temperature_Test(void);
extern void vFreqAdj(UINT8 u1Flag);
extern void vScu_WakeUpTimerWakeUp_1Hour(void);
extern void vScu_WakeUpTimerWakeUp_1Min(void);
extern void Comp_Difference_10PPM(float value);
#endif
#endif