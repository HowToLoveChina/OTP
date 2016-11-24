#ifndef _HMAC_H_
#define _HMAC_H_
#include "typedefine.h"

#define BLOCK_LEN  64


// 计算HMAC，输出到pbMAC指向的地址
// 注意：缓冲区长度至少0x14字节
//void HMAC_SHA1(const UINT8* pbKey, UINT32 dwKeyLen, const UINT8* pbMsg, UINT32 dwMsgLen, UINT8* pbMAC);
//void  HashSha1(u1* pu1Out, u2* pu2OutLen, u1* pu1In, u4 u4InLen, u1* pu1Iv, u2 u2IvLen);
long HmacEx(u1 u1HashAlgId, u1* pu1MacKey, u2 u2MacKeyLen, u1* pu1Msg, u4 u4MsgLen, u1* pu1OutMac, u2* pu2OutMacLen);

#endif




