#include "REG_Z8D16R_2.h"
#include "Debug.h"
#include "CRC.H"
#include "CMDPROC.h"
#include "TbcFunc.h"
#include "TimerFunc.h" 
#include "ScuFunc.h" 
#include "IomFunc.h" 
#include "IomFrame.h" 
#include "TokenApp.h"
#include "OtpFunc.h"  
#include "UARTFRAME.H"
//#include "LCDFUNC.H"
#include "uc8118.h"
#include "string.H"
#include "OTPFRAME.H"
#include "ADCFUNC.H" 
#include "intrins.H"
#include "TBCFRAME.H" 
#include "ADCFRAME.H" 
#include "Debug.h"  
#include "SM4.h"
#include "typedefine.h"
//#include "SM3.h"
#include "SM4Process.h"
#include "OTPFUNC.h"
#include "WDTFUNC.H"
#include "SecFunc.h"
#include "Hmac.h"
//SM3Context SM3Block;

//extern void vOTP_Ewatch(void);

UINT8 idata _FIRST_POWERON_FLAG_ _at_ 0xb0;

void EnterStandby(void)				//120s进入standby
{
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();
	vScu_SuspendModeEn();
}

void vSysInit(void)
{   
	//UINT8 Key;
	#if 1
	vScu_SetSysClk32K(); 
	
	SCULCDSWEN|=SCUOXDETEN; 

	//vScu_PowerIo_ResultInterrupt();
	vScu_PowerIo_ResultStandby();
	if(gsuperpw == 0)
	{
		gsuperpw = 1;
		vScu_StandbyModeEn();
	}
	
//	vLcdInit(LCD_BIAS_1O3,LCD_DUTY_1O4,LCD_FRM_102HZ);
	ElcPaper_Init();
	vScu_SetSysClkOsc2M();
//	vLcd_All_Clear2();
	Paper_All_Clear();
	vScu_SetSysClk32K(); 
		
	vTbcInit();	
	
	//vUartInit();	

	//vKeboardInit();

	vScu_IomClkEn();
	 vScu_GpioWakeupEn();
	 vScu_GpioIntEn();
	 ES=1;
	 EA=1;

	
    
    //定时唤醒，频率补偿
    #if 1
	if(SCUIS1&0x08)
	{
	    SCUIS1|=0x08;        //清中断状态位	  
	    vScu_SetSysClkOsc2M();
	   // vLcd_SetStopMode();      

        if(WT_TenMin_Flag == 1)        		//一分钟唤醒标志位
        {
    	    if(WT_TenMin_Couter >= WakeUp_MIN)		//十分钟补偿一次
            {
                WT_TenMin_Couter = 0;
                vFreqAdj(1);
            }
            else
            {
            	WT_TenMin_Couter++;
		vScu_SetSysClk32K(); 
            	vScu_WakeUpTimerWakeUp_1Min();         	
            }
        }
        else if(WT_Onehour_Flag == 1)		//一小时唤醒标志位，一小时补偿一次
        {
            vFreqAdj(1);
        }   

        //vDelay1ms(100);                     //延时100毫秒，便于打印信息
        vScu_SetSysClk32K(); 
        vScu_StandbyModeEn();
	}
    #endif   

	#if 0
	#if(!DEBUG)
	vScu_SetSysClkOsc2M();
	if(ucPowerDetect())//低电压
   	{	
   		u1LowPowerFlag = 1;
   	}
	vScu_SetSysClk32K(); 
	//功耗控制部分
	//关闭BG使能
	vAdcBgDis();
	
	//LDO TRIM 到2.05V
	ANALOGCTRLTRIMR1=0x00;
	//PDR 关闭低电压检测
	vScu_PdrDis();	
	#endif
	#endif
	
	#if(YJZX)
	vScu_SecIntEn();
	vSec_XramErrInt_En();
	vSec_IramErrInt_En();
	vSec_IpEalErrInt_En();
	vSec_OtpcPatternErrInt_En();
	#endif
	#endif
	
} 

void Main(void)
{    
    	u1 u1CHeckStatus = 0;
    	//u1 pu1MacKey[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
	//u1 pu1MacKeyBuf[20];
	//u2 u2Outlen;
	//u2 u2MacKeyLen = 6;
	
    	vSysInit();
	
	
	u1FirstSetFlag = 0;

	if((OTPSTATUS_MACKEY==gUseCase)||(OTPSTATUS_SN_TIME==gUseCase))    
	{
		gUseCase = OTPSTATUS_SN_TIME;
	}
	else if((OTPSTATUS_HARDWARE==gUseCase)||(OTPSTATUS_PARACHECK==gUseCase))
	{
		gUseCase = OTPSTATUS_CALIB;
	}
	#if 0
	if((OTPSTATUS_TIMETOKEN==gUseCase)||(OTPSTATUS_PAY==gUseCase)||(OTPSTATUS_SELECTMODE==gUseCase))
	{
		gUseCase = OTPSTATUS_PWVERIFY;
	}
	else if(OTPSTATUS_UNLOCK==gUseCase) //增加自动解锁判断
	{	    
		if(g_AutoUnlockCnt < AUTO_UNLOCK_LIMITED)
		{
			if(Base_Time() - gPinErr_Time > AUTO_UNLOCK_TIME)
			{
				g_AutoUnlockCnt++;
				gPinErr_Count   =0x00;
				gUseCase = OTPSTATUS_PWVERIFY;
			}
		}
	}
	else if((OTPSTATUS_MACKEY==gUseCase)||(OTPSTATUS_SN_TIME==gUseCase))    
	{
		gUseCase = OTPSTATUS_SN_TIME;
	}

	if(g_ModfyPwFlag == 1)					
	{
		g_ModfyPwFlag = 0;
		if(OTPSTATUS_PWSET==gUseCase)
		{
			gUseCase = OTPSTATUS_PWVERIFY;
		}
	}
	#endif

	#if 0
	vScu_SetSysClkOsc2M();
	u1CHeckStatus =  SM4DEC_TEST();			//SM4ENC_TEST();			//任何一个算法模块自检失败，都会使设备失效，进入失效状态
	if(u1CHeckStatus == 0xff)
	{
		gUseCase = OTPSTATUS_INVALID;
	}

	
	if(u1CHeckStatus == 0)					//如果前面的自检错误了，就没有必要再进行下面的自检了，也避免前面自检错误，后面自检正确的情况
	{
		u1CHeckStatus = SHA1_TEST();	
		if(u1CHeckStatus == 0xff)
		{
			gUseCase = OTPSTATUS_INVALID;
		}
	}
	vScu_SetSysClk32K(); 
	#endif
	#if 0
	if(u1CHeckStatus == 0)
	{
		u1CHeckStatus = SM3_TEST();
		if(u1CHeckStatus == 0xff)
		{
			gUseCase = OTPSTATUS_INVALID;
		}
	}
	
	#endif
	

	//#if(!DEBUG)
	//if((SCULCDSWEN&SCUPOWERIOSTS))
	//{	
		//vLcd_All_Clear2();
	//	vScu_PowerIo_ResultStandby();
	//	vScu_StandbyModeEn();	
	//}
	//#endif

	//vScu_PowerIo_ResultStandby();

	//vLcdInit(LCD_BIAS_1O3,LCD_DUTY_1O4,LCD_FRM_102HZ);
	//vScu_SetSysClkOsc2M();
	//vLcd_All_Clear2();
	//vScu_SetSysClk32K(); 
	
	
	if((gUseCase>6))	//激活后打开看门狗
	{
		vWdtInit(WDT_8S);			//打开看门狗
	}
	
	//#if(DEBUG)
	//gUseCase = OTPSTATUS_CALIB;		//test0406
	//#endif
	
	while(1)
	{
		#if 1
		//Key_Input = ucGetKeyValue();		
		//if(0x3B==Key_Input)
		{    
			//判断是按下开机键		
			switch(gUseCase)
			{
			   	case OTPSTATUS_HARDWARE:	       //按键检测
			    case OTPSTATUS_PARACHECK:
				case OTPSTATUS_CALIB: 
				case OTPSTATUS_SN_TIME:			   //SN_时间输入
				case OTPSTATUS_MACKEY:			
					//vOTP_InputMacKey_SeedKey();
					//break;
				//case OTPSTATUS_PARACHECK:			
				//	vOTP_InputHardware_Check();
				//	break;
				case OTPSTATUS_CRCSNTIME:
				    //vOTP_CRC_SN_TIME();
				    vOTP_InitProcess();
				    break;
				case OTPSTATUS_TIMETOKEN:         
				    VOTP_TimeToken();	
					break;
				#if 0
				case OTPSTATUS_ACTIVATE:			//激活流程
					vOTP_ActiveProcess();
					break;
				case OTPSTATUS_PWSET:
					vOTP_SetPinProcess();
					break;
				case OTPSTATUS_PWVERIFY:			
					vOTP_InputPinProcess();
					break;
				case OTPSTATUS_SELECTMODE:			
					vOTP_RunModeProcess();	
					break;
				
				case OTPSTATUS_PAY:
				    vOTP_PAYProcess();
				    break;
				case OTPSTATUS_UNLOCK:
					vOTP_UnlockPinProcess();		//手动解除锁定
					break;
					#endif
				case OTPSTATUS_INVALID:
					vOTP_InvalidProcess();
					break;
				
				default:
				    break;					
			 }
		}
		#endif
	}
}

