#ifndef __TYPEDEFINE_H__
#define __TYPEDEFINE_H__
#include "REG_Z8D16R_2.h"


//定义数据类型
typedef unsigned char      BOOL;
typedef unsigned char      UINT8;
typedef unsigned int       UINT16;  //short
typedef unsigned long      UINT32;

typedef signed char        INT8;
typedef signed int         INT16;
typedef signed long        INT32;

typedef unsigned char       U8;
typedef unsigned int        U16;
typedef unsigned long       U32;
typedef signed char         S8;
typedef signed int        	S16;
typedef signed long         S32;

typedef unsigned char	u1; 
typedef unsigned long	u4;				//unsigned long
typedef unsigned short	u2; 

//------------------------------------------------------------------
//                        Definitions
//------------------------------------------------------------------
#define REG32(addr)     (*((UINT32 volatile *)(addr)))
#define REG16(addr)     (*((UINT16 volatile *)(addr)))
#define REG8(addr)      (*((UINT8 volatile  *)(addr)))
#define xREG8(addr)     (*((UINT8 volatile xdata *)(addr)))

#define TRUE   1
#define FALSE  0 

//得到指定地址上的一个8位、16位、32位数据
#define GET_MEM_B( addr )  ( *( (UINT8 *) (addr) ) )
#define GET_MEM_W( addr )  ( *( (UINT16 *) (addr) ) )
#define GET_MEM_DW( addr ) ( *( (UINT32 *) (addr) ) )
//取最大值与最小值
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
//按照小端模式将一个word变为2个字节
#define WORD2BYTE( ray, val ) \
(ray)[0] = ((val) / 256); \
(ray)[1] = ((val) & 0xFF)
//将一个字母变为大写
#define UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )
//16进制和BCD码
#define FROM_BCD(n) ((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n) ((((DWORD)(n) / 10) << 4) | ((DWORD)(n) % 10))

#endif/*__TYPEDEFINE_H__*/