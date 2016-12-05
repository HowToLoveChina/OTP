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

void EnterStandby(void)				//120s����standby
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

	
    
    //��ʱ���ѣ�Ƶ�ʲ���
    #if 1
	if(SCUIS1&0x08)
	{
	    SCUIS1|=0x08;        //���ж�״̬λ	  
	    vScu_SetSysClkOsc2M();
	   // vLcd_SetStopMode();      

        if(WT_TenMin_Flag == 1)        		//һ���ӻ��ѱ�־λ
        {
    	    if(WT_TenMin_Couter >= WakeUp_MIN)		//ʮ���Ӳ���һ��
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
        else if(WT_Onehour_Flag == 1)		//һСʱ���ѱ�־λ��һСʱ����һ��
        {
            vFreqAdj(1);
        }   

        //vDelay1ms(100);                     //��ʱ100���룬���ڴ�ӡ��Ϣ
        vScu_SetSysClk32K(); 
        vScu_StandbyModeEn();
	}
    #endif   

	#if 0
	#if(!DEBUG)
	vScu_SetSysClkOsc2M();
	if(ucPowerDetect())//�͵�ѹ
   	{	
   		u1LowPowerFlag = 1;
   	}
	vScu_SetSysClk32K(); 
	//���Ŀ��Ʋ���
	//�ر�BGʹ��
	vAdcBgDis();
	
	//LDO TRIM ��2.05V
	ANALOGCTRLTRIMR1=0x00;
	//PDR �رյ͵�ѹ���
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
	else if(OTPSTATUS_UNLOCK==gUseCase) //�����Զ������ж�
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
	u1CHeckStatus =  SM4DEC_TEST();			//SM4ENC_TEST();			//�κ�һ���㷨ģ���Լ�ʧ�ܣ�����ʹ�豸ʧЧ������ʧЧ״̬
	if(u1CHeckStatus == 0xff)
	{
		gUseCase = OTPSTATUS_INVALID;
	}

	
	if(u1CHeckStatus == 0)					//���ǰ����Լ�����ˣ���û�б�Ҫ�ٽ���������Լ��ˣ�Ҳ����ǰ���Լ���󣬺����Լ���ȷ�����
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
	
	
	if((gUseCase>6))	//�����򿪿��Ź�
	{
		vWdtInit(WDT_8S);			//�򿪿��Ź�
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
			//�ж��ǰ��¿�����		
			switch(gUseCase)
			{
			   	case OTPSTATUS_HARDWARE:	       //�������
			    case OTPSTATUS_PARACHECK:
				case OTPSTATUS_CALIB: 
				case OTPSTATUS_SN_TIME:			   //SN_ʱ������
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
				case OTPSTATUS_ACTIVATE:			//��������
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
					vOTP_UnlockPinProcess();		//�ֶ��������
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

