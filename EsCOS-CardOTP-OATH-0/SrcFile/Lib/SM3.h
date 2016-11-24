/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009��2012 All Rights Reserved
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
	HashNull,  /* ��ָʾ���� */
    HashInputTooLong,  /* ��������̫����ʾ */
    HashStateError /* called Input after Result --������������֮ */
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
 context          ���룬ָ��SM3Context�ṹ���ָ��
 */
S8 SM3_Init(SM3Context *context);

/*
context         ���룬ָ��SM3Context�ṹ��ָ��
message         ���룬ָҪ�Ӵյ���Ϣ
byteLen         ���룬ָ�Ӵ���Ϣ���ֽڳ���
*/
S8 SM3_Input(SM3Context *context, U8 *message, U32 byteLen);

/*
context         ���룬ָ��SM3Context�ṹ��ָ��
digest[32]      �������ϣֵ������Ϊ32���ֽ�
*/
S8 SM3_Result(SM3Context *context, U8 digest[32]);

/*
in              ���룬ָҪ�Ӵյ���Ϣ
inByteLen       ���룬ָ�Ӵ���Ϣ���ֽڳ���
digest[32]      �������ϣֵ������Ϊ32���ֽ�
*/
S8 SM3_Hash(U8 *in, U32 inByteLen, U8 digest[32]); 

/*
version[4]      �����SM3�㷨��汾��ȡ����
*/
void SM3_Version(U8 version[4]);  


#endif /*_SM3_H_*/
