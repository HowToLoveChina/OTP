#ifndef _WDTFRAME_H_
#define _WDTFRAME_H_
#include "WDTFUNC.H"



#ifdef _WDTRAME_C_
UINT8 _WDT_FLAG_;

void vWdtVerify(void);

#else

extern UINT8 _WDT_FLAG_;

extern void vWdtVerify(void);


#endif	
#endif