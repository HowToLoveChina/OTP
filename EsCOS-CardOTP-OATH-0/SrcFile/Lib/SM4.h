#ifndef _SM4_H_
#define _SM4_H_

#include "SM3.h"
//#include "string.H"
#include "typedefine.h"

/*
key 			���룬SM4�ӽ�����Կ������Ϊ16�ֽ�
*/
extern void SM4_Init(U8 key[16]);

/*
in 				���룬ָ�����ӽ�����������Ļ�����
inByteLen 		���룬����ӽ�����������Ļ����ĵ��ֽڳ��ȣ����û���֤Ϊ16�ı���
mode 			���룬�ӽ���ģʽѡ��0--ECBģʽ��1--CBCģʽ
iv[16] 			���룬ָ�򳤶�Ϊ16�ֽڵĳ�ʼ��������CBCģʽ����Ч
out 			������ӽ������������������ΪinByteLen�ֽڣ����Ժ�inָ��ͬһ��ַ
*/
extern U8 SM4_Crypto(U8 *in, U32 inByteLen, U8 En_De, U8 mode, U8 iv[16], U8 *out);

/* 
SM4�㷨�˳����� 
*/
extern void SM4_Close(void);

/*
version[4] 		�������ȡSM4�㷨��İ汾��Ϣ������Ϊ4�ֽ�
*/
extern void SM4_Version(U8 version[4]); 

#endif
