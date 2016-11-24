/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009－2015 All Rights Reserved
 ********************************************************************
File: OtpFrame.c
History：
Version                      Author                     Date
V0.1                         nationz              2013-03-27

Description:
     OtpFrame Source Code File;
Function list:
        void vOTP_RunModeProcess(void);
		void vOTP_UnlockPinProcess(void);
		void vOTP_SetPinProcess(void);
		void vOTP_InputPinProcess(void);
		void vOTP_ActiveProcess(void);
		void vOTP_SeedImportProcess(void);
		void vOTP_ScrapProcess(void);        
Special statement:
     NULL
 *******************************************************************/
#define _OTPFRAME_C_
#include "OTPFRAME.H"
//#include "LCDFUNC.H"
#include "uc8118.h"	
#include "SCUFUNC.H"
//#include "KBCFRAME.H"
#include "UartFrame.h"
#include "TBCFUNC.H"
#include "ADCFUNC.H" 
#include "string.H"	
#include "UartFunc.h"
#include "Debug.h" 
#include "SM3FUNC.H"
#include "TBCFRAME.H"
#include "SM4.h"
#include "OtpFunc.h"
#include "Iomfunc.h"
#include "WDTFUNC.H"
#include "SecFunc.h"
#include "SM4Process.h"
#include "I2C.h"

//UINT8 bOpenHash,bCloseLCD,bAlwaysbdisplay;
UINT8 DispKeyPad[SCREENLEN];

UINT8 u1Time_Dispaly[6];

//UINT8  g_UART_COM_BUF[64];
UINT8 idata g_ModfyPwFlag _at_ 0x38;
u1 LongPressFlag = 0; 
UINT8 g_AutoUnlockFlag = 0;
UINT8 idata g_AutoUnlockCnt _at_ 0x37;
UINT8 idata gUnlockPinErr_Count _at_ 0x36;//手动解锁计数，超过一定次数，OTP失效或自毁
UINT8 idata gsuperpw  _at_ 0x30;            //激活标志位
UINT8 idata gUseCase _at_ 0x31;           //OTP 状态切换标志
UINT32 idata gTimer_Count_Base _at_ 0x32; //	OTP系统基准偏移时间
//UINT32 idata gTimer_Count _at_ 0x36; //	OTP系统时间
UINT32 idata gPinErr_Time _at_ 0x3A; // OTP被锁定的系统时间
UINT8 idata gPinErr_Count _at_ 0x3E;      //Pin码输入错误计数，最大错误次数不超过6次
UINT8 idata gCalibflag _at_ 0x3F;
//UINT8 idata PowerOnFlag _at_ 0xb0; 		//STARTUP文件里面的变量，改善复位后RAM被清掉的情况
u1 u1LowPowerFlag = 0;
u1 u1FirstSetFlag = 0;

volatile UINT8 gSecCount_10S,_TIMER_FLAG_,gFlag_1S; //指示关机等待时间计数器，每10S显示标志关闭一个，时间标志全部消失OTP自动关机
UINT8 gu1DetectErrFlag = 0;

UINT8 idata gUserInfo[SEED_LEN] _at_ 0x40;      //种子密钥等用户信息，掉电保持
//UINT8 idata gActiveSeed[32]; 
//UINT8 idata gUnlockSeed[32];
UINT8 idata gToken_Save[8] _at_ 0x60;//
UINT32 idata gToken_Time1 _at_ 0x68;
UINT32 idata gToken_Time2 _at_ 0x6c;
//UINT8 gPin_Key[8];
//UINT8 gErr[8]={"Error-00"};

UINT8 idata g_u1SeedKey[SEEDKEY_LEN] _at_ 0x70;		//seedkey
UINT8 idata g_u1MacKey[MACKEY_LEN] _at_ 0x80;		//MACkey
//UINT8 xdata g_u1Key[KEY_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};              //u1KEY
UINT8 idata g_u1Key[KEY_LEN] _at_ 0x90;              //u1KEY
//UINT8 xdata g_u1SN[SN_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};              //序列号
UINT8 idata g_u1SN[16] _at_ 0xa0;              //序列号
UINT8 idata g_SNLen _at_ 0xb1;
//UINT8 EndFlag;

/********************************************************************
Function name:       void vOTP_KEY_CHECK(void)
Input parameters:
    void  
Return:
    void
Function:
    Input Pin Code Process when OTP(been Actived) Power On  	
********************************************************************/
//输入PIN 操作
u4 GetBaseTime(UINT8 *TimeInput)
{
	u4	Ticks;
	u2	k;
	u2  u2Tmp;
	
	Ticks	=BASE_2011MINUTES; //BASE_MINUTES_2011;

	//u2Tmp = *(TimeInput + 1)*100 + *(TimeInput + 2);
	k	=2011;
	while( k<(2000 + *(TimeInput + 2) )	)
	{
		Ticks	+= 525600ul;  //365*24*60
		if((k&0x03)==0)
		{
			Ticks	+=1440ul; //24*60
		}
		k++;
	}
	
	if(*(TimeInput + 3)>1)
	{
		Ticks	+=44640ul;    //31*24*60
	}

	if(*(TimeInput + 3)>2)
	{
		if(((*(TimeInput + 2))&0x03)==0)
		{
			Ticks	+=41760ul; //29*24*60
		}
		else
		{
			Ticks	+=40320ul; //28*24*60
		}
	}

	if(*(TimeInput + 3)>3)
	{
		Ticks	+=44640ul;
	}	

	if(*(TimeInput + 3)>4)
	{
		Ticks	+=43200ul;  //30*24*60
	}	

	if(*(TimeInput + 3)>5)
	{
		Ticks	+=44640ul;
	}	

	if(*(TimeInput + 3)>6)
	{
		Ticks	+=43200ul;
	}	

	if(*(TimeInput + 3)>7)
	{
		Ticks	+=44640ul;
	}	

	if(*(TimeInput + 3)>8)
	{
		Ticks	+=44640ul;
	}	

	if(*(TimeInput + 3)>9)
	{
		Ticks	+=43200ul;
	}	

	if(*(TimeInput + 3)>10)
	{
		Ticks	+=44640ul;
	}	

	if(*(TimeInput + 3)>11)
	{
		Ticks	+=43200ul;
	}

	u2Tmp    = (u2)(*(TimeInput + 4)-1)*(1440);//24*60;
	Ticks	+= u2Tmp;//(RTC_DateStruct.RTC_Date-1)*(24*60);//24*60
	u2Tmp   = *(TimeInput + 5)*60;
	Ticks	+= u2Tmp;//RTC_TimeStruct.RTC_Hours*60;
	Ticks	+= *(TimeInput + 6);
	Ticks   *= 60;
	Ticks	+= *(TimeInput + 7);
	//Ticks	=1206446790;  // 单位为秒，即“3 25 2008, 12:06:30 GMT”
	//*pdata =Ticks;
	return Ticks;
}

UINT8 DectoBCD( UINT8 temp )  
{  
	return ( (temp/10)*16 + temp%10 ) ;  
} 

void HexToStr(UINT8 *pbDest, UINT8 *pbSrc, UINT8 nLen)
{
	char	ddl,ddh;
	UINT8   i;
	
	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	
	pbDest[nLen*2] = '\0';
}

UINT8 Get2121Chk2(const UINT8* pu1Num, UINT32 u4NumLen)
{
	UINT8	u1Temp;
	UINT32	u4Data;
	UINT32	i;
	
	u4Data = 0;
	for ( i = 0; i < u4NumLen; i++)
	{
		u1Temp = (UINT8)((pu1Num[i] - 0x30)*(0x02 - (i&0x01)));
		u4Data += /*(u1Temp/10) + */(u1Temp%10);
	}
	u1Temp = (UINT8)(10 - (u4Data%10));
	if ( 10 == u1Temp )
	{
		u1Temp = 0x00;
	}
	return u1Temp;
}


#if 0
/********************************************************************
Function name:       void vOTP_KEY_CHECK(void)
Input parameters:
    void  
Return:
    void
Function:
    Input Pin Code Process when OTP(been Actived) Power On  	
********************************************************************/
//输入PIN 操作
void vOTP_KEY_CHECK(void)
{
	//UINT8 i;
	UINT8 KeyCheckCnt = 0;
	UINT8 Key_Input = 0xff;
	UINT16 u2Cnt;
	//UINT8 KeyPad[13];
	#if 1
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 
#endif
	if(u1LowPowerFlag == 1)
	{
		vLcd_String_Display2("LPkkkkkk", 8);
   		vScu_TbcIntEn();
		vTbc_Int1HzEn();
		gSecCount_10S=0;
		while(gSecCount_10S < 2);
		vLcd_String_Clear2();
	}
	
	vLcd_String_Display2("12345678",8);

#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif

	#endif

	_TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	
	while(1)
	{
		Key_Input = ucGetKeyValue();

		#if 1
		switch(Key_Input)
		{
			case KEY_1:						//KEY_1
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 0)
				{
					
					KeyCheckCnt = 1;
					
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
		
					vLcd_String_Display2("P1P1P1P1",8);
					//vLcd_String_Display2("11111111",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 12)
				{
					KeyCheckCnt = 13;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("1P1P1P1P",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;
			}
			case KEY_2:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 1)
				{
					KeyCheckCnt = 2;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("22222222",8);
					vLcd_String_Display2("n2n2n2n2",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 13)
				{
					KeyCheckCnt = 14;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("2n2n2n2n",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_3:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 2)
				{
					KeyCheckCnt = 3;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("33333333",8);
					vLcd_String_Display2("o3o3o3o3",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 14)
				{
					KeyCheckCnt = 15;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("3o3o3o3o",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_4:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 3)
				{
					KeyCheckCnt = 4;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("44444444",8);
					vLcd_String_Display2("-4-4-4-4",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 15)
				{
					KeyCheckCnt = 16;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("4-4-4-4-",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_5:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 4)
				{
					KeyCheckCnt = 5;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("55555555",8);
					vLcd_String_Display2("_5_5_5_5",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 16)
				{
					KeyCheckCnt = 17;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("5_5_5_5_",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_6:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 5)
				{
					KeyCheckCnt = 6;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("66666666",8);
					vLcd_String_Display2("E6E6E6E6",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 17)
				{
					KeyCheckCnt = 18;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("6E6E6E6E",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_7:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 6)
				{
					KeyCheckCnt = 7;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("77777777",8);
					vLcd_String_Display2("r7r7r7r7",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 18)
				{
					KeyCheckCnt = 19;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("7r7r7r7r",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_8:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 7)
				{
					KeyCheckCnt = 8;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("88888888",8);
					vLcd_String_Display2("L8L8L8L8",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 19)
				{
					KeyCheckCnt = 20;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("8L8L8L8L",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_9:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 8)
				{
					KeyCheckCnt = 9;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_String_Display2("99999999",8);
					vLcd_String_Display2("c9c9c9c9",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				else if(KeyCheckCnt == 20)
				{
					KeyCheckCnt = 21;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					
					vLcd_String_Display2("9c9c9c9c",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif

					gUseCase = OTPSTATUS_CALIB;	

					return;
				}
				break;	
			}
			case KEY_DEL:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 10)
				{
					KeyCheckCnt = 11;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					vLcd_String_Display2("KkKkKkKk",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			case KEY_0:
			{
				_TIMER_FLAG_=0;
				if(KeyCheckCnt == 9)
				{
					KeyCheckCnt = 10;
					#ifdef _FPGA_DEMO_EN_
				    vLcd_SetStopMode();
					#endif	
					//vLcd_Flag_Clear();
					vLcd_String_Display2("00000000",8);
					#ifdef _FPGA_DEMO_EN_	
					vLcd_SetDisplayMode(); 
					#endif
				}
				break;	
			}
			default:
				break;
				
		}
		#endif

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;

			if(KeyCheckCnt == 11)
			{
				KeyCheckCnt = 12;
				#ifdef _FPGA_DEMO_EN_
			    vLcd_SetStopMode();
				#endif	
				vLcd_String_Display2("kKkKkKkK",8);
				#ifdef _FPGA_DEMO_EN_	
				vLcd_SetDisplayMode(); 
				#endif

				//gUseCase = OTPSTATUS_CALIB;	

				//return;
			}
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}

		
		if((_TIMER_FLAG_>= 120))	
		{
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();
		}

		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
	}
}

#endif



void vOTP_InitProcess(void)
{
	u2 u2Status; 
	UINT8 u1CHeckStatus;
	UINT8 u1Datum[64];
//	UINT16 E2prom_Address;
	u2 u2PPM_Value;
	

//	vLcd_All_Display2();	//需替换
	vPaper_String_Display("88888888",8);

	#if 1			// suanfa zijian
	vScu_SetSysClkOsc2M();
	u1CHeckStatus =  SM4DEC_TEST();			//SM4ENC_TEST();			//任何一个算法模块自检失败，都会使设备失效，进入失效状态
	if(u1CHeckStatus == 0xff)
	{
		gUseCase = OTPSTATUS_INVALID;
		return;
	}

	
	if(u1CHeckStatus == 0)					//如果前面的自检错误了，就没有必要再进行下面的自检了，也避免前面自检错误，后面自检正确的情况
	{
		u1CHeckStatus = SHA1_TEST();	
		if(u1CHeckStatus == 0xff)
		{
			gUseCase = OTPSTATUS_INVALID;
			return;
		}
	}
	vScu_SetSysClk32K();  
	#endif



	vScu_TbcIntEn();			//da kai		
	vTbc_Int1HzEn();
	
//	IOMP0PD   &= 0xfe;		//GP0关闭下拉
//	IOMP0PU   |= 0x01;		//GP0打开上啦
	
//	vScu_SetSysClk64K();		//9600_test
//	vUartInit();
	vIom_LsclkOutEn();

	while(1)
	{
		//u2Status = ReceiveData_Poll();
		readFromROM(u1Datum, EEPROM_ADDRESS, FRAME_LENGTH);
		
		if( FRAME_HEAD == u1Datum[0] )
		{
				
		}
		else
		{
				
		}
		
		

		if(u2Status == RSP_SET_SUCCESS)
		{
			if(OPCODE_SET_CALIB== g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase < OTPSTATUS_ACTIVATE)
				{
			                Bak_PPMValue = ((u2)g_UART_COM_BUF[OFFSET_DATA]<<8) + (u2)g_UART_COM_BUF[OFFSET_DATA+1];
			                
			                if(0x8000&Bak_PPMValue) //-ppm
			                {
			                  u2PPM_Value =  Bak_PPMValue^0x8000; 
												Crystal_PPM = -((float)u2PPM_Value/2);
			                }
			                else
			                {
			                    Crystal_PPM = ((float)Bak_PPMValue/2);
			                }

					if(Bak_PPMValue&0x8000) //在TBC启动之前需要配置
					{
	                    			TBCADJL=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))); 
						TBCADJH=(UINT8)((0x7FFF-(Bak_PPMValue&0x7FFF))>>8);		    			
					}
					else
					{
	                    			TBCADJL=(UINT8)((0x7FFF+Bak_PPMValue));
				 		TBCADJH=(UINT8)((0x7FFF+Bak_PPMValue)>>8);	 
					} 

					gCalibflag = 1;
					gUseCase = OTPSTATUS_SN_TIME; 				//先写温度再写校准值
						
					USART_TxRsp(u2Status,OPCODE_SET_CALIB);
				}
				else
				{
					u2Status = RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);	
				}

			}
			else if(OPCODE_SN_TIME== g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase < OTPSTATUS_ACTIVATE)
				{
					g_SNLen = g_UART_COM_BUF[OFFSET_LEN] - TIME_LEN - KEY_LEN;
					if((g_SNLen < 17))
					{
						if ( OTPSECU_OP_OK == CosOperation(OP_SN, &g_UART_COM_BUF[OFFSET_DATA]))
						{
							gTimer_Count_Base = GetBaseTime(&g_UART_COM_BUF[OFFSET_DATA+g_SNLen]);
		
							//vTbcInit();		//打开TBC模块
							vTbc_StartDis();
							vTbc_StartEn();
							gUseCase = 	OTPSTATUS_MACKEY;					//OTPSTATUS_MACKEY;
							u2Status	=RSP_SET_SUCCESS;
							USART_TxRsp(u2Status,OPCODE_SN_TIME);
	
		                   			 vTbc_ReadBakCounter();
		                   			 gBackupTBCInitVal = ulGetTbcCountVlaue();
							//return;
						}
						else
						{
							u2Status	=RSP_COS_ERR;
							USART_TxRsp(u2Status,OPCODE_SN_TIME);
						}					
					}
					else
					{
						u2Status = RSP_OPCODE_ERR;
						USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);					
					}
				}
				else
				{
					u2Status	=RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,OPCODE_SN_TIME);
				}
			}
			else if(OPCODE_MK_SK == g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase <OTPSTATUS_ACTIVATE)
				{
					if ( OTPSECU_OP_OK == CosOperation(OP_MACKEY_SEEDKEY, &g_UART_COM_BUF[OFFSET_DATA]))
					{
						gUseCase = OTPSTATUS_CRCSNTIME;			

						USART_TxRsp(u2Status,OPCODE_MK_SK);	
						u2Status	=RSP_SET_SUCCESS;				
						
						//break;
						//return;
					}
					else
					{
						u2Status	=RSP_COS_ERR;
						USART_TxRsp(u2Status,OPCODE_MK_SK);
					}
				}
				else
				{
					u2Status	=RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,OPCODE_MK_SK);
				}
			}
			else if(OPCODE_REQ_OK== g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase >= OTPSTATUS_CRCSNTIME)			//也可能是激活态
				{                    
					if((0xA0 == g_UART_COM_BUF[OFFSET_DATA])&&(0x02 == g_UART_COM_BUF[OFFSET_DATA + 1]))
					{
						USART_TxRsp(u2Status,OPCODE_REQ_OK);
						//if(OTPSECU_OP_OK ==CosOperation(OP_GEN_SEED, NULL))
						//{
							u2Status	=RSP_SET_SUCCESS;

//							vLcd_All_Clear2();
						  Paper_All_Clear();
							g_ModfyPwFlag = 1;
							gUseCase = OTPSTATUS_TIMETOKEN;   
							vScu_SetSysClk32K();				//时钟从64K变为32K
							//vScu_PowerIo_ResultInterrupt();			//zhong duan guanji
							return;
						//}
						//else
						//{
						//	u2Status	=RSP_COS_ERR;
						//	USART_TxRsp(u2Status,OPCODE_REQ_OK);
						//}
						
					}
					else
					{
						u2Status	=RSP_DATA_ERR;
						USART_TxRsp(u2Status,OPCODE_REQ_OK);
					}
				}
				else
				{
					u2Status	=RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,OPCODE_REQ_OK);
				}
			}
			else if(SET_TEMP_BASIC == g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase < OTPSTATUS_ACTIVATE)
				{
			                Basic_Temp_Cmd = 1;
			                Basic_Temp_Value = ((u2)g_UART_COM_BUF[OFFSET_DATA]<<8) + g_UART_COM_BUF[OFFSET_DATA+1]; 
			                
					 if(gCalibflag == 1)
					 {
					 	#if 1
						vScu_SetSysClkOsc2M();					//s会不会影响串口通信?
						Exral_Temp_Flag = 0;
						#if(DEBUG)
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
	 					 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						 vDelay200Us(250);
						#else

						IC_Temperature_Test();					//test0406
						vFreqAdj(0);
						#endif
						
						vScu_SetSysClk32K();
						vScu_WakeUpTimerWakeUp_1Hour();         //设置1小时定时唤醒 20150515 	
						vScu_SetSysClk64K();			//9600_test
						WT_Onehour_Flag = 1;
						#endif
						vUartInit();
						USART_TxRsp(u2Status,OPCPDE_READ_TEMP);
					 }
					 else
					 {
					 	u2Status = RSP_OPCODE_ERR;
					 	USART_TxRsp(u2Status,SET_TEMP_BASIC);
					 }
				}
				else
				{
					u2Status	=RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,SET_TEMP_BASIC);
				}
			}			
			else if(OPCODE_READ_STATUS== g_UART_COM_BUF[OFFSET_OP])
			{
				u2Status = RSP_SET_SUCCESS;
				USART_TxRsp(u2Status,OPCODE_READ_STATUS);
			}
			else if(OPCODE_READ_CALIB== g_UART_COM_BUF[OFFSET_OP])
			{
				//if(gUseCase > OTPSTATUS_CALIB)
				{
					u2Status = RSP_SET_SUCCESS;
					USART_TxRsp(u2Status,OPCODE_READ_CALIB);
				}
			}
			else if(OPCPDE_READ_TEMP== g_UART_COM_BUF[OFFSET_OP])
			{
				USART_TxRsp(u2Status,OPCPDE_READ_TEMP);
			}
			else if(OPCODE_READ_VERSION== g_UART_COM_BUF[OFFSET_OP])
			{
				USART_TxRsp(u2Status,OPCODE_READ_VERSION);
			}
			else if(OPCODE_READ_TIME== g_UART_COM_BUF[OFFSET_OP])
			{
				USART_TxRsp(u2Status,OPCODE_READ_TIME);
				//u2Status = RSP_SET_FAIL;
			}
			else if(OPCODE_READ_SN== g_UART_COM_BUF[OFFSET_OP])
			{
				USART_TxRsp(u2Status,OPCODE_READ_SN);
				//u2Status = RSP_SET_FAIL;
			}
			else if(OPCODE_CHANGE_STATUS== g_UART_COM_BUF[OFFSET_OP])
			{
				if(gUseCase>g_UART_COM_BUF[OFFSET_DATA])
				{
					gUseCase = g_UART_COM_BUF[OFFSET_DATA];
					USART_TxRsp(u2Status,OPCODE_CHANGE_STATUS);
					break;
				}
				else
				{
					u2Status = RSP_OPCODE_ERR;
					USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);
				}
			}
			else
			{
				u2Status = RSP_OPCODE_ERR;
				USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);
			}
		}
		else if(RSP_REV_TIMEOUT ==u2Status)
		{	
			u2Status	=RSP_REV_TIMEOUT;
			/**********直接进入休眠***********/
//			vLcd_All_Clear2();
			Paper_All_Clear();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();
		}
		else if(RSP_CHK_FAIL ==u2Status)
		{	
			u2Status	=RSP_CHK_FAIL;
			USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);
		}
		else if(RSP_DATA_ERR == u2Status)
		{
			USART_TxRsp(u2Status,g_UART_COM_BUF[OFFSET_OP]);
		}

	}
}

#if 0
/********************************************************************
Function name:       void vInputPin(void)
Input parameters:
    void  
Return:
    void
Function:
    Input Pin Code Process when OTP(been Actived) Power On  	
********************************************************************/
//输入PIN 操作
void vOTP_InputPinProcess(void)
{
	UINT8 Key_Input=0xff;
	UINT8 i=0;
	UINT8 InputCnt = 0;	
	UINT8 KBuff[PW_LEN];
	UINT16 u2Cnt;

	memset(KBuff, 0x00, PW_LEN);
	memset(DispKeyPad, 0x00, SCREENLEN);
	//LCD提示用户输入PIN
	for(i = 0; i<PW_LEN; i++)DispKeyPad[i] = '-';
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();			
#endif

	if(u1LowPowerFlag == 1)
	{
		vLcd_String_Display2("LPkkkkkk", 8);
   		vScu_TbcIntEn();
		vTbc_Int1HzEn();
		gSecCount_10S=0;
		while(gSecCount_10S < 2);
		
	}
	vLcd_String_Clear2();
			
	vLcd_String_Display2(DispKeyPad, PW_LEN);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif

    _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();

	while(1)
	{
		vClrWdtCounter();
		
		Key_Input = ucGetKeyValue();	
		//if(Key_Input>=0x30 && Key_Input<=0x3f)
		//{ 	
		switch(Key_Input)
		{
			case KEY_0:
			case KEY_1:
			case KEY_2:
			case KEY_3:
			case KEY_4:
			case KEY_5:
			case KEY_6:
			case KEY_7:
			case KEY_8:
			case KEY_9:
			    {
                    _TIMER_FLAG_=0;
					if(InputCnt < PW_LEN)
					{
						KBuff[InputCnt] = Key_Input;
						DispKeyPad[InputCnt++] = '_';
#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
					    //vLcd_Flag_Clear();
						//vLcd_String_Clear();

						//vLcd_Flag_Display(6,1);		//密码
						//vLcd_Flag_Display(7,1);		//开机
						//vLcd_String_Display(DispKeyPad, PW_LEN);
						vLcd_Bcd_Display2((2+InputCnt),14);		//'_'
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
					}
					break;
				}
			case KEY_DEL:			//删除键			     
				{
                    _TIMER_FLAG_=0;
					if(LongPressFlag)
					{
						LongPressFlag = 0;
						memset(KBuff, 0x00, PW_LEN);
						for(i = 0; i<PW_LEN; i++)DispKeyPad[i] = '-';
						InputCnt = 0;
						vLcd_String_Display2(DispKeyPad, PW_LEN);
						break;
					}
					
					if(InputCnt > 0)
					{
						InputCnt--;
						DispKeyPad[InputCnt] = '-';
						KBuff[InputCnt] = 0x00;
#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
					    //vLcd_Flag_Clear();
						//vLcd_String_Clear();

						//vLcd_Flag_Display(6,1);		//密码
						//vLcd_Flag_Display(7,1);		//开机
						//vLcd_String_Display(DispKeyPad, PW_LEN);
						vLcd_Bcd_Display2((3+InputCnt),13);
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
					}
					break;
				}
			case KEY_DOT:			
			case KEY_LEFT:
			case KEY_RIGHT:	
			    _TIMER_FLAG_=0;
			default:
				break;
		  }

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			if(InputCnt == PW_LEN)	
			{
				if(OTPSECU_OP_OK == CosOperation(OP_VERIFY_PASSWORD, KBuff, PW_LEN))
				{
					if(g_ModfyPwFlag == 0)
					 {
					 	gUseCase = OTPSTATUS_SELECTMODE;
					 }
					 else
					 {
					 	gUseCase = OTPSTATUS_PWSET;
					 }
					 //g_ModfyPwFlag = 0;
					 
#ifdef _FPGA_DEMO_EN_
                     vLcd_SetStopMode();	                      	                    
#endif
					 //vLcd_Flag_Clear();
					 vLcd_String_Clear2();
					 //vLcd_Flag_Display(3,1);		//成功
					// vLcd_Flag_Display(6,1);		//开机
#ifdef _FPGA_DEMO_EN_  
                     vLcd_SetDisplayMode();		
#endif
					// vScu_TbcIntEn();
					// vTbc_Int1HzEn();
					 //gSecCount_10S=0;
					 //while(gSecCount_10S < 2);
				}
				else
				{
					 //gPinErr_Count++;
					 //gSecCount_10S=0;
					 if(gPinErr_Count >= PWLOCK_LIMITED)
					 {
					 	gPinErr_Time = Base_Time();
					 	gUseCase = OTPSTATUS_UNLOCK;
						//gPinErr_6TimeFlag = 1;
					 }
					 else
					 {
					 	if(gsuperpw == 0)
					 	{
					 		gUseCase = OTPSTATUS_PWVERIFY;
					 	}
					 	else
					 	{
					 		gsuperpw = 0;
					 	}
					 }
#ifdef _FPGA_DEMO_EN_
                     vLcd_SetStopMode();	                      	                    
#endif
					 //vLcd_Flag_Clear();
					 //vLcd_String_Clear();
					 //vLcd_Flag_Display(2,1);		//错误
					 //vLcd_Flag_Display(6,1);		//开机
					// vLcd_String_Clear2();
					//vLcd_String_Display2("Error",5);

					DispKeyPad[0] = 'E';
					DispKeyPad[1] = 'r';
					DispKeyPad[2] =  'r';
					DispKeyPad[3] =  'o';
					DispKeyPad[4] =  'r';
					DispKeyPad[5] =  'k';
					DispKeyPad[6] =  'k';
					 DispKeyPad[7] = gPinErr_Count + 0x30;
					 
					 vLcd_String_Display2(DispKeyPad,8);
#ifdef _FPGA_DEMO_EN_  
                     vLcd_SetDisplayMode();		
#endif
					 vScu_TbcIntEn();
					 vTbc_Int1HzEn();
					 gSecCount_10S=0;
					 while(gSecCount_10S < 2);
				}
				vScu_TbcIntDis();
				vTbc_Int1HzDis();
				return;
			}
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
		
		if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
	    	{
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}

		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
	}
	vScu_TbcIntDis();
	vTbc_Int1HzDis();
}
/********************************************************************
Function name:       void vOTP_SetPinProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    Setting Pin Code Process when The OTP Power On The First Time  	
********************************************************************/
//设置PIN 操作
void vOTP_SetPinProcess(void)
{
	
	UINT8 Key_Input=0xff;
	UINT8 i=0;
	UINT8 KBuff[SCREENLEN];
	UINT8 bSecondInput=0;
	UINT8 InputCnt;
	UINT8 Pin_Save[16];
	UINT16 u2Cnt;

	InputCnt = 0;
	
	for(i = 0; i<PW_SM_LEN; i++)Pin_Save[i] = 0xff;
	
	DispKeyPad[0] = 'P';
	DispKeyPad[1] = '1';
	//DispKeyPad[1] += 0x80;
	//DispKeyPad[2] = '1';
	
	//for(i = 3; i<(SCREENLEN - PW_LEN); i++)DispKeyPad[i] = 'k';

	for(i = (SCREENLEN - PW_LEN); i<SCREENLEN; i++)DispKeyPad[i] = '-';
	
	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 	
#endif
    //vLcd_Flag_Clear();
	

	//vLcd_Flag_Display(7,1);		//密码
	//vLcd_Flag_Display(6,1);		//开机
	//vLcd_Flag_Display(5,1);		//新
	if(u1LowPowerFlag == 1)
	{
		vLcd_String_Display2("LPkkkkkk", 8);
   		vScu_TbcIntEn();
		vTbc_Int1HzEn();
		gSecCount_10S=0;
		while(gSecCount_10S < 2);
		
	}
	vLcd_String_Clear2();
	vLcd_String_Display2(DispKeyPad, SCREENLEN);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif

    _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	while(1)
	{

		vClrWdtCounter();
		Key_Input = ucGetKeyValue();	
		//if(Key_Input>=0x30 && Key_Input<=0x3F)
		//{
			switch(Key_Input)
			{
				case KEY_0:
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
				case KEY_5:
				case KEY_6:
				case KEY_7:
				case KEY_8:
				case KEY_9:
					{
                        _TIMER_FLAG_=0;
						if(InputCnt < PW_LEN)
						{
							if(bSecondInput == 0)				//第一次输入
							{
								KBuff[InputCnt] =  Key_Input;
							}
							else
							{
								Pin_Save[InputCnt] =  Key_Input;
							}
							DispKeyPad[SCREENLEN - PW_LEN + InputCnt] = '_';
							InputCnt++;
#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
						    //vLcd_Flag_Clear();
							//vLcd_String_Clear();

							//vLcd_Flag_Display(7,1);		//密码
							//vLcd_Flag_Display(6,1);		//开机
							//vLcd_Flag_Display(5,1);		//新
							///vLcd_String_Display(DispKeyPad, SCREENLEN);
							vLcd_Bcd_Display2((InputCnt+2),14);		//'_'
#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif
						}
						break;
					}
					 break;
				case KEY_DEL:   //清屏幕
					{
                        _TIMER_FLAG_=0;
						if(LongPressFlag)
						{
							InputCnt = 0;
							LongPressFlag = 0;
							if(bSecondInput == 0)
							{	
								memset(KBuff, 0x00, PW_LEN);								
							}
							else
							{
								memset(Pin_Save, 0x00, PW_LEN);
							}
							for(i = (SCREENLEN - PW_LEN); i<SCREENLEN; i++)DispKeyPad[i] = '-';
							vLcd_String_Display2(DispKeyPad, SCREENLEN);
							break;
						}
						
						if(InputCnt > 0)
						{
							InputCnt--;
							if(bSecondInput == 0)				//第一次输入
							{
								KBuff[InputCnt] =  0xff;
							}
							else
							{
								Pin_Save[InputCnt] =  0xff;
							}
							DispKeyPad[SCREENLEN - PW_LEN + InputCnt] = '-';
#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
						    //vLcd_Flag_Clear();
							//vLcd_String_Clear();

							//vLcd_Flag_Display(7,1);		//密码
							//vLcd_Flag_Display(6,1);		//开机
							//vLcd_Flag_Display(5,1);		//新
							//vLcd_String_Display(DispKeyPad, SCREENLEN);
							vLcd_Bcd_Display2((InputCnt+3),13);
#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif
						}

					}
					 break;
				case KEY_LEFT:
				case KEY_RIGHT:
				case KEY_DOT:
				    _TIMER_FLAG_=0;
					break;
				default:
					break;
					
			 }

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			if(InputCnt == PW_LEN)
			{
				if(bSecondInput == 0)
				{
					//gSecCount_10S=0;
					InputCnt = 0;
					bSecondInput = 1;
#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();	 	
#endif
					
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif
					
					DispKeyPad[0] = 'P';
					DispKeyPad[1] = '2';

					for(i = (SCREENLEN - PW_LEN); i<SCREENLEN; i++)DispKeyPad[i] = '-';
				}
				else
				{
					if(0 == memcmp(Pin_Save, KBuff, PW_LEN))		//第一次与第二次相等
					{
						if(OTPSECU_OP_OK== CosOperation(OP_SET_PASSWORD, Pin_Save, PW_LEN))
						{

							gUseCase = OTPSTATUS_PWVERIFY;
						
							memset(Pin_Save,0x00,16);
							memset(KBuff,0x00,SCREENLEN);
							
#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
							vLcd_String_Clear2();

#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif
							return;
						}
						else
						{

							memset(Pin_Save,0x00,16);
							memset(KBuff,0x00,SCREENLEN);
							
							#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
							#endif
							vLcd_String_Clear2();
							vLcd_String_Display2("Error",5);
							
#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif
							vScu_TbcIntEn();
							vTbc_Int1HzEn();
							gSecCount_10S=0;
							while(gSecCount_10S < 2);
							vScu_TbcIntDis();
							vTbc_Int1HzDis();
							return;
						}
					}
					else
					{
						//gSecCount_10S=0;
#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
						vLcd_String_Clear2();
						vLcd_String_Display2("Error",5);
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
						vScu_TbcIntEn();
						vTbc_Int1HzEn();
						gSecCount_10S=0;
						while(gSecCount_10S < 2);
						vScu_TbcIntDis();
						vTbc_Int1HzDis();
						return;
					}
				}
#ifdef _FPGA_DEMO_EN_
				vLcd_SetStopMode();	 	
#endif
				vLcd_String_Display2(DispKeyPad, SCREENLEN);
#ifdef _FPGA_DEMO_EN_  
				vLcd_SetDisplayMode();		
#endif
			}
					
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
			
		if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
	    	{
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}
						
		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
	 }
	 vScu_TbcIntDis();
	 vTbc_Int1HzDis();
}
/********************************************************************
Function name:       void vOTP_UnlockPinProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    Unlock The OTP in Manual Operation when The OTP Been Locked  	
********************************************************************/
//解除锁定操作
void vOTP_UnlockPinProcess(void)
{
	UINT8 Key_Input=15,i=0;
	//i = 0;
	UINT8 u1Buffer[CHAL_LEN];
	UINT8 InputCnt = 0;
	UINT16 u2Cnt;

	vClrWdtCounter();
	#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		
#endif
	vLcd_String_Clear2();
	
	vLcd_String_Display2("LocKkkkk", 8);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif

	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	gSecCount_10S=0;
	while(gSecCount_10S < 2);
	
	vScu_TbcIntDis();
	vTbc_Int1HzDis();
	
	if( OTPSECU_OP_OK == CosOperation(OP_UNLOCK_CODE, u1Buffer,InputCnt))
	{
		for(i=0; i<CHAL_LEN; i++)DispKeyPad[i] = u1Buffer[CHAL_LEN - i - 1];		//u1Buffer[i];	显示正反不一样
	}
	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		
#endif
	vLcd_String_Clear2();
	vLcd_String_Display2(DispKeyPad, CHAL_LEN);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
	_TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	while(1)
	{
		vClrWdtCounter();
		
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{
			break;
		}

		if(_TIMER_FLAG_>=TIMEOUT_SEC)
		{	
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			break;
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}

		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
	}

	for(i=0; i<CHAL_LEN; i++)DispKeyPad[i] = '-';
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		
#endif
    //vLcd_Flag_Clear();
	//vLcd_String_Clear();
	
	//vLcd_Flag_Display(8,1);			//锁定
	//vLcd_Flag_Display(7,1);				//mi ma
	vLcd_String_Display2(DispKeyPad, CHAL_LEN);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	while(1)
	{
		vClrWdtCounter();
	
		Key_Input = ucGetKeyValue();	
		//if(Key_Input>=0x30 && Key_Input<=0x3f)
		//{
			//如果是正常的数据，存入接受数组 
			//如果移动了光标位置，则修改光标位置的数据
		switch(Key_Input)
		{
			case KEY_0:
			case KEY_1:
			case KEY_2:
			case KEY_3:
			case KEY_4:
			case KEY_5:
			case KEY_6:
			case KEY_7:
			case KEY_8:
			case KEY_9:
			{
                _TIMER_FLAG_=0;
				if(InputCnt < CHAL_LEN)
				{
					DispKeyPad[InputCnt++] = Key_Input;
#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();		
#endif
				    //vLcd_Flag_Clear();
					//vLcd_String_Clear();
					
					//vLcd_Flag_Display(8,1);			//锁定
					//vLcd_String_Display(DispKeyPad, CHAL_LEN);
					vLcd_Bcd_Display2((InputCnt),(Key_Input - 0x30));		//(CHAL_LEN-InputCnt)
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif				
				}
				break;	
			}
			case KEY_DEL:
				{
                    _TIMER_FLAG_=0;
					if(LongPressFlag)
					{
						LongPressFlag = 0;
						for(i=0; i<CHAL_LEN; i++)DispKeyPad[i] = '-';
						InputCnt = 0;
						//vLcd_String_Clear();
						vLcd_String_Display2(DispKeyPad, CHAL_LEN);
						break;
					}

					if(InputCnt == 0)
					{
						return;
					}
					
					if(InputCnt > 0)
					{
						InputCnt--;
						DispKeyPad[InputCnt] = '-';
#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();		
#endif
				   // vLcd_Flag_Clear();
					//vLcd_String_Clear();
					
					//vLcd_Flag_Display(8,1);			//锁定
					//vLcd_String_Display(DispKeyPad, CHAL_LEN);
					vLcd_Bcd_Display2(InputCnt+1,13);		//(CHAL_LEN-InputCnt-1)
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif
					}
					break;
				}
			case KEY_DOT:			//右移动光标
			case KEY_LEFT:
			case KEY_RIGHT:
			    _TIMER_FLAG_=0;
			default	:
				 break;
		}

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			if(InputCnt == CHAL_LEN)
			{
				for(i=0; i<CHAL_LEN; i++)u1Buffer[i] = DispKeyPad[i];
				if ( OTPSECU_OP_OK == CosOperation(OP_UNLOCK_TOKEN, u1Buffer,InputCnt))
				{
					gUseCase = OTPSTATUS_PWSET;

					#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();	 	
#endif
				    //vLcd_Flag_Clear();
					vLcd_String_Clear2();

					//vLcd_Flag_Display(3,1);		//
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif
					
					return;
				}
				else
				{
#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();	 	
#endif
					vLcd_String_Clear2();
					vLcd_String_Display2("Error",5);
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif
					if(gUnlockPinErr_Count <6)
					{
						gUseCase = OTPSTATUS_UNLOCK;
					}
					else
					{
						gUseCase = OTPSTATUS_INVALID;
						return;
					}
					
					vScu_TbcIntEn();
					vTbc_Int1HzEn();
					gSecCount_10S=0;
					while(gSecCount_10S < 60)
					{
						vClrWdtCounter();
					}
					
					vScu_TbcIntDis();
					vTbc_Int1HzDis();
					return;
				}
			}
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
		
		if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
	    	{
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}		
		
		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
		 
	}
	vScu_TbcIntDis();

}

/********************************************************************
Function name:       void vOtp_RunModeProcesss(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP RUN MODE PROCESS,IT CAN GENERATE THE DYNAMIC  PASSWORD CODE IN THIS STATUS 	
********************************************************************/
//产生挑战码并计算密码PIN密码
void vOTP_RunModeProcess(void)
{
	UINT8 InputCnt = 0;
	UINT8 Key_Input=0xff;
	UINT16 u2Cnt;
	//UINT8 *u1Buffer;
	//gUnlockPinErr_Count = 0;

	memset(DispKeyPad, 0x00, SCREENLEN);
	DispKeyPad[0] = '-';
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 	
#endif
    //vLcd_Flag_Clear();
	vLcd_String_Clear2();

	//vLcd_Flag_Display(4,1);			//信息
	//if(u1LowPowerFlag == 1)
	//{
	//	vLcd_Flag_Display(10,1);//低电池电量标志
	//}
	vLcd_String_Display2(DispKeyPad, 1);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
     _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();

	//系统处于正常模式
	while(1)
	{
		vClrWdtCounter();
		
		Key_Input = ucGetKeyValue();	
		//if(Key_Input>=0x30 && Key_Input<=0x3f)
		//{
			//如果移动了光标位置，则修改光标位置的数据
			switch(Key_Input)
			{
				case KEY_0:
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
				case KEY_5:
				case KEY_6:
				case KEY_7:
				case KEY_8:
				case KEY_9:
					 {
                        _TIMER_FLAG_=0;
					 	DispKeyPad[0] = Key_Input;
#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
						//vLcd_String_Clear();

						//vLcd_String_Display(DispKeyPad, 1);
						vLcd_Bcd_Display2(8,Key_Input - 0x30);
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
						gUseCase = OTPSTATUS_PAY;		

						vScu_TbcIntDis();
						vTbc_Int1HzDis();
						
					 	return;
					 }
				#if 1
				case KEY_DEL:	 
					{	
                        			_TIMER_FLAG_=0;
						if(LongPressFlag)
						{
							LongPressFlag = 0;
							g_ModfyPwFlag = 1;
							gUseCase = OTPSTATUS_PWVERIFY;
							vScu_TbcIntDis();
							vTbc_Int1HzDis();
							return;
						}
					}
					break;
					#endif
				default:
					break;
			 }	

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			if((InputCnt == 0))
			{
				gUseCase = OTPSTATUS_TIMETOKEN;	
				vScu_TbcIntDis();
				vTbc_Int1HzDis();
				return;
			}
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
		if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
	    	{
			/**********直接进入休眠***********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}				 
			
		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
	}
	vScu_TbcIntDis();

}
#endif

void VOTP_TimeToken(void)				//闪动点要显示吗?
{
	UINT8 i;
	UINT8 u1Buffer[8];
	//UINT8 Key_Input;
	u4 u4Minite;
	UINT8 u1TenSecond_Conuter;
	//u1 u1CHeckStatus;
	UINT16 u2Cnt;
	//UINT16 u2time;
	UINT8 u1Time_Dispaly[6];
	UINT8 u1Time_BCD[6];
	UINT8 u1Time_STR[12];
	UINT8 j;

	if(g_ModfyPwFlag==1)
	{		
		memset(u1Time_Dispaly,0,6);
		memset(u1Time_BCD,0,6);
		memset(u1Time_STR,0,12);
		
		vScu_TbcIntEn();
		vTbc_Int1HzEn();		
		gSecCount_10S = 0;
		 
		while(gSecCount_10S < 5)
		{
				vScu_SetSysClkOsc2M();
				GETOTPTime(&u1Time_Dispaly);
			
				for(j=0; j<6; j++)
				{
						u1Time_BCD[j] = DectoBCD(u1Time_Dispaly[j]); 
				}
				HexToStr(u1Time_STR,u1Time_BCD,6);	

				memcpy(u1Buffer,u1Time_STR+8,4);
				memcpy(u1Buffer+4,g_u1SN+6,4);
//				vLcd_String_Display2(u1Buffer,8);	
				vPaper_String_Display(u1Buffer,8);
				memset(u1Buffer,0x00,8);
		}
		
		vScu_TbcIntDis();
		vTbc_Int1HzDis();	
		vScu_PowerIo_ResultInterrupt();
	}
	
	u4Minite = (gTimer_Count_Base + Base_Time())/TIMESTEP;
	#if(GMT)
	u2time = 8*60*60/TIMESTEP;
	u4Minite -= u2time;
	#endif
	
	if(u4Minite != gToken_Time2)
	{
		vScu_PowerIo_ResultInterrupt();
		if(OTPSECU_OP_OK== CosOperation(OP_TIME_TOKEN, u1Buffer))
		{
			for(i=0; i<TIME_TOKEN_LEN; i++)gToken_Save[i] = u1Buffer[i];	
			gToken_Time2 = gToken_Time1;

//			vLcd_String_Display2(u1Buffer, TIME_TOKEN_LEN);
			vPaper_String_Display(u1Buffer,TIME_TOKEN_LEN);
			
			if(g_ModfyPwFlag==2)
			{
				vScu_PowerIo_ResultStandby();
			}
		}
		else
		{
//			vLcd_String_Display2("Error",5);
			vPaper_String_Display("55555555",8);
			
			vScu_TbcIntEn();
			vTbc_Int1HzEn();
			gSecCount_10S=0;
			while(gSecCount_10S < 2);
			vScu_TbcIntDis();
			vTbc_Int1HzDis();
			//gUseCase = OTPSTATUS_SELECTMODE;
			return;
		}
	}
	else
	{
//		vLcd_String_Display2(gToken_Save, TIME_TOKEN_LEN);
		  vPaper_String_Display(gToken_Save,TIME_TOKEN_LEN);
	}

	
	_TIMER_FLAG_ = 0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();	
	
	vScu_TbcWakeupEn();		// bi xu
	
	while(1)
	{     
		vClrWdtCounter();

    u1TenSecond_Conuter = (UINT8)((gTimer_Count_Base + Base_Time())%TIMESTEP);
    if(0 == u1TenSecond_Conuter)
		{
			if(OTPSECU_OP_OK== CosOperation(OP_TIME_TOKEN, u1Buffer))
			{
				for(i=0; i<TIME_TOKEN_LEN; i++)gToken_Save[i] = u1Buffer[i];
				gToken_Time2 = gToken_Time1;
//				vLcd_String_Display2(gToken_Save, TIME_TOKEN_LEN);	
				vPaper_String_Display(gToken_Save,TIME_TOKEN_LEN);
			}
			else
			{
//				vLcd_String_Display2("Error",5);
				  vPaper_String_Display("55555555",8);

				vScu_TbcIntEn();
				vTbc_Int1HzEn();
				gSecCount_10S=0;
				while(gSecCount_10S < 2);
				vScu_TbcIntDis();
				vTbc_Int1HzDis();
				vScu_TbcWakeupDis();
				//gUseCase = OTPSTATUS_SELECTMODE;
				return;
			}
		}

		if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
		{
//			vLcd_All_Clear2();
			Paper_All_Clear();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}
		
		#if 1
		if(g_ModfyPwFlag==1)
		{
			u2Cnt = 0;
			while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
			{
				u2Cnt++	;
			}
			
			if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
			{
				/*******直接进入休眠********/
//				vLcd_All_Clear2();
				Paper_All_Clear();
				vScu_SetSysClk32K(); 
				vScu_StandbyModeEn();
				
			}
			else if(u2Cnt>=2000)
			{
				/*******直接进入休眠********/
				g_ModfyPwFlag = 2;
//				vLcd_All_Clear2();
				Paper_All_Clear();
				vScu_SetSysClk32K(); 
				vScu_StandbyModeEn();	
			}
		}
		#endif
	}
	
}

#if 0
/********************************************************************
Function name:       void vOtp_ActiveProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP ACTIVE MODE PROCESS 	
********************************************************************/
//激活OTP
void vOTP_ActiveProcess(void)
{
    UINT8 Key_Input=0xff,i=0;	 	
	//UINT8 k = 0;
	//UINT8 ActiveBuf[6]={"012345"};
	UINT8 InputCnt = 0;
	UINT8 KBuff[AC_LEN];
	UINT16 u2Cnt;
	UINT8 u1LeftOffset = 0;
	UINT8 u1RightOffset = 0;
	UINT8 u1Tmp;

	memset(KBuff, 0x00, AC_LEN);
	for(i = 0; i<SCREENLEN; i++)DispKeyPad[i] = '-';
	//DispKeyPad[0] = 'k';
	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 	
#endif
    //vLcd_Flag_Clear();
	

	if(u1LowPowerFlag == 1)
	{
		vLcd_String_Display2("LPkkkkkk", 8);
   		vScu_TbcIntEn();
		vTbc_Int1HzEn();
		gSecCount_10S=0;
		while(gSecCount_10S < 2);
		
	}
	vLcd_String_Clear2();
	vLcd_String_Display2(DispKeyPad, SCREENLEN);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif

    _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();

	vIom_LsclkOutDis();

	vScu_IomClkEn();
	vIom_UartDis();
	vScu_UartClkDis();
	IOMP0PU   &= 0xfe;		//GP0关闭上啦
	IOMP0PD   |= 0x01;		//GP0打开下拉
	IOMP0DIR &= 0xfe;		//GP0设置为输入
	
	
	while(1)
	{
		#if(!DEBUG)
		if((u1FirstSetFlag == 0)&&(IOMP0DIN&0x01))		//高电平进入串口		test0406 
		{
			//while(1);
			vOTP_InitProcess();
			return;
		}
		else
		#endif
		{
			vClrWdtCounter();
		
			Key_Input = ucGetKeyValue();	
			//if(Key_Input>=0x30 && Key_Input<=0x3f)
			//{ 		
				//如果移动了光标位置，则修改光标位置的数据
			switch(Key_Input)
			{
				case KEY_0:			//down
				case KEY_1:
				case KEY_2:
				case KEY_3:
				case KEY_4:
				case KEY_5:			//up
				case KEY_6:
				case KEY_7:
				case KEY_8:
				case KEY_9:
				{
	                		_TIMER_FLAG_=0;

					if(LongPressFlag)			
					{
						LongPressFlag = 0;
						if(Key_Input == KEY_0) 			//down
						{
							if(u1RightOffset==1)
							{
								u1LeftOffset = 1;
								u1RightOffset = 0;
								vLcd_String_Clear2();

								if(InputCnt<12)
								{
									memcpy(DispKeyPad, &KBuff[InputCnt - 7], 7);
									DispKeyPad[7] = '-';
									
								}
								else if(InputCnt==12)
								{
									memcpy(DispKeyPad, &KBuff[4], 8);
								}
								vLcd_String_Display2(DispKeyPad, 8);
								
							}
							break;
						}
						else if(Key_Input == KEY_5)			//up
						{
							if(u1LeftOffset==1)
							{
								u1LeftOffset = 0;
								u1RightOffset = 1;
								vLcd_String_Clear2();
								if(InputCnt < AC_LEN)
								{
									u1Tmp = InputCnt - 7;
									
								}
								else if(InputCnt==12)
								{
									u1Tmp = 4;
									
								}
								memcpy(DispKeyPad, KBuff, u1Tmp);
								vLcd_String_Display2(DispKeyPad, u1Tmp);
							}
							break;
						}
						
					}
					
					if((InputCnt < AC_LEN)&&(u1RightOffset==0))
					{
						KBuff[InputCnt] = Key_Input;
						DispKeyPad[InputCnt] = Key_Input;
						InputCnt++;
#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
					      if(InputCnt<8)
					      	{
					      		vLcd_Bcd_Display2(InputCnt,(Key_Input-0x30));
					      	}
						 else if(InputCnt<12)
						 {
						 	u1LeftOffset = 1;
						 	memcpy(DispKeyPad, &KBuff[InputCnt-7], 7);
							DispKeyPad[7] = '-';
							vLcd_String_Display2(DispKeyPad, SCREENLEN);
						 }
						 else if(InputCnt==12)
						 {
						 	u1LeftOffset = 1;
						 	memcpy(DispKeyPad, &KBuff[4], 8);
							vLcd_String_Display2(DispKeyPad, SCREENLEN);
						 }
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
					}
					break;
				}
				case KEY_DEL:
					{
	                   			_TIMER_FLAG_=0;
						//if(LongPressFlag)
						//{
						//	LongPressFlag = 0;
						//	memset(KBuff, 0x00, AC_LEN);
						//	for(i = 0; i<AC_LEN; i++)DispKeyPad[i] = '-';
						//	InputCnt = 0;
						//	vLcd_String_Display2(DispKeyPad, SCREENLEN);
						//	break;
						//}
						
						if((InputCnt > 0)&&(u1RightOffset==0))
						{
							if(LongPressFlag)
							{
								LongPressFlag = 0;
								memset(KBuff, 0x00, AC_LEN);
								for(i = 0; i<AC_LEN; i++)DispKeyPad[i] = '-';
								InputCnt = 0;
								vLcd_String_Display2(DispKeyPad, SCREENLEN);
								break;
							}
							
							InputCnt--;
							
							DispKeyPad[InputCnt] = '-';
							KBuff[InputCnt] = 0x00;
#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
						    	if(InputCnt>=7)
						    	{
						    		memcpy(DispKeyPad, &KBuff[InputCnt-7], 7);
								DispKeyPad[7] = '-';	
								vLcd_String_Display2(DispKeyPad, SCREENLEN);
						    	}
							else
							{
								vLcd_Bcd_Display2((InputCnt+1),13);		//'-'			//12位的就是-1
							}
							
#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif
						}
						break;
					}
				case KEY_DOT:
				case KEY_LEFT:			//左移动光标
				case KEY_RIGHT:			//右移动光标	
				    _TIMER_FLAG_=0;
				    break;
				default:
					break;
			 }	
		}

		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			if(InputCnt == AC_LEN)
			{
				if(OTPSECU_OP_OK ==CosOperation(OP_GEN_SEED, KBuff, AC_LEN))
				{
					vScu_TbcIntEn();
					vTbc_Int1HzEn();
					g_ModfyPwFlag = 0;
					gUseCase = OTPSTATUS_PWSET;
				}
				else
				{
#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();	 	
#endif
					vLcd_String_Clear2();
					vLcd_String_Display2("Error",5);
#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
#endif
					vScu_TbcIntEn();
					vTbc_Int1HzEn();
					gSecCount_10S=0;
					while(gSecCount_10S < 2);
					gUseCase = OTPSTATUS_ACTIVATE;
				}
				vScu_TbcIntDis();
				vTbc_Int1HzDis();
				return;
			}
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
		
		 if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
		{
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn(); 
		}

		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			// vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			#if 1
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))		//仿真测出来的温度不对
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			
			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			#endif
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  		//不能关模拟模块时钟，LCD显示需要
			//vScu_Osc2MDis();

		 }
		 #endif
			
	}
	vScu_TbcIntDis();
	vTbc_Int1HzDis();

}
/********************************************************************
Function name:      void vOTP_PAYProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP KEY SEED IMPORT PROCESS 	
********************************************************************/
void vOTP_PAYProcess(void)
{
	UINT8 Key_Input=0xff;
	UINT8 InputCnt = 1;
	UINT8 u1Buffer[80];
	UINT8 i;
	UINT8 u1LeftOffset = 0;
	UINT8 u1RightOffset = 0;
	UINT8 DotFlag = 0;
	UINT8 EndFlag = 0;
	//UINT8 DotDisplayFlag = 0;
	UINT16 u2Cnt;
	UINT8 u1Tmp1;
	UINT8 u1Tmp2;
	// EndFlag = 0;

	memset(u1Buffer, 0x00, 80);

    _TIMER_FLAG_=0;
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	
	u1Buffer[0] = DispKeyPad[0];

	while(1)
	{
		vClrWdtCounter();
	
		Key_Input = ucGetKeyValue();
		switch(Key_Input)
		{
			case KEY_0:
			case KEY_1:
			case KEY_2:
			case KEY_3:
			case KEY_4:
			case KEY_5:
			case KEY_6:
			case KEY_7:
			case KEY_8:
			case KEY_9:
			{
               		 _TIMER_FLAG_=0;	

				if(LongPressFlag)
				{
					LongPressFlag = 0;
					if(Key_Input==KEY_5)			//up
					{
						if((u1LeftOffset > 0))
						{
							u1LeftOffset--;
							u1RightOffset++;
							//for(i=0; i<SCREENLEN; i++)DispKeyPad[i] = u1Buffer[i + u1LeftOffset];

							u1Tmp1 = u1RightOffset*8;
							u1Tmp2 = InputCnt - u1Tmp1;

#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
							if(u1Tmp2>=8)
							{
								//u1Tmp2 -= 8;
								memcpy(DispKeyPad, &u1Buffer[u1Tmp2-8], 8);
								vLcd_String_Display2(DispKeyPad, SCREENLEN);
							}
							else
							{
								memcpy(DispKeyPad, u1Buffer, u1Tmp2);
								vLcd_String_Clear2();
								vLcd_String_Display2(DispKeyPad, u1Tmp2);
							}

#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif				
					
						}
						break;
					}
					else if(Key_Input==KEY_0)		//down
					{
						if((u1RightOffset > 0))
						{
							u1LeftOffset++;
							u1RightOffset--;
							//for(i=0; i<SCREENLEN; i++)DispKeyPad[i] = u1Buffer[i + u1LeftOffset];
							u1Tmp1 = (u1RightOffset+1)*8;
							u1Tmp2 = InputCnt - u1Tmp1;

							memcpy(DispKeyPad, &u1Buffer[u1Tmp2], 8);
#ifdef _FPGA_DEMO_EN_
							vLcd_SetStopMode();	 	
#endif
						   
							vLcd_String_Display2(DispKeyPad, SCREENLEN);
							if(u1RightOffset==0)
							{	
								vLcd_Dot_Display(EndFlag);
							}
#ifdef _FPGA_DEMO_EN_  
							vLcd_SetDisplayMode();		
#endif	
						}
						break;
					}
					else if(Key_Input==KEY_9)	  //dot
					{
						if((InputCnt> 0x00)&&(InputCnt < PAY_PARA_LEN)&&(u1RightOffset == 0))
						{
							if(DotFlag == 0)
							{
								u1Buffer[InputCnt - 1] |= DOT_OFFSET;

								DotFlag = 1;
								EndFlag= 1;
								//DotDisplayFlag = 1;
							}
								#ifdef _FPGA_DEMO_EN_
								vLcd_SetStopMode();	 	
								#endif
								if(InputCnt < SCREENLEN)
								{
									vLcd_String_Display2(DispKeyPad, InputCnt);
								}
								else
								{
									vLcd_String_Display2(DispKeyPad, SCREENLEN);
								}

								vLcd_Dot_Display(EndFlag);
								#ifdef _FPGA_DEMO_EN_  
								vLcd_SetDisplayMode();		
								#endif
						}
						break;
					}
				}

				if((InputCnt < PAY_PARA_LEN)&&(u1RightOffset == 0)&&(EndFlag < 3))				//&&(EndFlag < 3)
				{
					if(InputCnt < SCREENLEN)
					{
						DispKeyPad[InputCnt] = 	Key_Input;
						
					}
					else
					{
						for(i=0; i<SCREENLEN - 1; i++)
						{
							DispKeyPad[i] = DispKeyPad[i+1];
						}
						DispKeyPad[i] = Key_Input;
						//u1LeftOffset++;
					}
					u1Buffer[InputCnt] = Key_Input;
					InputCnt++;
					if(DotFlag)EndFlag++;
					//DotDisplayFlag = EndFlag;

					if(InputCnt>32)
					{
						u1LeftOffset = 4;
					}
					else if(InputCnt>24)
					{
						u1LeftOffset = 3;
					}
					else if(InputCnt>16)
					{
						u1LeftOffset = 2;
					}
					else if(InputCnt>8)
					{
						u1LeftOffset = 1;
					}

					#ifdef _FPGA_DEMO_EN_
					vLcd_SetStopMode();	 	
					#endif
				 
					if(InputCnt < SCREENLEN)
					{
						vLcd_String_Display2(DispKeyPad, InputCnt);
					}
					else
					{
						vLcd_String_Display2(DispKeyPad, SCREENLEN);
					}

					vLcd_Dot_Display(EndFlag);
					#ifdef _FPGA_DEMO_EN_  
					vLcd_SetDisplayMode();		
					#endif
				}
					
				break;
			}
			case KEY_DEL:
			{
                _TIMER_FLAG_=0;	
				if((InputCnt > 0)&&(u1RightOffset == 0))
				{
					if(LongPressFlag)
					{
						LongPressFlag = 0;
						gUseCase = OTPSTATUS_SELECTMODE;
						vScu_TbcIntDis();
						vTbc_Int1HzDis();
						return;
					}
					
					if(InputCnt <= SCREENLEN)
					{
						#if 1		
						if(EndFlag>0)
						{
							EndFlag--;
							if(0 == EndFlag)
							{
								u1Buffer[InputCnt - 1] &= 0x7f;
								vLcd_Dot_Display(EndFlag);
								DotFlag =0;
								break;		
							}
						}
						
						InputCnt--;
						DispKeyPad[InputCnt] = 0x00;
						u1Buffer[InputCnt] = 0x00;
						
						#endif

#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
					  
						vLcd_String_Display2(DispKeyPad, InputCnt);
						//vLcd_Bcd_Clear(InputCnt);
						vLcd_Bcd_Display2(SCREENLEN- InputCnt, 20);
						vLcd_Dot_Display(EndFlag);
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
					}
					else
					{
						#if 1				
						//u1LeftOffset--;
						//if(EndFlag>0)EndFlag--;
						//if(0 == EndFlag)DotFlag =0;
						if(EndFlag>0)
						{
							EndFlag--;
							if(0 == EndFlag)
							{
								u1Buffer[InputCnt - 1] &= 0x7f;
								vLcd_Dot_Display(EndFlag);
								DotFlag =0;
								break;		
							}
						}
						
						InputCnt--;
						u1Buffer[InputCnt] = 0x00;
						for(i=0; i<SCREENLEN; i++)DispKeyPad[i] = u1Buffer[InputCnt-8+i ];

						if((InputCnt==32)||(InputCnt==24)||(InputCnt==16)||(InputCnt==8))
						{
							u1LeftOffset--;
						}
						#endif

#ifdef _FPGA_DEMO_EN_
						vLcd_SetStopMode();	 	
#endif
					    //vLcd_Flag_Clear();
						//vLcd_String_Clear();

						//if(u1LeftOffset>0)
						//{
						//	vLcd_Flag_Display(11,1);		//左移
						//}
						//else
						//{
						//	vLcd_Flag_Display(11,0);
						//}

						//vLcd_Flag_Display(4,1);		//信息
						vLcd_String_Display2(DispKeyPad, SCREENLEN);
						vLcd_Dot_Display(EndFlag);
#ifdef _FPGA_DEMO_EN_  
						vLcd_SetDisplayMode();		
#endif
						
					}
				}

				if(InputCnt == 0)
				{
					gUseCase = OTPSTATUS_SELECTMODE;
					vScu_TbcIntDis();
					vTbc_Int1HzDis();
					return;
				}
				break;
			}
			default:
				break;
		}

		
		u2Cnt = 0;
		while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
		{
			u2Cnt++	;
		}
		
		if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
		{
			_TIMER_FLAG_=0;
			
			if ( OTPSECU_OP_OK == CosOperation(OP_PAY_TOKEN, u1Buffer,InputCnt))
			{
				DotFlag =0x00;
				EndFlag =0x00;
				InputCnt=0x00;
				//DotDisplayFlag = 0;
#ifdef _FPGA_DEMO_EN_
				vLcd_SetStopMode();	 	
#endif
			    //vLcd_Flag_Clear();
				vLcd_String_Clear2();

				vLcd_String_Display2(u1Buffer, 0x06);
#ifdef _FPGA_DEMO_EN_  
				vLcd_SetDisplayMode();		
#endif

				_TIMER_FLAG_=0;
				vScu_TbcIntEn();
				vTbc_Int1HzEn();
				
				while(1)
				{
					vClrWdtCounter();
					Key_Input = ucGetKeyValue();	
					if((Key_Input>=0x30 && Key_Input<=0x3f))
					{
						vScu_TbcIntDis();
						vTbc_Int1HzDis();
						gUseCase = OTPSTATUS_SELECTMODE;
						return;
					}

					if(_TIMER_FLAG_>=TIMEOUT_SEC)
					{	
						/**********直接进入休眠***********/
						vLcd_All_Clear2();
						vScu_SetSysClk32K(); 
						vScu_StandbyModeEn(); 
					}

					u2Cnt = 0;
					while(!(SCULCDSWEN&SCUPOWERIOSTS)&&(u2Cnt<2000))
					{
						u2Cnt++	;
					}
					
					if((u2Cnt<2000)&&(u2Cnt>10))				//前面超时关机，后面去抖动
					{
						vScu_TbcIntDis();
						vTbc_Int1HzDis();
						gUseCase = OTPSTATUS_SELECTMODE;
						return;
						
					}
					else if(u2Cnt>=2000)
					{
						/*******直接进入休眠********/
						vLcd_All_Clear2();
						vScu_SetSysClk32K(); 
						vScu_StandbyModeEn();	
					}

					#if(YJZX)
					 if(gFlag_1S>=2)
					 {
					 	gFlag_1S = 0;
						//vScu_PdrEn();
						vSec_FreqExcepDetectEn();
						vSec_LightDetectEn(0x03);

						vScu_SetSysClkOsc2M();
						if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
						{
							gUseCase = OTPSTATUS_INVALID;
							return;
						}

						if(2 == ucPowerDetect())//低电压
					   	{	   
					   		gUseCase = OTPSTATUS_INVALID;
							return;
					   	}
						vScu_SetSysClk32K();
						//vAdcBgDis();
						
						//LDO TRIM 到2.05V
						//ANALOGCTRLTRIMR1=0x00;
						//PDR 关闭低电压检测
						//vScu_PdrDis();
				
						vAdcFDDis();
						vAdcLDDis();
						//vScu_AnactrlClkDis();  
						//vScu_Osc2MDis();
					 }
					 #endif
				}
			}
			else
			{
				DotFlag =0x00;
				EndFlag =0x00;
				InputCnt=0x00;
				//DotDisplayFlag = 0;

				gUseCase = OTPSTATUS_SELECTMODE;
#ifdef _FPGA_DEMO_EN_
				vLcd_SetStopMode();	 	
#endif
			    //vLcd_Flag_Clear();
				//vLcd_String_Clear2();

				//vLcd_Flag_Display(2,1);			//错误
				vLcd_String_Display2("Error",5);
#ifdef _FPGA_DEMO_EN_  
				vLcd_SetDisplayMode();		
#endif
				vScu_TbcIntEn();
				vTbc_Int1HzEn();
				gSecCount_10S=0;
				while(gSecCount_10S < 2);
				vScu_TbcIntDis();
				vTbc_Int1HzDis();
			}
				
			
		}
		else if(u2Cnt>=2000)
		{
			/*******直接进入休眠********/
			vLcd_All_Clear2();
			vScu_SetSysClk32K(); 
			vScu_StandbyModeEn();	
		}
		#if 1
	    	if( (_TIMER_FLAG_ >= TIMEOUT_SEC)) 
	    	{
	    		vLcd_All_Clear2();
	    		vScu_SetSysClk32K(); 
	    		vScu_StandbyModeEn(); 
	    	}	
	    	#endif

		#if(YJZX)
		 if(gFlag_1S>=2)
		 {
		 	gFlag_1S = 0;
			//vScu_PdrEn();
			vSec_FreqExcepDetectEn();
			vSec_LightDetectEn(0x03);

			vScu_SetSysClkOsc2M();
			if((vSec_TMPERDetect())||(gu1DetectErrFlag == 1))
			{
				gUseCase = OTPSTATUS_INVALID;
				return;
			}

			if(2 == ucPowerDetect())//低电压
		   	{	   
		   		gUseCase = OTPSTATUS_INVALID;
				return;
		   	}
			vScu_SetSysClk32K();
			//vAdcBgDis();
			
			//LDO TRIM 到2.05V
			//ANALOGCTRLTRIMR1=0x00;
			//PDR 关闭低电压检测
			//vScu_PdrDis();
	
			vAdcFDDis();
			vAdcLDDis();
			//vScu_AnactrlClkDis();  
			//vScu_Osc2MDis();
		 }
		 #endif
			
	}
}
#endif
/********************************************************************
Function name:       void vOtp_ScrapProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP SCRAP PROCESS WHEN OTP UNLOCK TIME IN MANUAL PASSED SIX TIME	
********************************************************************/
void vOTP_InvalidProcess(void)
{
	//u1 i;
	//u4 Time_Temp;
	//u2 u2Cnt;
	//Time_Temp = TBC_COUNT_VALUE;

	//DispKeyPad[0] = (u1)(Time_Temp>>24);
	//DispKeyPad[1] = (u1)(Time_Temp>>16);
	//DispKeyPad[2] = (u1)(Time_Temp>>8);
	//DispKeyPad[3] = (u1)(Time_Temp);
    	//vLcd_Flag_Clear();
	//vLcd_String_Clear();
	//if(u1LowPowerFlag == 1)
	//{
	//	vLcd_Flag_Display(10,1);//低电池电量标志
	//}

	//for(i=0;i<16;i++)
	//{
	//	vLcd_Flag_Display(i,1);	
	//}

//	vLcd_String_Display2("Error",5);			//失效后设备显示Error
	vPaper_String_Display("55555555",8);
	memset(gUserInfo,0x00,32);						//清除种子
	memset(g_u1SeedKey,0x00,16);					//清除密钥
	memset(g_u1MacKey,0x00,16);		
	while(1)
	{
		vClrWdtCounter();
		//vScu_SuspendModeEn();
	}
}


