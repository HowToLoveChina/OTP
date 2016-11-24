#ifndef _ADCFUNC_H_
#define _ADCFUNC_H_
#include "REG_Z8D16R_2.h"

#define BG_EN		(1<<0)
#define LD_EN		(1<<1)
#define FD_EN		(1<<2)
#define TS_EN		(1<<3)
#define Glue_EN		(1<<4)
#define vAdcBgEn()		{ANALOGCTRLCR |= BG_EN;}
#define vAdcLDEn()		{ANALOGCTRLCR |= LD_EN;}
#define vAdcFDEn()		{ANALOGCTRLCR |= FD_EN;}
#define vAdcTsEn()		{ANALOGCTRLCR |= TS_EN;}
#define vAdcGlueEn()	{ANALOGCTRLCR |= Glue_EN;}

#define vAdcBgDis()		{ANALOGCTRLCR &= (~BG_EN);}
#define vAdcLDDis()		{ANALOGCTRLCR &= (~LD_EN);}
#define vAdcFDDis()		{ANALOGCTRLCR &= (~FD_EN);}
#define vAdcTsDis()		{ANALOGCTRLCR &= (~TS_EN);}
#define vAdcGlueDis()	{ANALOGCTRLCR &= (~Glue_EN);}

#define LDFlagEn	        (1<<0)
#define FStopFlagEn	        (1<<1)
#define vAdcLDIntEn()	    {ANALOGCTRLIER |= LDFlagEn;}
#define vAdcFStopIntEn()    {ANALOGCTRLIER |= FStopFlagEn;}
#define vAdcLDIntDis()	    {ANALOGCTRLIER &= (~ LDFlagEn);}
#define vAdcFStopIntDis()	{ANALOGCTRLIER &= (~ FStopFlagEn);}
//REG : ANALOGCTRLADCCR
#define ADC_EN         (1<<0)
#define vAdc_En()      {ANALOGCTRLADCCR|=ADC_EN;}
#define vAdc_Dis()     {ANALOGCTRLADCCR&=(~ADC_EN);}

#define ADC_START      (1<<1)
#define vAdc_Start()   {ANALOGCTRLADCCR|=ADC_START;}
#define vAdc_Stop()    {ANALOGCTRLADCCR&=(~ADC_START);}	


#define CH_MASK        (0xf3)
#define CH_GPIO        (0x1<<2)//µçÑ¹ÊäÈë·¶Î§ÊÇ0~0.9v
#define CH_TEMP        (0x0<<2)
#define CH_VDD         (0x2<<2)

//REG : ANALOGCTRLSR1
#define ADC_DONE       (0x10)

#define ADC_VALUE      (UINT16)((UINT16)((ANALOGCTRLSDHR&0x03)<<8)| ANALOGCTRLSDLR)

#ifdef _ADCFUNC_C_
void vAdcInit(UINT8 CH);
void vAdcClose(void);
UINT16 uiAdc_GetData(void);
void vDelay200Us(UINT8 Tcount);
#else
extern void vAdcInit(UINT8 CH);
extern void vAdcClose(void);
extern UINT16 uiAdc_GetData(void);
void vDelay200Us(UINT8 Tcount);
#endif
#endif