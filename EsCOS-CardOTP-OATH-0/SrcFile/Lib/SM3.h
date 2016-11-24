/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2012 All Rights Reserved
 ********************************************************************
File: SM3.h
History:
Version                      Author                     Date

Description:
     SM3 Header File
Function list:
     S8 SM3Init(SM3Context *context);
     S8 SM3Input(SM3Context *context, U8 *message, U32 byteLen);
     S8 SM3Result(SM3Context *context, U8 digest[32]);
     S8 SM3Hash(U8 *in, U32 inByteLen, U8 digest[32]);
	 void SM3_Version(unsigned char version[4]); 
Special statement:
     NULL
********************************************************************/

#ifndef _SM3_H_
#define _SM3_H_

#include "typedefine.h"

//-------------------------------------------------------------------
//                      Headers
//-------------------------------------------------------------------

#if 0
#define ARAM_BASE       (0x0B80)
#define SM3_A_BASE 		(&XBYTE[ARAM_BASE])
#define SM3_MSG_BASE 	(&XBYTE[ARAM_BASE + 32])
#define SM3_T_BASE 		(&XBYTE[ARAM_BASE + 96])
#define SM3_VA_BASE 	SM3_MSG_BASE 
#endif

// #define ARAM_BASE       (0x0380)
// #define SM3_A_BASE 		(&XBYTE[ARAM_BASE])
// #define SM3_MSG_BASE 	(&XBYTE[ARAM_BASE + 32])
// #define SM3_T_BASE 		(&XBYTE[ARAM_BASE + 96])
// #define SM3_VA_BASE 	SM3_MSG_BASE

#ifndef _HASH_enum_
#define _HASH_enum_
enum
{
    HashSuccess = 0, 
	HashNull,  /* 空指示参量 */
    HashInputTooLong,  /* 输入数据太长提示 */
    HashStateError /* called Input after Result --以输入结果命名之 */
};
#endif

typedef struct SM3Context
{
    U32 byteLen[2];
	U8 *Message_Block;//U8  Message_Block[64];
    U8  Message_Block_Index;
    S8  Corrupted;
} SM3Context;


/*
 context          输入，指向SM3Context结构体的指针
 */
S8 SM3_Init(SM3Context *context);

/*
context         输入，指向SM3Context结构的指针
message         输入，指要杂凑的信息
byteLen         输入，指杂凑信息的字节长度
*/
S8 SM3_Input(SM3Context *context, U8 *message, U32 byteLen);

/*
context         输入，指向SM3Context结构的指针
digest[32]      输出，哈希值，长度为32个字节
*/
S8 SM3_Result(SM3Context *context, U8 digest[32]);

/*
in              输入，指要杂凑的信息
inByteLen       输入，指杂凑信息的字节长度
digest[32]      输出，哈希值，长度为32个字节
*/
S8 SM3_Hash(U8 *in, U32 inByteLen, U8 digest[32]); 

/*
version[4]      输出，SM3算法库版本获取函数
*/
void SM3_Version(U8 version[4]);  


#endif /*_SM3_H_*/
