#ifndef _EWATCHAPP_H_
#define _EWATCHAPP_H_
#include "LCDFRAME.H"



#ifdef _EWATCHAPP_C_
void vUtc_To_GTM8(void);

#else
extern void vUtc_To_GTM8(void);
#endif