#ifndef _RNGFUNC_H_
#define _RNGFUNC_H_
#include "REG_Z8D16R_2.h"

#define vRng_Start()  {RNGSTART=0x01;}
#define vRng_Stop()   {RNGSTART&=0xfe;}

#define RNGMODE_IPA   (0x00<<1)
#define RNGMODE_IPB   (0x01<<1)
#define RNGMODE_IPC   (0x02<<1)
#define RNGMODE_IPD   (0x03<<1)
#define RNGMODE_IPE   (0x04<<1)//IPD作为种子，LFSR59伪随机序列输出
#define RNGMODE_IPF   (0x05<<1)//LFSR59伪随机序列输出与IPA/IPB/IPC/IPD分别异或后经ODDEVENSAMP4奇偶循环采用输出
#define RNGMODE_IPG   (0x06<<1)//LFSR59伪随机序列输出与IPA/IPB/IPC/IPD分别异或后经CycleSamp4时钟循环采用输出
#define RNGMODE_IPH   (0x07<<1)//LFSR59伪随机序列输出与IPAOUT数据异或后输出


#define RNGIPEN       (1<<0)
#define vRng_IpEn()   {RNGMODE|=RNGIPEN;}
#define vRng_IpDis()   {RNGMODE&=(~RNGIPEN);}

#ifdef _RNGFUNC_C_
void vRngStartInit(UINT8 IPMODE,UINT8 IPCLK);
UINT8 ucGetRngByte(void);
void vRngData(UINT8 *OutData,UINT16 Len);
void vRngClose(void);

#else
extern void vRngStartInit(UINT8 IPMODE,UINT8 IPCLK);
extern UINT8 ucGetRngByte(void);
extern void vRngData(UINT8 *OutData,UINT16 Len);
extern void vRngClose(void);

#endif
#endif