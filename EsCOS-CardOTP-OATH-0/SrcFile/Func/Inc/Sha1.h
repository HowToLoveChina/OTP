#ifndef _SHA1_H_
#define _SHA1_H_
#define _BIG_ENDIAN_

#include "typedefine.h"

#if 1
typedef struct {
	unsigned int length[2];
	unsigned int h[8];
	unsigned int w[80];
} sha;

#define H0 0x67452301L
#define H1 0xefcdab89L
#define H2 0x98badcfeL
#define H3 0x10325476L
#define H4 0xc3d2e1f0L

//#define H0 0x01234567L
//#define H1 0x89abcdefL
//#define H2 0xfedcba98L
//#define H3 0x76543210L
//#define H4 0xf0e1d2c3L

#define K0 0x5a827999L
#define K1 0x6ed9eba1L
#define K2 0x8f1bbcdcL
#define K3 0xca62c1d6L

#define PAD  0x80
#define ZERO 0

/* functions */

#define S(n,x) (((x)<<n) | ((x)>>(32-n)))

#define F0(x,y,z) ((x&y)|((~x)&z))
#define F1(x,y,z) (x^y^z)
#define F2(x,y,z) ((x&y) | (x&z)|(y&z)) 
#define F3(x,y,z) (x^y^z)
#endif

// Functions

void sha1_init(sha *sh);
void sha1_process(sha *sh,int byte);
void sha1_hash(sha *sh,char hash[20]);
static void sha1_transform(sha *sh);

// 计算SHA值，输出到pbHashValue指向的地址
// 注意：缓冲区长度至少0x14字节
void ComputeSHA1(const UINT8* pbData, UINT32 dwDataLen, UINT8* pbHashValue);
void SHA1Data(u1 xdata* pu1Output, const u1 xdata* pu1Input, u2 u2DataSize);

#endif




