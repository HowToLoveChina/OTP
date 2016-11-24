#ifndef _KBCFUNC_H_
#define _KBCFUNC_H_
#include "REG_Z8D16R_2.h"
#define RELEASE           (1<<0)
#define LONGPRESS         (1<<1)
#define KEYNUM1           (1<<2)
#define KEYNUM2           (1<<3)

#define KBC_EN            (1<<0)
#define COMB_N            (1<<1)
#define FSTDET_EN         (1<<2)
#define vKbc_KbcEn()      {KBCCR|=KBC_EN;}
#define vKbc_KbcDis()     {KBCCR&=0xfe;}
#define vKbc_CombDis()    {KBCCR|=COMB_N;}
#define vKbc_CombEn()     {KBCCR&=0xfd;}
#define vKbc_FstDecEn()   {KBCCR|=FSTDET_EN;}
#define vKbc_FstDecDis()  {KBCCR&=(~FSTDET_EN);}

#ifdef 	_KBCFUNC_C_
//void vKbcInit(void);
#else
//extern void vKbcInit(void);
#endif
#endif