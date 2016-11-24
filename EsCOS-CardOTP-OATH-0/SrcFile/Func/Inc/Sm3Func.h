#ifndef _SM3FUNC_H_
#define _SM3FUNC_H_
#include "REG_Z8D16R_2.h"
#include "intrins.H"

#ifdef _SM3FUNC_C_
void vSm3Hash(void);
#else
extern void vSm3Hash(void);
#endif
#endif