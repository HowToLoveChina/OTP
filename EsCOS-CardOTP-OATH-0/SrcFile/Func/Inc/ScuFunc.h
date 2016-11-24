#ifndef _SCUFUNC_H_
#define _SCUFUNC_H_
#include "REG_Z8D16R_2.h"
//REG : SCUOSCCTRL
#define OSC2MEN                 (1<<0) 
#define SYSCLKSEL_2M            (3<<1)
#define SYSCLKSEL_500K          (2<<1)
#define SYSCLKSELSTS            (1<<3)
#define HALCLKSEL               (1<<4)
#define CRYSTAL64KHZEN          (1<<5)
#define CRYSTALCLKSEL           (1<<6)
#define CRYSTALCLKSELSTS        (1<<7)
#define vScu_Osc2MEn()          {SCUOSCCTRL|=OSC2MEN;}
#define vScu_Osc2MDis()         {SCUOSCCTRL&=(~OSC2MEN);}
#define vScu_SetSysClk32K()     {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL&=0xfb;SCUOSCCTRL|=CRYSTAL64KHZEN;while(!(SCUOSCCTRL&CRYSTALCLKSELSTS));SCUOSCCTRL&=(~CRYSTALCLKSEL);SCUOSCCTRL&=(~CRYSTAL64KHZEN);SCUOSCCTRL&=(~OSC2MEN);}
#define vScu_SetSysClk64K()     {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL&=0xfb;SCUOSCCTRL|=CRYSTAL64KHZEN;while(!(SCUOSCCTRL&CRYSTALCLKSELSTS));SCUOSCCTRL|=CRYSTALCLKSEL;SCUOSCCTRL&=(~OSC2MEN);}
//#define vScu_SetSysClk32K()     {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL&=0xfb;SCUOSCCTRL|=CRYSTAL64KHZEN;while(!(SCUOSCCTRL&CRYSTALCLKSELSTS));SCUOSCCTRL&=(~CRYSTALCLKSEL);}
//#define vScu_SetSysClk64K()     {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL&=0xfb;SCUOSCCTRL|=CRYSTAL64KHZEN;while(!(SCUOSCCTRL&CRYSTALCLKSELSTS));SCUOSCCTRL|=CRYSTALCLKSEL;}
#define vScu_SetSysClkOsc2M()   {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL|=SYSCLKSEL_2M;}
#define vScu_SetSysClkOsc500K() {SCUOSCCTRL|=OSC2MEN;while(!(SCUOSCCTRL&SYSCLKSELSTS));SCUOSCCTRL&=0xfd;SCUOSCCTRL|=SYSCLKSEL_500K;}
#define vScu_SetHalClk8K()		{SCUOSCCTRL&=(~HALCLKSEL);}
#define vScu_SetHalClk16K()		{SCUOSCCTRL|=HALCLKSEL;}

//REG : SCUMCG
#define SCU_UARTEN              (1<<0)
#define TIMER1EN                (1<<1)
#define WDTEN                   (1<<2)
#define TIMER2EN                (1<<3)
#define SM3EN                   (1<<4)
#define IOMEN                   (1<<5)
#define ANACTRLEN               (1<<6)
#define TBCEN                   (1<<7)
#define vScu_UartClkEn()        {SCUMCG|=SCU_UARTEN;}
#define vScu_Timer1ClkEn()      {SCUMCG|=TIMER1EN;}				//第一路定时器
#define vScu_WdtClkEn()         {SCUMCG|=WDTEN;}
#define vScu_Timer2ClkEn()      {SCUMCG|=TIMER2EN;}				//第二路时钟
#define vScu_Sm3ClkEn()         {SCUMCG|=SM3EN;}
#define vScu_IomClkEn()         {SCUMCG|=IOMEN;}
#define vScu_AnactrlClkEn()     {SCUMCG|=ANACTRLEN;}
#define vScu_TbcClkEn()         {SCUMCG|=TBCEN;}

#define vScu_UartClkDis()       {SCUMCG&=(~SCU_UARTEN);}
#define vScu_Timer1ClkDis()     {SCUMCG&=(~TIMER1EN);}
#define vScu_WdtClkDis()        {SCUMCG&=(~WDTEN);}
#define vScu_Timer2ClkDis()     {SCUMCG&=(~TIMER2EN);}
#define vScu_Sm3ClkDis()        {SCUMCG&=(~SM3EN);}
#define vScu_IomClkDis()        {SCUMCG&=(~IOMEN);}
#define vScu_AnactrlClkDis()    {SCUMCG&=(~ANACTRLEN);}
#define vScu_TbcClkDis()        {SCUMCG&=(~TBCEN);}

//REG : SCUPM
#define STANDBY_MODE            (1<<0)
#define STORAGE_MODE            (1<<1)
#define vScu_StandbyModeEn()    {SCUPMP=0x05;SCUPMP=0x0a;SCUPM=STANDBY_MODE;}//进入低功耗模式前，须配置保护序列，才能进入
#define vScu_StorageModeEn()    {SCUPMP=0x05;SCUPMP=0x0a;SCUPM=STORAGE_MODE;}
#define vScu_SuspendModeEn()    {PCON|=0x02;_nop_();_nop_();_nop_();}

//REG : SCUSO
#define SUSPENDOTEN			             (1<<0)
#define SUSPENDOTTIME_MASK               (0xe7)
#define SUSPENDOTTIME_15S                (0<<3)
#define SUSPENDOTTIME_30S	             (1<<3)
#define SUSPENDOTTIME_60S	             (2<<3)
#define SUSPENDOTTIME_120S	             (3<<3)
#define vScu_SuspendOTEn()		         {SCUSO|=SUSPENDOTEN;}
#define vScu_SuspendOTDis()		         {SCUSO&=(~SUSPENDOTEN);}
#define vScu_SetSuspendOT_15S()	         {SCUSO&=(~SUSPENDOTEN);SCUSO&=SUSPENDOTTIME_MASK;SCUSO|=SUSPENDOTTIME_15S;}
#define vScu_SetSuspendOT_30S()	         {SCUSO&=(~SUSPENDOTEN);SCUSO&=SUSPENDOTTIME_MASK;SCUSO|=SUSPENDOTTIME_30S;}
#define vScu_SetSuspendOT_60S()	         {SCUSO&=(~SUSPENDOTEN);SCUSO&=SUSPENDOTTIME_MASK;SCUSO|=SUSPENDOTTIME_60S;}
#define vScu_SetSuspendOT_120S()         {SCUSO&=(~SUSPENDOTEN);SCUSO&=SUSPENDOTTIME_MASK;SCUSO|=SUSPENDOTTIME_120S;}
#define vScu_SuspendOT_ResultStandby()   {SCUSO&=0xf8;SCUSO|=0x02;}
#define vScu_SuspendOT_ResultStorage()	 {SCUSO&=0xf8;SCUSO|=0x06;}
#define vScu_SuspendOT_ResultInterrupt() {SCUSO&=0xf8;}

#define RESULT_STANDBY_MODE   0x02
#define RESULT_STORAGE_MODE   0x06
#define RESULT_INTERRUPT_MODE 0x00
#define vScu_SuspendOutTimeSet(OUTTIME,RESULTMODE) \
{\
	SCUSO=0x00;\
	SCUSO&=SUSPENDOTTIME_MASK;\
	SCUSO|=OUTTIME;\
	SCUSO|=RESULTMODE;\
	vScu_SuspendOTEn();\	
}


//REG : SCUWT
#define TIMERWAKEUPEN 		           (1<<0)
#define TIMERWAKESEL_MASK		       (~(7<<1))
#define TIMERWAKEUPTIME_500MS	       (0<<1)
#define TIMERWAKEUPTIME_1S	           (1<<1)
#define TIMERWAKEUPTIME_10S	           (2<<1)
#define TIMERWAKEUPTIME_1M	           (3<<1)
#define TIMERWAKEUPTIME_1H	           (4<<1)
#define TIMERWAKEUPTIME_2H	           (5<<1)
#define TIMERWAKEUPTIME_4H	           (6<<1)
#define IOPOWERWAKEENTEREN	           (1<<4)
#define vScu_TimerWakeupEn()	       {SCUWT|=TIMERWAKEUPEN;}
#define vScu_SetTimerWakeup_500MS()    {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_500MS;}
#define vScu_SetTimerWakeup_1S()       {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_1S;}
#define vScu_SetTimerWakeup_10S()      {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_10S;}
#define vScu_SetTimerWakeup_1M()       {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_1M;}
#define vScu_SetTimerWakeup_1H()       {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_1H;}
#define vScu_SetTimerWakeup_2H()       {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_2H;}
#define vScu_SetTimerWakeup_4H()       {SCUWT&=0xF1;SCUWT|=TIMERWAKEUPTIME_4H;}
#define vScu_PowerIo_ResultStandby()   {SCUWT|=IOPOWERWAKEENTEREN;}
#define vScu_PowerIo_ResultInterrupt() {SCUWT&=(~IOPOWERWAKEENTEREN);}

#define vScu_TimerWakeupDis()	       {SCUWT&=(~TIMERWAKEUPEN);}

//REG : SCUWE0
#define ADCWEN                  (1<<0)
#define GPIOWEN                 (1<<1)
#define UARTWEN                 (1<<2)
#define SM3WEN                  (1<<3)
#define TIMER0WEN               (1<<4)
#define TIMER1WEN               (1<<5)
#define TBCWEN                  (1<<6)

#define vScu_AdcWakeupEn()		{SCUWE0|=ADCWEN;}
#define vScu_GpioWakeupEn()		{SCUWE0|=GPIOWEN;}
#define vScu_UartWakeupEn()		{SCUWE0|=UARTWEN;}
#define vScu_Sm3WakeupEn()		{SCUWE0|=SM3WEN;}
#define vScu_Timer0WakeupEn()	{SCUWE0|=TIMER0WEN;}
#define vScu_Timer1WakeupEn()	{SCUWE0|=TIMER1WEN;}
#define vScu_TbcWakeupEn()		{SCUWE0|=TBCWEN;}


#define vScu_AdcWakeupDis()		{SCUWE0&=(~ADCWEN);}
#define vScu_GpioWakeupDis()	{SCUWE0&=(~GPIOWEN);}
#define vScu_UartWakeupDis()	{SCUWE0&=(~UARTWEN);}
#define vScu_Sm3WakeupDis()		{SCUWE0&=(~SM3WEN);}
#define vScu_Timer0WakeupDis()	{SCUWE0&=(~TIMER0WEN);}
#define vScu_Timer1WakeupDis()	{SCUWE0&=(~TIMER1WEN);}
#define vScu_TbcWakeupDis()		{SCUWE0&=(~TBCWEN);}


//REG : SCUWE1
#define WDTWWEN                 (1<<0)
#define SECWEN                  (1<<1)
#define IOWEN                   (1<<2)
#define WTWEN			        (1<<3)
#define SOTWEN			        (1<<4)
#define TIMER2WEN			    (1<<5)
#define TIMER3WEN			    (1<<6)


#define vScu_WdtWakeupEn()	    {SCUWE1|=WDTWWEN;}
#define vScu_SecWakeupEn()	    {SCUWE1|=SECWEN;}
#define vScu_IoWakeupEn()	    {SCUWE1|=IOWEN;}
#define vScu_WtWakeupEn()	    {SCUWE1|=WTWEN;}
#define vScu_SotWakeupEn()	    {SCUWE1|=SOTWEN;}
#define vScu_Timer2WakeupEn()	{SCUWE1|=TIMER2WEN;}
#define vScu_Timer3WakeupEn()	{SCUWE1|=TIMER3WEN;}

#define vScu_WdtWakeupDis()	    {SCUWE1&=(~WDTWWEN);}
#define vScu_SecWakeupDis()	    {SCUWE1&=(~SECWEN);}
#define vScu_IoWakeupDis()	    {SCUWE1&=(~IOWEN);}
#define vScu_WtWakeupDis()	    {SCUWE1&=(~WTWEN);}
#define vScu_SotWakeupDis()	    {SCUWE1&=(~SOTWEN);}
#define vScu_Timer2WakeupDis()	{SCUWE1&=(~TIMER2WEN);}
#define vScu_Timer3WakeupDis()	{SCUWE1&=(~TIMER3WEN);}

//REG : SCUIE0
#define ADCIEN                  (1<<0)
#define GPIOIEN                 (1<<1)
#define UARTIEN                 (1<<2)
#define SM3IEN                  (1<<3)
#define TIMER0IEN               (1<<4)
#define TIMER1IEN               (1<<5)
#define TBCIEN                  (1<<6)

#define vScu_AdcIntEn()		    {SCUIE0|=ADCIEN;}
#define vScu_GpioIntEn()		{SCUIE0|=GPIOIEN;}
#define vScu_UartIntEn()		{SCUIE0|=UARTIEN;}
#define vScu_Sm3IntEn()		    {SCUIE0|=SM3IEN;}
#define vScu_Timer0IntEn()		{SCUIE0|=TIMER0IEN;}
#define vScu_Timer1IntEn()		{SCUIE0|=TIMER1IEN;}
#define vScu_TbcIntEn()		    {SCUIE0|=TBCIEN;}


#define vScu_AdcIntDis()		{SCUIE0&=(~ADCIEN);}
#define vScu_GpioIntDis()		{SCUIE0&=(~GPIOIEN);}
#define vScu_UartIntDis()		{SCUIE0&=(~UARTIEN);}
#define vScu_Sm3IntDisn()		{SCUIE0&=(~SM3IEN);}
#define vScu_Timer0IntDis()		{SCUIE0&=(~TIMER0IEN);}
#define vScu_Timer1IntDis()		{SCUIE0&=(~TIMER1IEN);}
#define vScu_TbcIntDis()		{SCUIE0&=(~TBCIEN);}


//REG : SCUIE1
#define WDTIEN              (1<<0)
#define SECIEN              (1<<1)
#define IOIEN               (1<<2)
#define WTIEN               (1<<3)
#define SOTIEN              (1<<4)
#define TIMER2IEN           (1<<5)
#define TIMER3IEN           (1<<6)
#define vScu_WdtIntEn()	    {SCUIE1|=WDTIEN;}
#define vScu_SecIntEn()	    {SCUIE1|=SECIEN;}
#define vScu_IoIntEn()	    {SCUIE1|=IOIEN;}
#define vScu_WtIntEn()	    {SCUIE1|=WTIEN;}
#define vScu_SotIntEn()	    {SCUIE1|=SOTIEN;}
#define vScu_Timer2IntEn()	{SCUIE1|=TIMER2IEN;}
#define vScu_Timer3IntEn()	{SCUIE1|=TIMER3IEN;}

#define vScu_WdtIntDis()	{SCUIE1&=(~WDTIEN);}
#define vScu_SecIntDis()	{SCUIE1&=(~SECIEN);}
#define vScu_IoIntDis()	    {SCUIE1&=(~IOIEN);}
#define vScu_WtIntDis()	    {SCUIE1&=(~WTIEN);}
#define vScu_SotIntDis()	{SCUIE1&=(~SOTIEN);}
#define vScu_Timer2IntDis()	{SCUIE1&=(~TIMER2IEN);}
#define vScu_Timer3IntDis()	{SCUIE1&=(~TIMER3IEN);}

//REG : SCUIS0
#define ADCSTS              (1<<0)
#define GPIOSTS             (1<<1)
#define UARTSTS             (1<<2)
#define SM3STS              (1<<3)
#define TIMER0STS           (1<<4)
#define TIMER1STS           (1<<5)
#define TBCSTS              (1<<6)
#define TIMER2STS           (1<<7)
//REG : SCUIS1
#define WDTSTS              (1<<0)
#define SECSTS              (1<<1)
#define IOSTS               (1<<2)
#define WTSTS               (1<<3)
#define SOTSTS              (1<<4)
#define WDTRSTSTS           (1<<5)
#define PORENSTS            (1<<6)
#define TIMER3STS           (1<<7)

//REG : SCUCTRL
#define RAMVALID                    (1<<0)
#define UARTMASK                    (1<<1)
#define PDREN                       (1<<2)
//#define USERSTS                   (1<<3)
#define vScu_SetRamValid()	        {SCUCTRL|=RAMVALID;}
#define vScu_UartMask()	            {SCUCTRL|=UARTMASK;}
#define vScu_PdrEn()	            {SCUCTRL|=PDREN;}
//#define vScu_SetUserMode()	    {SCUCTRL|=USERSTS;}

#define vScu_SetRamInvalid()	    {SCUCTRL&=(~RAMVALID);}
#define vScu_PdrDis()	            {SCUCTRL&=(~PDREN);}

//REG : SCURNGC
#define RNGCEN                      (1<<0)
#define RNGCLKSELMASK               (0xf9)
#define SM3CIPHEREN                 (1<<3)
#define vScu_RngcClkEn()	        {SCURNGC|=RNGCEN;}
#define vScu_SetRngcIPClk_1M()	    {SCURNGC&=RNGCLKSELMASK;}
#define vScu_SetRngcIPClk_500K()	{SCURNGC&=RNGCLKSELMASK;SCURNGC|=0x02;}
#define vScu_SetRngcIPClk_250K()	{SCURNGC&=RNGCLKSELMASK;SCURNGC|=0x04;}
#define vScu_Sm3CipherEn()	        {SCURNGC|=SM3CIPHEREN;}

#define vScu_RngcClkDis()	        {SCURNGC&=(~RNGCEN);}
#define vScu_Sm3CipherDis()	        {SCURNGC&=(~SM3CIPHEREN);}


//REG : SCUPC
#define LOWERPOWEROPT_MASK          (0xFE)
#define CRYSTALPOWERCTRL_MASK       (0xE3)

#define LOWERPOWEROPT_VDD           (0<<0)
#define LOWERPOWEROPT_VDD_VTH       (1<<0)

#define CRYSTALPOWER_LOW            (0x00<<2)
#define CRYSTALPOWER_HI             (0x07<<2)
#define vScu_SetCrystalPower_Low()  {SCUPC&=CRYSTALPOWERCTRL_MASK;SCUPC|=CRYSTALPOWER_LOW;}
#define vScu_SetCrystalPower_HI()   {SCUPC&=CRYSTALPOWERCTRL_MASK;SCUPC|=CRYSTALPOWER_HI;}

#define vScu_SetVDD()               {SCUPC&=LOWERPOWEROPT_MASK;SCUPC|=LOWERPOWEROPT_VDD;}
#define vScu_SetVDD_VTH()           {SCUPC&=LOWERPOWEROPT_MASK;SCUPC|=LOWERPOWEROPT_VDD_VTH;} 

//REG:SCUTMRCG
#define TM1_32KEN                 (1<<0)
#define TM1_64KEN                 (1<<1)
#define TM1_500KEN                (1<<2)
#define TM1_2MEN                  (1<<3)
#define TM2_32KEN                 (1<<4)
#define TM2_64KEN                 (1<<5)
#define TM2_500KEN                (1<<6)
#define TM2_2MEN                  (1<<7) 
#define vScu_SetTime1Clk_32K()    (SCUTMRCG &= 0xf0;SCUTMRCG |= TM1_32KEN;)
#define vScu_SetTime1Clk64K()     (SCUTMRCG &= 0xf0;SCUTMRCG |= TM1_64KEN;)
#define vScu_SetTime1Clk500K()    (SCUTMRCG &= 0xf0;SCUTMRCG |= TM1_500KEN;)
#define vScu_SetTime1Clk2M()      (SCUTMRCG &= 0xf0;SCUTMRCG |= TM1_2MEN;)
#define vScu_SetTime2Clk32K()     (SCUTMRCG &= 0x0f;SCUTMRCG |= TM2_32KEN;)
#define vScu_SetTime2Clk64K()     (SCUTMRCG &= 0x0f;SCUTMRCG |= TM2_64KEN;)
#define vScu_SetTime2Clk500K()    (SCUTMRCG &= 0x0f;SCUTMRCG |= TM2_500KEN;)
#define vScu_SetTime2Clk2M()      (SCUTMRCG &= 0x0f;SCUTMRCG |= TM2_2MEN;)

//REG:SCULCDSWEN
#define SCUPOWERIOSTS              (1<<0) 
#define SCULCDEN                   (1<<1)
#define SCUBIASEN                  (1<<2)
#define LDOTRIMSWEN                (1<<3)
#define SCUOXDETEN                 (1<<4)
#define vScu_SetLcdEn()               {SCULCDSWEN|=SCULCDEN;}
#define vScu_SetBiasEn()             {SCULCDSWEN|=SCUBIASEN;}
#define vScu_SetLdoTrimSwEn()         (SCULCDSWEN|=LDOTRIMSWEN;)
#define vScu_SetOxDetEn()             (SCULCDSWEN|=SCUOXDETEN;)


#define vScu_SetLcdDis()               {SCULCDSWEN&=(~SCULCDEN);}
#define vScu_SetBiasDis()              {SCULCDSWEN&=(~SCUBIASEN);}
#define vScu_SetLdoTrimSwDis()         (SCULCDSWEN&=(~LDOTRIMSWEN);)
#define vScu_SetOxDetDis()             (SCULCDSWEN&=(~SCUOXDETEN);)


//REG:SCURAMSWEN
#define SCURAM1KEN                 (1<<0) 
#define SCURAM2KEN                 (1<<1)

#define vScu_Set1KRAMDis()              (SCURAMSWEN|=(SCURAM1KEN);)
#define vScu_Set2KRAMDis()              (SCURAMSWEN|=(SCURAM2KEN);)

#ifdef _SCUFUNC_C_

#else

#endif
#endif
