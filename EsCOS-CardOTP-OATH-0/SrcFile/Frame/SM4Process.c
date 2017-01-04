#include "SM4.h"
#include "stddef.h"
#include "string.H"
#include "SM4Process.h"
#include "ScuFunc.h"
#include "Sha1.h"
//#include "SM3.h"

void memxor(void *pu1Output, const void *pu1Input, u2 u2Len)
{
	unsigned char	*pOutput;
	unsigned char	*pInput;
	unsigned short	u2Index;
	
	pOutput = (unsigned char*)pu1Output;
	pInput =  (unsigned char*)pu1Input;
	for ( u2Index = 0; u2Index < u2Len; u2Index++ )
	{
		if ( ((void *)0) == pInput )
		{
			*pOutput++ ^= 0xFF;
		}
		else
		{
			*pOutput++ ^= *pInput++;
		}
	}
	return;
}

void Xrmemcpy(void *pu1Output, const void *pu1Input, unsigned short u2Len)
{
	unsigned char	*pOutput;
	unsigned char	*pInput;
	
	pOutput = (unsigned char*)pu1Output + u2Len;
	pInput = (unsigned char*)pu1Input + u2Len;
	while ( pOutput > (unsigned char*)pu1Output )
	{
		*(--pOutput) = *(--pInput);
	}
	return;
}

#if 1
void AlgSymmMacFun2(u1 * pu1Data, u2 *pu2DataSize, u1 *pu1KeyVal,  u1* pu1IV)	//,u1 u1FinalFlag)
{
	u1			u1BlockSize;
    u2          u2DataSize;

	vScu_SetSysClkOsc2M();
    u2DataSize = *pu2DataSize;

	//vSMS4SetKey(pu1KeyVal,0x10);
	SM4_Init(pu1KeyVal);
	u1BlockSize = 0x10;
	
		while ( u2DataSize >= u1BlockSize )
		{
			memxor(pu1IV,pu1Data,u1BlockSize);

			//if ( ALGID_SMS4 == u1AlgID )
			{
				//vSMS4EncDec(pu1IV,pu1IV,0x00);
				SM4_Crypto(pu1IV,0x10, 0x00,0x00,NULL,pu1IV);
			}


			pu1Data += u1BlockSize;
			u2DataSize -= u1BlockSize;
		}
		// ÅÐ¶ÏÊÇ·ñÊÇ×îºóÒ»¸ö·Ö×é£¬Èç¹ûÊÇ£¬ÔòÐèÒª²¹×ãºó¼ÆËã
		//if ( u1FinalFlag )
		{

			//if ( ALGID_SMS4 == u1AlgID )
			{
				//vSMS4SetKey(pu1KeyVal,0x10);
				SM4_Init(pu1KeyVal);
			}

			memxor(pu1IV,pu1Data,(u1)u2DataSize);
			pu1IV[(u1)u2DataSize] ^= 0x80;

			//if ( ALGID_SMS4 == u1AlgID )
			{
				SM4_Crypto(pu1IV,0x10, 0x00,0x00,NULL,pu1IV);			//Õâ¸ö³¤¶ÈÊÇ16×Ö½ÚÂëå
			}

		}
		//vScu_SetSysClk32K();		//9600_test
		vScu_SetSysClk64K();		
		return;
	
}
#endif

#if 0
u2 GenOtpSeed(u1 *pu1OutSeed,u1 *pu1SeedKey,u1 *pu1MacKey,u1 *pu1RandData)
{
	u1	pu1IV[16];
	u1	pu1Buf[9];
	u2	u2Len;

	// ¼ÆËãMAC
	memset(pu1IV, 0x00, 0x10);
	u2Len = 18;
	AlgSymmMacFun2(pu1RandData, u2Len, pu1MacKey, pu1IV);			//	Ëãmac

	//Byte2Num(pu1Buf, 8, pu1IV, 4);  // 8×Ö½ÚMAC		?
	if (0 != memcmp(pu1RandData+18,pu1Buf+4,4))			//0716  18Òª¸Ä?
	{
		return 1;
	}
	// ¼ÆËã±£´æSEED
	// ÏÈSM3
	u2Len = SM3Hash(pu1RandData, 18, gUserInfo);			//0716  18Òª¸Ä?
	// ÔÙSMS4
	AlgSymmEncDecFun1(pu1OutSeed, &u2Len, pu1SeedKey, ALGID_SMS4, ALG_MODE_ECB, NULL, 0x00, 0x01);
	
	return 0;
}
#endif

#if 0
u1 SM4ENC_TEST(void)				//SM4¼ÓÃÜ×Ô¼ìº¯Êý
{
	u1 i;
	u1 xdata g_u1Key[16] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};		//µÈ´ý¼ÓÃÜµÄÊý¾Ý
	u1 xdata pu1MacKey[16] = {0x53,0x66,0x47,0xcf,0x5a,0xb0,0x51,0x1d,0x81,0x03,0xfe,0xe9,0x74,0x5f,0x17,0x34};	//ÃÜÔ¿
	u1 xdata u1Result[16] = {0x67,0x0f,0x58,0xb1,0x78,0xf1,0x5c,0x3a,0x08,0xc5,0xcb,0xdb,0x49,0x82,0x06,0x10};		//Ó¦¸ÃµÃµ½µÄ½á¹û
	
	SM4_Init(pu1MacKey);		//³õÊ¼»¯ÃÜÔ¿
	SM4_Crypto(g_u1Key,16,0x00,0x00,NULL,g_u1Key);			//¼ÓÃÜ²âÊÔ

	for(i=0;i<16;i++)
	{
		if(g_u1Key[i]!=u1Result[i])
		{
			break;
		}
	}

	if(i==16)
	{
		return 0;
	}
	else
	{
		return 0xff;
	}
}
#endif
u1 SM4DEC_TEST(void)				//SM4½âÃÜ×Ô¼ìº¯Êý
{
	u1 i;
	u1 xdata g_u1Key[16] = {0x67,0x0f,0x58,0xb1,0x78,0xf1,0x5c,0x3a,0x08,0xc5,0xcb,0xdb,0x49,0x82,0x06,0x10};		//µÈ´ý½âÃÜµÄÊý¾Ý
	u1 xdata pu1MacKey[16] = {0x53,0x66,0x47,0xcf,0x5a,0xb0,0x51,0x1d,0x81,0x03,0xfe,0xe9,0x74,0x5f,0x17,0x34};	//ÃÜÔ¿
	u1 xdata u1Result[16] = 	{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};		//Ó¦¸ÃµÃµ½µÄ½á¹û
	SM4_Init(pu1MacKey);		//³õÊ¼»¯ÃÜÔ¿
	SM4_Crypto(g_u1Key,16,0x01,0x00,NULL,g_u1Key);			//½âÃÜ²âÊÔ

	for(i=0;i<16;i++)
	{
		if(g_u1Key[i]!=u1Result[i])
		{
			break;
		}
	}

	if(i==16)
	{
		return 0;
	}
	else
	{
		return 0xff;
	}
}

u1 SHA1_TEST(void)
{
	u1 pu1In[10] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19};
	u1 u1Result[20] = {0x49,0xCA,0xD5,0x1D,0xF8,0x2D,0x5C,0x73,0x97,0xB0,0x06,0xB0,0x6A,0xB1,0x21,0x40,0x1B,0x18,0x63,0x56};
	u2 u2Len = 10;
	u1 pu1O[20];
	u1 i;
	
	SHA1Data(pu1O, pu1In, u2Len);

	for(i=0;i<20;i++)
	{
		if(pu1O[i]!=u1Result[i])
		{
			break;
		}
	}

	if(i==20)
	{
		return 0;
	}
	else
	{
		return 0xff;
	}
	
}
#if 0
u1 SM3_TEST(void)				//SM3×Ô¼ìº¯Êý
{
	u1 i;
	u1 xdata out[32];
	u1 xdata InBuff[64] = 
	{
		0x37,0x93,0x26,0x6b,0xfa,0x6c,0xe4,0xdb,0x6c,0x90,0x34,0x44,0xed,0x2b,0x05,0xa8,
		0xeb,0xdf,0x25,0xc5,0x4f,0x5e,0x76,0x83,0x59,0x73,0x6b,0xfd,0xb5,0xd2,0x90,0x87,
		0xb3,0xb7,0x18,0xb1,0xbc,0x7b,0x10,0x99,0x73,0x1a,0xf2,0xa2,0xb4,0x6a,0xee,0xea,
		0x52,0x57,0x37,0x52,0x12,0x10,0x5c,0xfd,0xb3,0x6b,0x9b,0x6d,0x9b,0x0b,0xb1,0xf6
	};
	u1 u1Result[32] = 
	{
		0x74,0x40,0xFD,0x2A,0xD0,0xA6,0x81,0xC7,0xD7,0x51,0xD7,0x0B,0xBF,0xDE,0x94,0x53,
		0x21,0xD8,0x39,0x1E,0x36,0x12,0x90,0xAB,0x92,0x26,0x93,0xB9,0x30,0x94,0x8E,0xBB
	};

	SM3_Hash(InBuff,64,out);			//¹þÏ£²âÊÔ

	for(i=0;i<32;i++)
	{
		if(out[i]!=u1Result[i])
		{
			break;
		}
	}

	if(i==32)
	{
		return 0;
	}
	else
	{
		return 0xff;
	}
}
#endif

