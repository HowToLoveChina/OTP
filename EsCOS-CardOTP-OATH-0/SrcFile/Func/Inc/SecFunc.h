#ifndef _SECFUNC_H_
#define _SECFUNC_H_
#include "REG_Z8D16R_2.h"
//REG : ANALOGCTRLCR
#define BGEN                      (1<<0)
#define LDEN                      (1<<1)
#define FDEN                      (1<<2)
#define TSEN                      (1<<3)
#define GLUCELLERRCLR             (1<<4)
#define vSec_BgDectect_En()       {ANALOGCTRLCR|=BGEN;}
#define vSec_LdDectect_En()       {ANALOGCTRLCR|=LDEN;}
#define vSec_FdDectect_En()       {ANALOGCTRLCR|=FDEN;}
#define vSec_TsDectect_En()       {ANALOGCTRLCR|=TSEN;}

#define vSec_BgDectect_Dis()      {ANALOGCTRLCR&=(~BGEN);}
#define vSec_LdDectect_Dis()      {ANALOGCTRLCR&=(~LDEN);}
#define vSec_FdDectect_Dis()      {ANALOGCTRLCR&=(~FDEN);}
#define vSec_TsDectect_Dis()      {ANALOGCTRLCR&=(~TSEN);}

#define vSec_GluCellErrClr()      {ANALOGCTRLCR|=GLUCELLERRCLR;}

//REG : ANALOGCTRLIER
#define LDFLAGEN                         (1<<0)
#define CRYSTALSTOPINTEN                 (1<<1)
#define XRAMPARERREN                     (1<<2)
#define IRAMPARERREN                     (1<<3)
#define IPEALERRFLAGEN                   (1<<4)
#define OTPCPATTERNERRFLAGEN             (1<<5)
#define vSec_LdDectectInt_En()           {ANALOGCTRLIER|=LDFLAGEN;}
#define vSec_CrystalStopInt_En()         {ANALOGCTRLIER|=CRYSTALSTOPINTEN;}
#define vSec_XramErrInt_En()             {ANALOGCTRLIER|=XRAMPARERREN;}
#define vSec_IramErrInt_En()             {ANALOGCTRLIER|=IRAMPARERREN;}
#define vSec_IpEalErrInt_En()            {ANALOGCTRLIER|=IPEALERRFLAGEN;}
#define vSec_OtpcPatternErrInt_En()      {ANALOGCTRLIER|=OTPCPATTERNERRFLAGEN;}

#define vSec_LdDectectInt_Dis()          {ANALOGCTRLIER&=(~LDFLAGEN);}
#define vSec_CrystalStopInt_Dis()        {ANALOGCTRLIER&=(~CRYSTALSTOPINTEN);}
#define vSec_XramErrInt_Dis()            {ANALOGCTRLIER&=(~XRAMPARERREN);}
#define vSec_IramErrInt_Dis()            {ANALOGCTRLIER&=(~IRAMPARERREN);}
#define vSec_IpEalErrInt_Dis()           {ANALOGCTRLIER&=(~IPEALERRFLAGEN);}
#define vSec_OtpcPatternErrInt_Dis()     {ANALOGCTRLIER&=(~OTPCPATTERNERRFLAGEN);}


//REG : AnalogCtrlISR1
#define OTPCGLUERR                       (1<<7)
#define IRAMPARERR                       (1<<6)
#define XRAMPARERR                       (1<<5)
//#define ADCDONE                          (1<<4)
#define FSTOPERR                         (1<<3)
#define LFERR                            (1<<2)
#define HFERR                            (1<<1)
#define LDERR                            (1<<1)
//REG : AnalogCtrlISR2
#define OTPCPATTERNERRFLAG               (1<<2)
#define IPEALERRFLAG                     (1<<1)
#define GLUCELLERRFLAG                   (1<<0)

#ifdef _SECFUNC_C_
void vSec_FreqExcepDetectEn(void);
void vSec_LightDetectEn(UINT8 LDVAL);
float vSec_TmpDetect(void);
u1 vSec_TMPERDetect(void);		//ÎÂ¶È
#else
extern void vSec_FreqExcepDetectEn(void);
extern void vSec_LightDetectEn(UINT8 LDVAL);
extern float vSec_TmpDetect(void);
extern u1 vSec_TMPERDetect(void);	
#endif
#endif