#ifndef _SCUFRAME_H_
#define _SCUFRAME_H_
#include "SCUFUNC.H"



#ifdef _SCURAME_C_
UINT32 _SCU_FLAG_;

void vScuVerify(void);


#else

extern UINT32 _SCU_FLAG_;

extern void vScuVerify(void);



#endif	
#endif