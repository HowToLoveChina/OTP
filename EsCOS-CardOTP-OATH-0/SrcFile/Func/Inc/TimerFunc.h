#ifndef _TIMERFUNC_H_
#define _TIMERFUNC_H_
#include "REG_Z8D16R_2.h"

#define TIMER_MODE0 (0)
#define TIMER_MODE1 (1)
#define TIMER_MODE2 (2)

#define TIMER0 (0)
#define TIMER1 (1)
#define TIMER2 (2)
#define TIMER3 (3)

#define TIMER_CLK_32K  (0)
#define TIMER_CLK_64K  (1)
#define TIMER_CLK_500K (2)
#define TIMER_CLK_2m   (3)

#define vTimer1_SetMode0()         {TM1MOD=0x00;} //两个独立8位TIMER
#define vTimer1_SetMode2()         {TM1MOD=0x02;} //1个16位TIMER
#define vTimer1_SetMode1()         {TM1MOD=0x01;} //自动加载8位TIMER

#define vTimer2_SetMode0()         {TM2MOD=0x00;}
#define vTimer2_SetMode2()         {TM2MOD=0x02;}
#define vTimer2_SetMode1()         {TM2MOD=0x01;}

#define vTimer0_CounterClr()       {TM0CON|=0x02;}
#define vTimer1_CounterClr()       {TM1CON|=0x02;}
#define vTimer2_CounterClr()       {TM2CON|=0x02;}
#define vTimer3_CounterClr()       {TM3CON|=0x02;}

#define vTimer0_Start()           {TM0CON=0x01;}
#define vTimer1_Start()           {TM1CON=0x01;}
#define vTimer2_Start()           {TM2CON=0x01;}
#define vTimer3_Start()           {TM3CON=0x01;}

#define vTimer0_Stop()            {TM0CON=0x00;}
#define vTimer1_Stop()            {TM1CON=0x00;}
#define vTimer2_Stop()            {TM2CON=0x00;}
#define vTimer3_Stop()            {TM3CON=0x00;}

#define T0INT (1<<0)
#define T1INT (1<<1)

void vTimerClkSel(UINT8 TimerNum,UINT8 TimerClk);

#ifdef _TIMERFUNC_C_
void vTimer1Start(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk);
void vTimer1Start1(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk);
#else
extern void vTimer1Start(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk);
extern void vTimer1Start1(UINT8 TimerNum,UINT8 Mode,UINT16 InitValue,UINT8 TimerClk);
#endif
#endif