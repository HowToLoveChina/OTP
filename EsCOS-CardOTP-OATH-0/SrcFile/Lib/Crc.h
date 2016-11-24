#ifndef _CRC_H_
#define _CRC_H_
#include "REG_Z8D16R_2.h"

#define CRC_DFE_POLY    0x8005


#ifdef _CRC_C_

UINT16 u16CrcCompute(UINT16 addr, UINT16 len, UINT16 crc);
UINT8 ucEndianChange(UINT8 inchar);

#else

extern UINT16 u16CrcCompute(UINT16 addr, UINT16 len, UINT16 crc);
extern UINT8 ucEndianChange(UINT8 inchar);


#endif	
#endif