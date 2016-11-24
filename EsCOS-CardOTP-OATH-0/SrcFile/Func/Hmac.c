//#include "StdAfx.h"
#include "hmac.h"
#include "sha1.h"
#include "string.h"
#include "OtpFunc.h"
#include "SM4Process.h"

#if 0
//////////////////////////////////////////////////////////////////////////
// 函数功能
//	计算Sha1值
// 
// 函数参数
//	pu1Out:			输出参数，哈希结果缓冲区
//	pu2OutLen:		输出参数，哈希结果长度
//	pu1In:			输入参数，参与哈希的数据缓冲区
//	u4InLen:		输入参数，哈希数据长度
//	pu1Iv:			输入参数，参与哈希的数据缓冲区
//	u2IvLen:		输入参数，哈希数据长度	
// 返回值
//	无
//
// 备注
//////////////////////////////////////////////////////////////////////////
void  HashSha1(u1* pu1Out, u2* pu2OutLen, u1* pu1In, u4 u4InLen, u1* pu1Iv, u2 u2IvLen)
{
	sha		blkSHA;	
	u4		i;
	
	sha1_init(&blkSHA);
	if( NULL != pu1Iv && 0x14 == u2IvLen )
	{
		(&blkSHA)->h[0] = BYTE2LONG(pu1Iv);
		(&blkSHA)->h[1] = BYTE2LONG(pu1Iv+0x04);
		(&blkSHA)->h[2] = BYTE2LONG(pu1Iv+0x08);
		(&blkSHA)->h[3] = BYTE2LONG(pu1Iv+0x0C);
		(&blkSHA)->h[4] = BYTE2LONG(pu1Iv+0x10);
	}
	
	for ( i = 0 ; i < u4InLen; i++ )
	{
		if ( 0x00 == (i%0x40) )
		{
			int a;
			
			a = 0;
		}
		sha1_process(&blkSHA, pu1In[i]);
	}
	sha1_hash(&blkSHA, (char*)pu1Out);
	*pu2OutLen = 0x14;
}


//////////////////////////////////////////////////////////////////////////
// 函数功能
//	计算Sm3值
// 
// 函数参数
//	pu1Out:			输出参数，哈希结果缓冲区
//	pu2OutLen:		输出参数，哈希结果长度
//	pu1In:			输入参数，参与哈希的数据缓冲区
//	u4InLen:		输入参数，哈希数据长度
//	u1HashAlgId:	输入参数，哈希算法号 C0： MD5， C1：SHA1， C2： SHA256， C3：SHA348， C4：SHA512， C5：SM3
//
// 返回值
//	无
//
// 备注
//////////////////////////////////////////////////////////////////////////
void HashMsg(u1* pu1Out, u2* pu2OutLen, u1* pu1In, u4 u4InLen, u1 u1HashAlgId)
{
	if ( HASH_ALGID_MD5 == u1HashAlgId )
	{
		HashMd5(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else if ( HASH_ALGID_SHA1 == u1HashAlgId )
	{
		HashSha1(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else if ( HASH_ALGID_SHA256 == u1HashAlgId )
	{
		HashSha256(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else if ( HASH_ALGID_SHA384 == u1HashAlgId )
	{
		HashSha384(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else if ( HASH_ALGID_SHA512 == u1HashAlgId )
	{
		HashSha512(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else if ( HASH_ALGID_SM3 == u1HashAlgId )
	{
		HashSm3(pu1Out, pu2OutLen, pu1In, u4InLen, NULL, 0x00);
	}
	else
	{
		*pu2OutLen = 0x00;
	}
}
#endif

#if 0
// 计算HMAC，输出到pbMAC指向的地址
// 注意：缓冲区长度至少0x14字节
void HMAC_SHA1(const UINT8* pbKey, UINT32 dwKeyLen, const UINT8* pbMsg, UINT32 dwMsgLen, UINT8* pbMAC)
{    
	UINT8 i;
	sha sh;
	char hash[20];
	
	char K_IPad[BLOCK_LEN] = {0};
	char K_OPad[BLOCK_LEN] = {0}; 
	char K[BLOCK_LEN] = {0};	
	
	if (dwKeyLen > BLOCK_LEN)
	{
		ComputeSHA1(pbKey, dwKeyLen, K);
	}
	else
	{
		memcpy(K, pbKey, dwKeyLen);
	}
	memcpy(K_IPad, K, BLOCK_LEN);
	memcpy(K_OPad, K, BLOCK_LEN);
	for (i = 0; i < BLOCK_LEN; i++) // 异或
	{
		K_IPad[i] ^= 0x36;
		K_OPad[i] ^= 0x5C;
	}
	
	sha1_init(&sh);
	for (i = 0; i < BLOCK_LEN; i++)
	{
		sha1_process(&sh, K_IPad[i]);
	}
	for (i = 0; i < dwMsgLen; i++)
	{
		sha1_process(&sh, pbMsg[i]);
	}
	sha1_hash(&sh, hash);
	
	sha1_init(&sh);
	for (i = 0; i < BLOCK_LEN; i++)
	{
		sha1_process(&sh, K_OPad[i]);
	}
	for (i = 0; i < sizeof(hash); i++)
	{
		sha1_process(&sh, hash[i]);
	}
	sha1_hash(&sh, hash);
	
	memcpy(pbMAC, hash, sizeof(hash));
}
#endif


// 交行Mini终端计算Hmac的步骤
//	ipad:	填充字节串，内容为：8位字节0x36 重复B次
//	opad:	填充字节串，内容为：8位字节0x5c 重复B次
//	text:	所输入的需要计算MAC的数据，不包括填充字节串
//	K	:	MAC密钥
//	t	:	所得MAC的字节长度
//	B	:	哈希算法分组长度
// 使用如下公式计算输入数据text的MAC值：
//	MAC(text)t = HMAC(K, text)t = HASH((K0⊕opad) || HASH((K0⊕ipad) || text) )
// 具体描述如下：
// 1.	若K = B，令K0 = K。跳转到步骤4；
// 2.	若K > B，令K0 = HASH(K)。跳转到步骤3；
// 3.	若K < B，则在K末尾补字节0x00产生B字节K0；
// 4.	K0与ipad异或产生B字节字符串：K0⊕ipad；
// 5.	将text追加到步骤4得到的字符串K0⊕ipad末尾：(K0⊕ipad)||text；
// 6.	对步骤5得到的字符串进行哈希得到：HASH((K0⊕ipad)||text)；
// 7.	K0与opad异或：K0⊕opad；
// 8.	将步骤6产生的结果追加到步骤7的结果末尾：(K0 ⊕ opad ) ||HASH((K0 ⊕ ipad) || text )
// 9.	对步骤8的结果做哈希得到：HASH((K0⊕opad) || HASH((K0 ⊕ ipad) || text))。
// 10.	步骤9得到的哈希值作为MAC值。

//////////////////////////////////////////////////////////////////////////
// 函数功能
//	计算Hmac值，使用软算法实现
//
// 函数参数
//	u1HashAlgId:			输入参数，哈希算法号 C0： MD5， C1：SHA1， C2： SHA256， C3：SHA348， C4：SHA512， C5：SM3
//	pu1MacKey：				输入参数，Mac密钥缓冲区
//	u2MacKeyLen:			输入参数，Mac密钥长度
//	pu1Msg：				输入参数，消息数据缓冲区
//	u4MsgLen:				输入参数，消息长度
//	pu1OutMac:				输出参数，Hmac结果的缓冲区
//	pu2OutMacLen:			输出参数，Hmac结果的长度
//
// 返回值
//	00:						成功
//	其它值：				失败
//
//	备注：
//
//////////////////////////////////////////////////////////////////////////
long HmacEx(u1 u1HashAlgId, u1* pu1MacKey, u2 u2MacKeyLen, u1* pu1Msg, u4 u4MsgLen, u1* pu1OutMac, u2* pu2OutMacLen)
{
	//long					lResult;
	u1		xdata				pu1Ipad[0x40] = {0x00};
	u1		xdata				pu1Opad[0x40] = {0x00};
	u1		xdata				pu1MacKeyBuf[0x40] = {0x00};
	u2		xdata				u2Outlen;
	u1		xdata				pu1KeyXorIpod[0x40] = {0x00};
	u1		xdata				pu1KeyXorOpod[0x80] = {0x00};
	u1		xdata				u1BlockLen;
	u1		xdata				pu1TempIn[80];
	u4		xdata				u4HashLen;

	//pu1TempIn = NULL;
	if ( u1HashAlgId < 0xC0 || u1HashAlgId > 0xC5 )
	{
		return -1;
	}
	#if 1
	u1BlockLen = 0x40;
	//if ( HASH_ALGID_SHA384 == u1HashAlgId || HASH_ALGID_SHA512 == u1HashAlgId )
	//{
	//	u1BlockLen = 0x80;
	//}
	memset(pu1Ipad, 0x36, u1BlockLen);
	memset(pu1Opad, 0x5C, u1BlockLen);
	// 处理Mac密钥，使其结果为B字节
	if ( u2MacKeyLen > u1BlockLen )
	{
		u2Outlen = sizeof(pu1MacKeyBuf);
		//HashMsg(pu1MacKeyBuf, &u2Outlen, pu1MacKey, u2MacKeyLen, u1HashAlgId);
		//HashSha1(pu1MacKeyBuf, &u2Outlen, pu1MacKey, u2MacKeyLen, NULL, 0x00);
		SHA1Data(pu1MacKeyBuf, pu1MacKey, u2MacKeyLen);
		u2MacKeyLen = 0x14;
	}
	else
	{
		memcpy(pu1MacKeyBuf, pu1MacKey, u2MacKeyLen);
	}
	memset(pu1MacKeyBuf + u2MacKeyLen, 0x00, ((u2)u1BlockLen - u2MacKeyLen));
	// 4.	K0与ipad异或产生64字节字符串：K0⊕ipad；
	memcpy(pu1KeyXorIpod, pu1MacKeyBuf, u1BlockLen);
	memcpy(pu1KeyXorOpod, pu1MacKeyBuf, u1BlockLen);
	memxor(pu1KeyXorIpod, pu1Ipad, u1BlockLen);

	// 5.	将text追加到步骤4得到的字符串K0⊕ipad末尾：(K0⊕ipad)||text；
	//pu1TempIn = (u1*)malloc(u4MsgLen + 0x90);
	//if ( NULL == pu1TempIn )
	//{
	//	lResult = GetLastError();
	//	return lResult;
	//}
	memcpy(pu1TempIn, pu1KeyXorIpod, u1BlockLen);
	memcpy(pu1TempIn + u1BlockLen, pu1Msg, u4MsgLen);
	u4HashLen = u4MsgLen + u1BlockLen;
	//u2Outlen = sizeof(pu1MacKeyBuf);
	#endif
	// 6.	对步骤5得到的字符串进行哈希得到：HASH((K0⊕ipad)||text)；
	//HashMsg(pu1MacKeyBuf, &u2Outlen, pu1TempIn, u4HashLen, u1HashAlgId);
	//pu1TempIn[0] = 1;
	//pu1TempIn[1] = 2;
	//pu1TempIn[2] = 3;
	//pu1TempIn[3] = 4;
	//pu1TempIn[4] = 5;
	//pu1TempIn[5] = 6;
	//u4HashLen = 6;
	//while(1);
	//HashSha1(pu1MacKeyBuf, &u2Outlen, pu1TempIn, u4HashLen, NULL, 0x00);
	SHA1Data(pu1MacKeyBuf, pu1TempIn, u4HashLen);
	u2Outlen = 0x14;
	// 7.	K0与opad异或：K0⊕opad；
	memxor(pu1KeyXorOpod, pu1Opad, u1BlockLen);

	// 8.	将步骤6产生的结果追加到步骤7的结果末尾：(K0 ⊕ opad ) ||HASH((K0 ⊕ ipad) || text )
	memcpy(pu1KeyXorOpod + u1BlockLen, pu1MacKeyBuf, u2Outlen);
	u4HashLen = u1BlockLen + u2Outlen;

	// 9.	对步骤8的结果做哈希得到：HASH((K0⊕opad) || HASH((K0 ⊕ ipad) || text))。
	//u2Outlen = sizeof(pu1MacKeyBuf);
	//HashMsg(pu1OutMac, &u2Outlen, pu1KeyXorOpod, u4HashLen, u1HashAlgId);
	//while(1);
	//HashSha1(pu1OutMac, &u2Outlen, pu1KeyXorOpod, u4HashLen, NULL, 0x00);
	SHA1Data(pu1OutMac, pu1KeyXorOpod, u4HashLen);

	// 10.	步骤9得到的哈希值作为MAC值。
	
	*pu2OutMacLen = 20;			//u2Outlen;

	//if ( NULL != pu1TempIn )
	//{
	//	free(pu1TempIn);
	//}
	return 0x00;
}




