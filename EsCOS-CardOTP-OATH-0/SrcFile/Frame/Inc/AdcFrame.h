#ifndef _ADCFRAME_H_
#define _ADCFRMAE_H_
#include "REG_Z8D16R_2.h"
#include "SCUFUNC.H"
#include "ADCFUNC.H"


UINT8 ucPowerDetect(void);

#ifdef _ADCFRAME_C_
void vAdcVerify(void);
#else
extern void vAdcVerify(void);
#endif
#endif