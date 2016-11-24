#ifndef _IOMFRAME_H_
#define _IOMFRAME_H_
#include "IOMFUNC.H"

//�м����½׶�
#define KEY_STAT_PRESS     (0)
//ȷ�ϰ������»����ͷŽ׶�
#define KEY_STAT_RELEASE   (1)
//ȷ�ϰ����̰����ǳ����׶�
#define KEY_STAT_2         (2)


//����2S
#define LONG_PRESS_TIME   (2)
//������ʱʱ��
#define PRESS_TIME_OUT    8//(5)



//extern UINT16 _TIMER_FLAG_;	
extern UINT16 _TIMER_FLAG_1_;

#ifdef 	_IOMFRAME_C_
volatile UINT8 _IOM_FLAG_,_KBC_INT_FLAG_;

#if 0		//������			//YJZX
UINT8 code KeyTable[16]=
{   
   0x88,0x48,0x28,0x18,
   0x84,0x44,0x24,0x14,
   0x82,0x42,0x22,0x12,
   0x81,0x41,0x21,0x11,
};
#endif
#if 0			//����� =
UINT8 code KeyTable[16]=
{   
   0x81,0x41,0x21,0x11,
   0x82,0x42,0x22,0x12,
   0x84,0x44,0x24,0x14,
   0x88,0x48,0x28,0x18,
};
#endif
#if 1			//��ʽ��
UINT8 code KeyTable[16]=
{  
   0x48,0x81,0x41,0x21,
   0x82,0x42,0x22,0x84,
   0x44,0x24,0x28,0x14, 
   0x12,0x88,0x18,0x11,   
};
#endif

void vIomVerify(void);
void vKeboardInit(void);
UINT8 ucGetKeyValue(void);

#else

extern UINT8 _IOM_FLAG_,_KBC_INT_FLAG_;
extern void vIomVerify(void);
extern void vKeboardInit(void);
extern UINT8 ucGetKeyValue(void);
#endif
#endif