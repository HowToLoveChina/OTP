#define _CRC_C_
#include "CRC.H"
#include "OtpFunc.H"
#include "DEBUG.H"



/********************************************************************
Function name:      u16CrcCompute
Input parameters:
    addr, XBUS ADDRESS of the data;
    len, the length of the data;
    crc, crc initialization value;
Return:
    crc value;
Function:
    CRC value direct compute, poly given as defined;
********************************************************************/
UINT16 u16CrcCompute(UINT16 addr, UINT16 len, UINT16 crc)
{
    UINT16 i, j;
    UINT8 uc;
    for ( j = 0; j < len; j++)
    {
        uc = CBYTE[addr + j];		
        for ( i = 0; i < 8; i++)
        {
            crc = ((uc ^ (UINT8)(crc >> 8)) & 0x80) ? ((crc << 1) ^ CRC_DFE_POLY) : (crc << 1);
            uc <<= 1;
        }
    }

    return crc;

}

/********************************************************************
Function name:      ucEndianChange
Input parameters:
    inc, inchar;
Return:
    outc, endian change of inchar;
Function:
    endian change of the in BYTE;
********************************************************************/
#pragma REGPARMS
UINT8 ucEndianChange(UINT8 inchar)
{
    inchar = 0;
#pragma asm
    CLR A
    MOV B, A
    MOV R0, #08H
    MOV A, R7
    CLR C
ENDIANLOOP:
    RLC A
    XCH A, B
    RRC A
    XCH A, B
    DJNZ R0, ENDIANLOOP

    XCH A, B
    MOV R7, A
#pragma endasm
    return ACC;
}





