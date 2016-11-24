#ifndef _RNGFRAME_H_
#define _RNGFRAME_H_
#include "RNGFUNC.H"



#ifdef _RNGRAME_C_
UINT8 _RNG_FLAG_;

void vRngVerify(void);

#else

extern UINT8 _RNG_FLAG_;

extern void vRngVerify(void);


#endif	
#endif