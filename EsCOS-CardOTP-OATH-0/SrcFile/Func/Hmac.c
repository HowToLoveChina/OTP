//#include "StdAfx.h"
#include "hmac.h"
#include "sha1.h"
#include "string.h"
#include "OtpFunc.h"
#include "SM4Process.h"

#if 0
//////////////////////////////////////////////////////////////////////////
// ��������
//	����Sha1ֵ
// 
// ��������
//	pu1Out:			�����������ϣ���������
//	pu2OutLen:		�����������ϣ�������
//	pu1In:			��������������ϣ�����ݻ�����
//	u4InLen:		�����������ϣ���ݳ���
//	pu1Iv:			��������������ϣ�����ݻ�����
//	u2IvLen:		�����������ϣ���ݳ���	
// ����ֵ
//	��
//
// ��ע
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
// ��������
//	����Sm3ֵ
// 
// ��������
//	pu1Out:			�����������ϣ���������
//	pu2OutLen:		�����������ϣ�������
//	pu1In:			��������������ϣ�����ݻ�����
//	u4InLen:		�����������ϣ���ݳ���
//	u1HashAlgId:	�����������ϣ�㷨�� C0�� MD5�� C1��SHA1�� C2�� SHA256�� C3��SHA348�� C4��SHA512�� C5��SM3
//
// ����ֵ
//	��
//
// ��ע
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
// ����HMAC�������pbMACָ��ĵ�ַ
// ע�⣺��������������0x14�ֽ�
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
	for (i = 0; i < BLOCK_LEN; i++) // ���
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


// ����Mini�ն˼���Hmac�Ĳ���
//	ipad:	����ֽڴ�������Ϊ��8λ�ֽ�0x36 �ظ�B��
//	opad:	����ֽڴ�������Ϊ��8λ�ֽ�0x5c �ظ�B��
//	text:	���������Ҫ����MAC�����ݣ�����������ֽڴ�
//	K	:	MAC��Կ
//	t	:	����MAC���ֽڳ���
//	B	:	��ϣ�㷨���鳤��
// ʹ�����¹�ʽ������������text��MACֵ��
//	MAC(text)t = HMAC(K, text)t = HASH((K0��opad) || HASH((K0��ipad) || text) )
// �����������£�
// 1.	��K = B����K0 = K����ת������4��
// 2.	��K > B����K0 = HASH(K)����ת������3��
// 3.	��K < B������Kĩβ���ֽ�0x00����B�ֽ�K0��
// 4.	K0��ipad������B�ֽ��ַ�����K0��ipad��
// 5.	��text׷�ӵ�����4�õ����ַ���K0��ipadĩβ��(K0��ipad)||text��
// 6.	�Բ���5�õ����ַ������й�ϣ�õ���HASH((K0��ipad)||text)��
// 7.	K0��opad���K0��opad��
// 8.	������6�����Ľ��׷�ӵ�����7�Ľ��ĩβ��(K0 �� opad ) ||HASH((K0 �� ipad) || text )
// 9.	�Բ���8�Ľ������ϣ�õ���HASH((K0��opad) || HASH((K0 �� ipad) || text))��
// 10.	����9�õ��Ĺ�ϣֵ��ΪMACֵ��

//////////////////////////////////////////////////////////////////////////
// ��������
//	����Hmacֵ��ʹ�����㷨ʵ��
//
// ��������
//	u1HashAlgId:			�����������ϣ�㷨�� C0�� MD5�� C1��SHA1�� C2�� SHA256�� C3��SHA348�� C4��SHA512�� C5��SM3
//	pu1MacKey��				���������Mac��Կ������
//	u2MacKeyLen:			���������Mac��Կ����
//	pu1Msg��				�����������Ϣ���ݻ�����
//	u4MsgLen:				�����������Ϣ����
//	pu1OutMac:				���������Hmac����Ļ�����
//	pu2OutMacLen:			���������Hmac����ĳ���
//
// ����ֵ
//	00:						�ɹ�
//	����ֵ��				ʧ��
//
//	��ע��
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
	// ����Mac��Կ��ʹ����ΪB�ֽ�
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
	// 4.	K0��ipad������64�ֽ��ַ�����K0��ipad��
	memcpy(pu1KeyXorIpod, pu1MacKeyBuf, u1BlockLen);
	memcpy(pu1KeyXorOpod, pu1MacKeyBuf, u1BlockLen);
	memxor(pu1KeyXorIpod, pu1Ipad, u1BlockLen);

	// 5.	��text׷�ӵ�����4�õ����ַ���K0��ipadĩβ��(K0��ipad)||text��
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
	// 6.	�Բ���5�õ����ַ������й�ϣ�õ���HASH((K0��ipad)||text)��
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
	// 7.	K0��opad���K0��opad��
	memxor(pu1KeyXorOpod, pu1Opad, u1BlockLen);

	// 8.	������6�����Ľ��׷�ӵ�����7�Ľ��ĩβ��(K0 �� opad ) ||HASH((K0 �� ipad) || text )
	memcpy(pu1KeyXorOpod + u1BlockLen, pu1MacKeyBuf, u2Outlen);
	u4HashLen = u1BlockLen + u2Outlen;

	// 9.	�Բ���8�Ľ������ϣ�õ���HASH((K0��opad) || HASH((K0 �� ipad) || text))��
	//u2Outlen = sizeof(pu1MacKeyBuf);
	//HashMsg(pu1OutMac, &u2Outlen, pu1KeyXorOpod, u4HashLen, u1HashAlgId);
	//while(1);
	//HashSha1(pu1OutMac, &u2Outlen, pu1KeyXorOpod, u4HashLen, NULL, 0x00);
	SHA1Data(pu1OutMac, pu1KeyXorOpod, u4HashLen);

	// 10.	����9�õ��Ĺ�ϣֵ��ΪMACֵ��
	
	*pu2OutMacLen = 20;			//u2Outlen;

	//if ( NULL != pu1TempIn )
	//{
	//	free(pu1TempIn);
	//}
	return 0x00;
}




