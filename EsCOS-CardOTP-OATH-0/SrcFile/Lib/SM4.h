#ifndef _SM4_H_
#define _SM4_H_

#include "SM3.h"
//#include "string.H"
#include "typedefine.h"

/*
key 			输入，SM4加解密密钥，长度为16字节
*/
extern void SM4_Init(U8 key[16]);

/*
in 				输入，指向参与加解密运算的明文或密文
inByteLen 		输入，参与加解密运算的明文或密文的字节长度，需用户保证为16的倍数
mode 			输入，加解密模式选择，0--ECB模式，1--CBC模式
iv[16] 			输入，指向长度为16字节的初始向量，仅CBC模式下有效
out 			输出，加解密运算后的输出，长度为inByteLen字节，可以和in指向同一地址
*/
extern U8 SM4_Crypto(U8 *in, U32 inByteLen, U8 En_De, U8 mode, U8 iv[16], U8 *out);

/* 
SM4算法退出函数 
*/
extern void SM4_Close(void);

/*
version[4] 		输出，获取SM4算法库的版本信息，长度为4字节
*/
extern void SM4_Version(U8 version[4]); 

#endif
