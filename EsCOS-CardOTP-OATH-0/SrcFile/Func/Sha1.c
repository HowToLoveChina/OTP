//#include "StdAfx.h"
#include "SHA1.H"
#include "string.h"

#if 0
static void sha1_transform(sha *sh)
{ /* basic transformation step */
    unsigned int a,b,c,d,e,temp;
    int t;

    for (t=16;t<80;t++) sh->w[t]=S(1,sh->w[t-3]^sh->w[t-8]^sh->w[t-14]^sh->w[t-16]);
    a=sh->h[0]; b=sh->h[1]; c=sh->h[2]; d=sh->h[3]; e=sh->h[4];
    for (t=0;t<20;t++)
    { /* 20 times - mush it up */
        temp=K0+F0(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=20;t<40;t++)
    { /* 20 more times - mush it up */
        temp=K1+F1(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=40;t<60;t++)
    { /* 20 more times - mush it up */
        temp=K2+F2(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    for (t=60;t<80;t++)
    { /* 20 more times - mush it up */
        temp=K3+F3(b,c,d)+S(5,a)+e+sh->w[t];
        e=d; d=c;
        c=S(30,b);
        b=a; a=temp;
    }
    sh->h[0]+=a; sh->h[1]+=b; sh->h[2]+=c;
    sh->h[3]+=d; sh->h[4]+=e;
} 

void sha1_init(sha *sh)
{ /* re-initialise */
    int i;
	
    for (i=0;i<80;i++) sh->w[i]=0L;
    sh->length[0]=sh->length[1]=0L;
	
    sh->h[0]=H0;
    sh->h[1]=H1;
    sh->h[2]=H2;
    sh->h[3]=H3;
    sh->h[4]=H4;
}

void sha1_process(sha *sh,int byte)
{ /* process the next message byte */
    int cnt;
    
    cnt=(int)((sh->length[0]/32)%16);
    
    sh->w[cnt]<<=8;
    sh->w[cnt]|=(unsigned int)(byte&0xFF);

    sh->length[0]+=8;
    if (sh->length[0]==0L) { sh->length[1]++; sh->length[0]=0L; }
    if ((sh->length[0]%512)==0) sha1_transform(sh);
}

void sha1_hash(sha *sh,char hash[20])
{ /* pad message and finish - supply digest */
    int i;
    unsigned int len0,len1;
    len0=sh->length[0];
    len1=sh->length[1];
    sha1_process(sh,PAD);
    while ((sh->length[0]%512)!=448) sha1_process(sh,ZERO);
    sh->w[14]=len1;
    sh->w[15]=len0;    
    sha1_transform(sh);
    for (i=0;i<20;i++)
    { /* convert to bytes */
        hash[i]=((sh->h[i/4]>>(8*(3-i%4))) & 0xffL);
    }
    sha1_init(sh);
}

// 计算SHA值，输出到pbHashValue指向的地址
// 注意：缓冲区长度至少0x14字节
void ComputeSHA1(const UINT8* pbData, UINT32 dwDataLen, UINT8* pbHashValue)
{
	sha sh;
	char hash[20];
	UINT32 i;

	sha1_init(&sh);
	for (i=0; i < dwDataLen; i++)
	{
		sha1_process(&sh, pbData[i]);
	}
	sha1_hash(&sh, hash);
	memcpy(pbHashValue, hash, sizeof(hash));
}

#endif

#define BYTE2LONG(pu1Data)			(((u4)((pu1Data)[0])<<24) + ((u4)((pu1Data)[1])<<16) + ((u2)((pu1Data)[2])<<8) + (u1)((pu1Data)[3]))
#define BYTE2WORD(pu1Data)			((u2)(((u2)((pu1Data)[0])<<8) + (u1)((pu1Data)[1])))
#define LONG2BYTE(pu1Data,u4Data)	((u1*)(pu1Data))[0] = (u1)((u4Data)>>24);((u1*)(pu1Data))[1] = (u1)((u4Data)>>16);((u1*)pu1Data)[2] = (u1)((u4Data)>>8);((u1*)pu1Data)[3] = (u1)((u4Data))
//#define WORD2BYTE(pu1Data,u2Data)	((u1*)(pu1Data))[0] = (u1)((u2Data)>>8);((u1*)(pu1Data))[1] = (u1)((u2Data))



#ifdef _BIG_ENDIAN_
#define	longReverse(pvDst, pvSrc, u4Count)	memcpy(pvDst, pvSrc, u4Count<<2)
#else
void longReverse(void xdata* pvDst, const void xdata* pvSrc, u4 u4Count)
{
	u4	i;
	u1	buf[4],tmp[4];
	
	u4Count <<= 0x02;

	for ( i = 0x00; i < u4Count; i+=0x04)
	{
		memcpy(buf, (u1*)pvSrc + i, 4);
		tmp[0]=buf[3];
		tmp[1]=buf[2];
		tmp[2]=buf[1];
		tmp[3]=buf[0];
		memcpy((u1*)pvDst + i,tmp,4);
	}
}
#endif

const   u1  code g_pu1Sha1DefaultIV[20] =
{
	0x67, 0x45, 0x23, 0x01, 0xef, 0xcd, 0xab, 0x89, 0x98, 0xba, 
	0xdc, 0xfe, 0x10, 0x32, 0x54, 0x76, 0xc3, 0xd2, 0xe1, 0xf0
};

void vSha1GetDefaultIv(void xdata* pvIv)
{
	memcpy(pvIv, g_pu1Sha1DefaultIV, 0x14);// T21移植时再确定在C51下是否能支持。
}

void SHA1_CycleCompute0(u4 xdata* pu4ABCDE, u4 xdata u4W)
{
	u4		temp;

	temp = (pu4ABCDE[0]<<5 | pu4ABCDE[0]>>27) + pu4ABCDE[4] + u4W + 0x5a827999 
		 + (pu4ABCDE[1] & pu4ABCDE[2] | ~pu4ABCDE[1] & pu4ABCDE[3]);

	pu4ABCDE[4] = pu4ABCDE[3];
	pu4ABCDE[3] = pu4ABCDE[2];
	pu4ABCDE[2] = pu4ABCDE[1]<<30 | pu4ABCDE[1]>>2;
	pu4ABCDE[1] = pu4ABCDE[0];
	pu4ABCDE[0] = temp;
}
void SHA1_CycleCompute1(u4 xdata* pu4ABCDE, u4 xdata u4W)
{
	u4		temp;
	
	temp = (pu4ABCDE[0]<<5 | pu4ABCDE[0]>>27) + pu4ABCDE[4] + u4W + 0x6ed9eba1 
		+ (pu4ABCDE[1] ^ pu4ABCDE[2] ^ pu4ABCDE[3]);

	pu4ABCDE[4] = pu4ABCDE[3];
	pu4ABCDE[3] = pu4ABCDE[2];
	pu4ABCDE[2] = pu4ABCDE[1]<<30 | pu4ABCDE[1]>>2;
	pu4ABCDE[1] = pu4ABCDE[0];
	pu4ABCDE[0] = temp;
}
void SHA1_CycleCompute2(u4 xdata* pu4ABCDE, u4 xdata u4W)
{
	u4		temp;
	
	temp = (pu4ABCDE[0]<<5 | pu4ABCDE[0]>>27) + pu4ABCDE[4] + u4W + 0x8f1bbcdc 
		+ (pu4ABCDE[1] & pu4ABCDE[2] | pu4ABCDE[1] & pu4ABCDE[3] | pu4ABCDE[2] & pu4ABCDE[3]);
	
	pu4ABCDE[4] = pu4ABCDE[3];
	pu4ABCDE[3] = pu4ABCDE[2];
	pu4ABCDE[2] = pu4ABCDE[1]<<30 | pu4ABCDE[1]>>2;
	pu4ABCDE[1] = pu4ABCDE[0];
	pu4ABCDE[0] = temp;
}
void SHA1_CycleCompute3(u4 xdata* pu4ABCDE, u4 xdata u4W)
{
	u4		temp;
	
	temp = (pu4ABCDE[0]<<5 | pu4ABCDE[0]>>27) + pu4ABCDE[4] + u4W + 0xca62c1d6 
		+ (pu4ABCDE[1] ^ pu4ABCDE[2] ^ pu4ABCDE[3]);
	
	pu4ABCDE[4] = pu4ABCDE[3];
	pu4ABCDE[3] = pu4ABCDE[2];
	pu4ABCDE[2] = pu4ABCDE[1]<<30 | pu4ABCDE[1]>>2;
	pu4ABCDE[1] = pu4ABCDE[0];
	pu4ABCDE[0] = temp;
}

void vSha1OneBlock(void xdata * pu1HashData,const void xdata * pu1Message)
{
	u1	t;
	u4 xdata	pu4W[0x40];
	u4 xdata	pu4ABCDE[0x14];
	u4 xdata	u4Temp;

	//pu4W = MngAllocateBuff(0x40);
	//pu4ABCDE = MngAllocateBuff(0x14);

	// 注意，输入的参数可能不是四字节的整数倍，但是我们在内部运算的时候要用成四字节整数倍
	longReverse(pu4ABCDE, pu1HashData, 0x05);

	for ( t = 0; t < 16; t++ )
	{
		pu4W[t] = BYTE2LONG(((u1*)pu1Message) + (t<<2));
		SHA1_CycleCompute0(pu4ABCDE, pu4W[t]);
	}
	for (t = 16 ; t < 20; ++t) 
	{
		u4Temp = pu4W[(t-3)&0x0F] ^ pu4W[(t-8)&0x0F] ^ pu4W[(t-14)&0x0F] ^ pu4W[t&0x0F];
		pu4W[t&0x0F] = u4Temp<<1 | u4Temp>>31; 
		SHA1_CycleCompute0(pu4ABCDE, pu4W[t&0x0F]);
	}

	for (t = 20 ; t < 40; ++t) 
	{
		u4Temp = pu4W[(t-3)&0x0F] ^ pu4W[(t-8)&0x0F] ^ pu4W[(t-14)&0x0F] ^ pu4W[t&0x0F];
		pu4W[t&0x0F] = u4Temp<<1 | u4Temp>>31; 
		SHA1_CycleCompute1(pu4ABCDE, pu4W[t&0x0F]);
	}

	for (t = 40 ; t < 60; ++t) 
	{
		u4Temp = pu4W[(t-3)&0x0F] ^ pu4W[(t-8)&0x0F] ^ pu4W[(t-14)&0x0F] ^ pu4W[t&0x0F];
		pu4W[t&0x0F] = u4Temp<<1 | u4Temp>>31; 
		SHA1_CycleCompute2(pu4ABCDE, pu4W[t&0x0F]);
	}

	for (t = 60 ; t < 80; ++t) 
	{
		u4Temp = pu4W[(t-3)&0x0F] ^ pu4W[(t-8)&0x0F] ^ pu4W[(t-14)&0x0F] ^ pu4W[t&0x0F];
		pu4W[t&0x0F] = u4Temp<<1 | u4Temp>>31; 
		SHA1_CycleCompute3(pu4ABCDE, pu4W[t&0x0F]);
	}
	pu4ABCDE[0] += BYTE2LONG(((u1*)pu1HashData));
	pu4ABCDE[1] += BYTE2LONG(((u1*)pu1HashData) + 0x04);
	pu4ABCDE[2] += BYTE2LONG(((u1*)pu1HashData) + 0x08);
	pu4ABCDE[3] += BYTE2LONG(((u1*)pu1HashData) + 0x0C);
	pu4ABCDE[4] += BYTE2LONG(((u1*)pu1HashData) + 0x10);

	longReverse(pu1HashData, pu4ABCDE, 0x05);

	//MngFreeBuff(0x54);
	//vEnableLowPower(MOUDLE_SOFT_ALG);
	return;
}

void SHA1Data(u1 xdata* pu1Output, const u1 xdata* pu1Input, u2 u2DataSize)
{
	u1 xdata	u1BlockSize;
	u1 xdata	u1IvSize;
	u4 xdata	u4DataSize;
	u1 xdata	pu1Temp[0x40];

	u4DataSize = (u2DataSize<<0x03);
	u1BlockSize = 0x40;
	u1IvSize = 0x14;
	//pu1Temp = (u1*)MngAllocateBuff(u1BlockSize);
	vSha1GetDefaultIv(pu1Output);
	while ( u2DataSize >= u1BlockSize )
	{
		vSha1OneBlock(pu1Output, pu1Input);
		pu1Input += u1BlockSize;
		u2DataSize -= u1BlockSize;
	}
	memset(pu1Temp, 0x00, u1BlockSize);
	memcpy(pu1Temp, pu1Input, u2DataSize);
	pu1Temp[u2DataSize] = 0x80;
	if ( u2DataSize >= (u1)(u1BlockSize - (u1BlockSize>>3)) )
	{
		vSha1OneBlock(pu1Output, pu1Temp);
		memset(pu1Temp, 0x00, u1BlockSize);
	}
	{

		LONG2BYTE(pu1Temp + u1BlockSize - 0x04,u4DataSize);
	}
	vSha1OneBlock(pu1Output, pu1Temp);
	//MngFreeBuff(u1BlockSize);
	return;
}


