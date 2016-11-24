#ifndef _TBCFRAME_H_
#define _TBCFRAME_H_
#include "TBCFUNC.H"

#ifdef 	_TBCFRAME_C_
//UINT32  _TBC_FLAG_;
void vTbcVerify(void);

#else
extern void vTbcVerify(void);
//extern UINT32  _TBC_FLAG_;


#endif
#endif