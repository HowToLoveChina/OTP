
void Xrmemcpy(void *pu1Output, const void *pu1Input, unsigned short u2Len);
void AlgSymmMacFun2(u1 * pu1Data, u2 *pu2DataSize, u1 *pu1KeyVal,  u1* pu1IV);
void memxor(void *pu1Output, const void *pu1Input, u2 u2Len);
//u1 SM4ENC_TEST(void);
u1 SM4DEC_TEST(void);
//u1 SM3_TEST(void);
u1 SHA1_TEST(void);
