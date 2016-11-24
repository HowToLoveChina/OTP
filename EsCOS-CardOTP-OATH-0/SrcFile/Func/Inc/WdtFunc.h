#ifndef _WDTFUNC_H_
#define _WDTFUNC_H_
#include "REG_Z8D16R_2.h"

#define WDT_125MS	0x00
#define WDT_500MS	0x01
#define WDT_2S		0x02
#define WDT_8S		0x03

#ifdef _WDTFUNC_C_
void vWdtInit(UINT8 InitTimer);
void vClrWdtCounter(void);

#else
extern void vWdtInit(UINT8 InitTimer);
extern void vClrWdtCounter(void);
#endif
#endif