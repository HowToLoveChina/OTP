#ifndef _TIMERFRAME_H_
#define _TIMERFRAME_H_
#include "TIMERFUNC.H"

#ifdef 	_TIMERFRAME_C_
//UINT16 _TIMER_FLAG_;
void vTimerVerify(void);

#else

//extern UINT16 _TIMER_FLAG_;
extern void vTimerVerify(void);
#endif
#endif