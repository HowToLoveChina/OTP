#define _TOKENAPP_C_	
//#include "TokenApp.H" 
#include "String.H" 
#include "Sm3Frame.H"
#include "Debug.H"
#include "EWATCHAPP.H" 

void vTokenApp(void)
{
	vScu_SetSysClk32K(); 	

	//RTCʱ�Ӻ��ж�ʹ�ܿ���
	vScu_TbcClkEn();
	//��оƬ��λ���ж�TBC�Ƿ�λ
	if(!(SCUCTRL&0x01))
	{
		SCUCTRL|=0x01;
		if(!(TBCSTART&0x04))//RSTMASK Ϊ0
		{
			if(!(TBCSTART&0x08)) //PARITYDIFFΪ0,TBCģ�鱻�쳣��λ
			{
				vTbcInit();
			}
		}
	}
	vScu_TbcWakeupEn();
	ET0=1;
	EA=1;
	if(FALSE==gActive)
	{
		vUartInit();
	}
	if(3==gUseCase||7==gUseCase)
	{
	    gUseCase=1;
	}
	if(4==gUseCase)	//��ʱ����
	{
	    //���ñ��ݼ�������ʱ
		vTbc_ReadBakCounter();
			 
		if(3600<=(TBC_COUNT_VALUE-gPinErr_Time))
		{
	    	gUseCase=1;
		}
	}

	//vIom_GpioConfig(3,1);
	//vIom_GpioConfig(4,1);

	vLcdInit(LCD_BIAS_1O3,LCD_DUTY_1O4,LCD_FRM_64HZ);
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(4,1);
	vLcd_Flag_Display(9,1);	
	
	vLcd_String_Display("0123456789",10);
	vLcd_SetDisplayMode();  	

	vKeboardInit(); 

    while(1)
	{
		switch(gUseCase)
		{
		    case 0:	       //��������
			    vToken_SeedImport();
				break;
			case 1:			//����PIN 			
				vToken_InputPin();				
				break;
			case 2:			//���ġ�����PIN�� 			
				vToken_SetPin();
				break;
			case 3:			//����״̬,���Ի�����ս״̬�������л�������PIN��״̬
				vToken_RunMode();
				break;
			case 4:			//�ֶ��������
				vToken_UnlockPin();
				break;
			case 5:			//��������		
				vToken_Active();
				break;
			case 6:         //�ֶ���������6�Σ�ʧЧ	
			    vToken_Invalid();	
				break;
			case 7://EWATCH
			    
				vUtc_To_GTM8();
			    break;
			default:
			    break;					
		 }
	 }
}

static void vFlag1S_DispClr(void)
{
		//10s��ʱ������
	if(10==gSecCount_10S)
	{
#ifdef _FPGA_DEMO_EN_
	    vLcd_SetStopMode();	 	    	
#endif
        vLcd_Flag_Display_ForIsr(8,0);
#ifdef _FPGA_DEMO_EN_  
	    vLcd_SetDisplayMode();		
#endif
	}
	else if(20==gSecCount_10S)
	{
#ifdef _FPGA_DEMO_EN_
	    vLcd_SetStopMode();	 	    	
#endif
		vLcd_Flag_Display_ForIsr(12,0);
#ifdef _FPGA_DEMO_EN_  
	    vLcd_SetDisplayMode();		
#endif
	}
	else if(30==gSecCount_10S)
	{
#ifdef _FPGA_DEMO_EN_
	    vLcd_SetStopMode();	 	    	
#endif
		vLcd_Flag_Display_ForIsr(13,0);
#ifdef _FPGA_DEMO_EN_  
	    vLcd_SetDisplayMode();		
#endif
	}
	else if(40==gSecCount_10S)
	{
#ifdef _FPGA_DEMO_EN_
	    vLcd_SetStopMode();	 	    	
#endif
		vLcd_Flag_Display_ForIsr(14,0);
#ifdef _FPGA_DEMO_EN_  
	    vLcd_SetDisplayMode();		
#endif
	}
	else if(50==gSecCount_10S)
	{
#ifdef _FPGA_DEMO_EN_
	    vLcd_SetStopMode();	 	    	
#endif
		vLcd_Flag_Display_ForIsr(15,0);
#ifdef _FPGA_DEMO_EN_  
	    vLcd_SetDisplayMode();		
#endif
	}
	else if(60==gSecCount_10S)
	{
		 gSecCount_10S=0;
		 //vScu_TbcIntDis();
		 //vScu_TbcWakeupDis(); 		 
		 //����STANDBY MODE
		 vScu_SuspendModeEn();
		 
	}

}

/****************************************************************************************
Function name:   void MessageExpand(unsigned char EventLen,unsigned char ChallengeCodeLen)
Input parameters:
    unsigned char EventLen:	 Event factor, An Option parameter in OTP arithmetic Operation
	unsigned char ChallengeCodeLen:Challenge factor,An Option parameter in OTP arithmetic Operation
 
Return:
    void
Function:  
   package  the Current RTC Time ,OTP Key Seed and Event/ChallengeCode factor(if necessary) 
   and padding these Data According to the requirement of the SM3 arithmetic
   Note:
       For SM3 arithmetic perfermance ,the Buffer is dealcred at 0x03A0.
******************************************************************************************/
//����Ϣ����ʱʱ�������չ 
void MessageExpand(unsigned char EventLen,unsigned char ChallengeCodeLen)
{
	unsigned char offset=0; 		
	memset(MessBuf,0,64);
	//������Կ 16B	
	memcpy(MessBuf,KeySeed,16);
	offset=16;
	//������ʱ�� 8B	
	memset(MessBuf+offset,0,4);//gTime_Baseƫ��ʱ��
	memcpy(MessBuf+offset+4,(unsigned char *)&gTimer_Count,4);
	offset+=8;
	//�¼����� 4B 
	if(EventLen!=NULL)
	{
		memcpy(MessBuf+offset,(unsigned char *)&gEvent,EventLen);
		offset+=EventLen;
	}
	//��ս���� 6B 

	if(ChallengeCodeLen!=NULL)
	{	   

        memcpy(MessBuf+offset,gPin_Key,ChallengeCodeLen);	
	    offset+=ChallengeCodeLen;

	}
	

	//���ݳ�����䣬�˴��Ѿ��̶����������OTP���ݳ���С��64Byte	
	MessBuf[offset]=0x80; 
	if(offset>=32)
	{
	    MessBuf[62]=0x01;
	    MessBuf[63]=(offset-32)<<3;
	}
	else
	{
		MessBuf[63]=offset<<3;
	}
}  
/********************************************************************
Function name:       void DynPassWord_SM3(char *DynCode)
Input parameters:
    unsigned char *DynCode: Save The Dynamic password  
Return:
    void
Function:
    Generate Dynamic password 	
********************************************************************/
void DynPassWord_SM3(unsigned char *DynCode)
{
    //unsigned char data i,a;
	unsigned long int xdata *p;
	unsigned long int OD; 
	
#if 0     			
	DebugPrintBuf(MessBuf, 64, "MessBuf");
	IOMP0DOUT|=0x08; 
#endif  	
	vSm3Hash();	
#if 0
	IOMP0DOUT&=(~0x08);    
	DebugPrintBuf(DynBuf, 32, "DynBuf");
	DebugPrintBuf(DynCode, 8, "DynCode"); 
	IOMP0DOUT|=0x08;
#endif

	p=(unsigned long int *)DynBuf;
	OD=(p[0]+p[1]+p[2]+p[3]+p[4]+p[5]+p[6]+p[7]);

#if 0
	IOMP0DOUT&=(~0x08); 
	DebugPrintf("p[0]=%08lx\r\n",p[0]);
	DebugPrintf("p[1]=%08lx\r\n",p[1]);
	DebugPrintf("p[2]=%08lx\r\n",p[2]);
	DebugPrintf("p[3]=%08lx\r\n",p[3]);
	DebugPrintf("p[4]=%08lx\r\n",p[4]);
	DebugPrintf("p[5]=%08lx\r\n",p[5]);
	DebugPrintf("p[6]=%08lx\r\n",p[6]);	
	DebugPrintf("p[7]=%08lx\r\n",p[7]);
	DebugPrintf("OD0=%08lx\r\n",OD);
	DebugPrintf("OD1=%10lu\r\n",OD);
	DebugPrintf("OD2=%10ld\r\n",OD);	
	IOMP0DOUT|=0x08;
#endif   

	//OD=OD%(int)pow(10,_DYNDISP_NUM_);
	//OD=OD%(int)(10*10*10*10*10*10*10*10);//���� _DYNDISP_NUM_������DEMO��_DYNDISP_NUM_==8	
	//sprintf(DynCode,"%10ld",OD);
	sprintf(DynCode,"%10lu",OD);
    
#if 0     
	IOMP0DOUT&=(~0x08);	
	DebugPrintBuf(DynCode, 8, "DynCode");

#endif	
	
}
/********************************************************************
Function name:       void vToken_SeedImport(void)
Input parameters:
    void  
Return:
    void
Function:
    Toekn KEY SEED IMPORT PROCESS 	
********************************************************************/
void vToken_SeedImport(void)
{
#ifdef _FPGA_DEMO_EN_
    vLcd_SetStopMode();
#endif	
	vLcd_String_Display("---nodA---",10);
#ifdef _FPGA_DEMO_EN_	
	vLcd_SetDisplayMode(); 
#endif
	while(FALSE==gActive)
	{
	    /*if(0x3e==ucGetKeyValue())//���ܼ�����ʾʱʹ��
		{
			 gActive=1;
			 gUseCase=5;//Set Pin Mode
#ifdef _FPGA_DEMO_EN_
             vLcd_SetStopMode();
#endif	
             vLcd_String_Clear();
	         vLcd_String_Display("-o-o-o-o-",9);
#ifdef _FPGA_DEMO_EN_	
	         vLcd_SetDisplayMode(); 
#endif		
             //SUSPEND��ʱ2���ӽ���STANDBY MODE
			 vScu_SetSuspendOT_120S();
	         vScu_SuspendOT_ResultStandby();
	         vScu_SuspendOTEn();
			 //TBC����
			 vTbcInit();
			 //��ʱ����TIMER
			 //vScu_SetTimerWakeup_1H();	
	         //vScu_WtWakeupEn();	         
	         //vScu_TimerWakeupEn();	
	         ET1=1;
	         EA=1;	
		} */
	#ifndef _UART_INTER_EN_
	    DebugPrintf("Seed Data Receive:\r\n");
	    vUartRecvData(g_UART_COM_BUF,64);
		g_UART_RECEIVE_OK=1;
		//DebugPrintBuf(g_UART_COM_BUF,64,"Recvive Data:");
	#endif
		
	    if(1==g_UART_RECEIVE_OK)
		{
		     //�ر�UART�жϽ���
		     vScu_UartIntDis();
	         UARTCON=0x00;
			 //��ȡTBC���ۼ�ʱ�䣨UTC������Э��ʱ�䣩
			 gTimer_Count_Base=((UINT32)g_UART_COM_BUF[3]<<24|(UINT32)g_UART_COM_BUF[2]<<16|(UINT32)g_UART_COM_BUF[1]<<8|(UINT32)g_UART_COM_BUF[0]);
			 //DebugPrintf("gTimer_Count_Base=%08lx",gTimer_Count_Base);
			 //����PPM����
			 vScu_TbcClkEn();
             if(0x80&g_UART_COM_BUF[4]) //-ppm
			 {
			 	  TBCADJL=(UINT8)((0x7FFF-((UINT16)(g_UART_COM_BUF[5]<<1))));
	              TBCADJH=(UINT8)((0x7FFF-((UINT16)(g_UART_COM_BUF[5]<<1))>>8));				  
			 }
			 else //+ppm
			 {
			 	  TBCADJL=(UINT8)((0x7FFF+((UINT16)(g_UART_COM_BUF[5]<<1))));
	              TBCADJH=(UINT8)((0x7FFF+((UINT16)(g_UART_COM_BUF[5]<<1))>>8));
				  
			 }
			 //TBC����
			 vTbcInit();
			 //���ñ��ݼ�������ʱ
			 vTbc_ReadBakCounter();
			 gBackupInitVal= TBC_COUNT_VALUE;
			 DebugPrintf("gBackupInitVal=%08lx\r\n",gBackupInitVal);

			 g_UART_RECEIVE_OK=0;
			 //����������Կ
			 memcpy(KeySeed,&g_UART_COM_BUF[6],16);
			 //���ݽ��յ��û���Ϣ��ԭʼ������Կ
			 memcpy(gUserInfo,&g_UART_COM_BUF[22],32);
			 
			 //��ʾ�û����뼤����
#ifdef _FPGA_DEMO_EN_ 
	         vLcd_SetStopMode();
#endif			 
			 vLcd_String_Clear();
			 vLcd_String_Display("oAbCdEFo",8);
#ifdef _FPGA_DEMO_EN_  
	         vLcd_SetDisplayMode();		
#endif
			 
			 //������ɣ�״̬�л�
			 gActive=1;
			 gUseCase=5;//Set Pin Mode

			 
			 //SUSPEND��ʱ2���ӽ���STANDBY MODE
			 vScu_SetSuspendOT_120S();
	         vScu_SuspendOT_ResultStandby();
	         vScu_SuspendOTEn();
			 
			 //��ʱ����TIMER
			 //vScu_SetTimerWakeup_1H();	         
	         //vScu_TimerWakeupEn();	
	         ET1=1;
	         EA=1;
			 
			 //vScu_StandbyModeEn();			 
		}
		else
		{
		     //�͹���STANDBY MODE����һֱδ�ж���������			 
			 //vScu_SuspendModeEn();	//��UART���ò�ѯ��ʽ�������δ����
			 vFlag1S_DispClr();
		}	
	}
	
    //�ر�UARTʱ�ӣ����͹���
#ifndef _Debug_	
	vScu_UartClkDis();//����ʱ���ܹر�ʱ��
#endif
	//�ر�UART�жϽ���
	vScu_UartIntDis();
	UARTCON=0x00;
	ES=1; 
}

/********************************************************************
Function name:       void vInputPin(void)
Input parameters:
    void  
Return:
    void
Function:
    Input Pin Code Process when OTP(been Actived) Power On  	
********************************************************************/
//����PIN ����
void vToken_InputPin(void)
{
	UINT8 Key_Input=15,j,i=0,bInputPin = 1;
	gUseCase = 1;			//ϵͳ��ʾΪ����PIN
	i = 0;
	gCursor_Flag = 0;
	gPinErr_Count = 0; 	
	//LCD��ʾ�û�����PIN
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();			
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(8,1);
	vLcd_Flag_Display(12,1);
	vLcd_Flag_Display(13,1);
	vLcd_Flag_Display(14,1);
	vLcd_Flag_Display(15,1);
			
	vLcd_Flag_Display(9,1);
	vLcd_Flag_Display(3,1);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s���¶�ʱ��ʼ
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	gSecCount_10S=0;

	//SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();

	while(bInputPin==1)
	{
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{ 	
#ifdef _FPGA_DEMO_EN_
	        vLcd_SetStopMode();		        	        
#endif	
            vLcd_Flag_Clear();
            vLcd_Flag_Display(8,1);
			vLcd_Flag_Display(12,1);
			vLcd_Flag_Display(13,1);
			vLcd_Flag_Display(14,1);
			vLcd_Flag_Display(15,1);
			vLcd_Flag_Display(9,1);
			vLcd_Flag_Display(3,1);

			//10s���¶�ʱ��ʼ
            gSecCount_10S=0;
#ifdef _FPGA_DEMO_EN_  
	        vLcd_SetDisplayMode();		
#endif	

			//��������������ݣ������������
			//����ƶ��˹��λ�ã����޸Ĺ��λ�õ�����
			switch(Key_Input)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				     if(gCursor_Flag )		//�й���Ƶ������޸Ĺ���ƶ�λ�õ�����
				     {
					     gCursor_Flag = 0;
					     gPin_Key[i-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                    
#endif
                         //vLcd_String_Clear();
					     vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					     gCursor_Count = 0;
							
				     }
			 	     else
				     {
					     if(i<6)
					     {
						     

						     gPin_Key[i] = Key_Input;
						     i++;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();  	                         	                         
#endif
                             //vLcd_String_Clear();
						     vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif						
                             
					     }						
				     }					
				     break;
				case 0x3a:					     
					 i = 0;
					 for(j=0;j<6;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                    
#endif
                         vLcd_String_Clear();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//���ƶ���� 				
					 if(i>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count < i)
						 {
							 gCursor_Count++;
						 }
						 else
						 {
							 gCursor_Count = 1;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3d:			//���ƶ����
					 if(i>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count > 1)
						 {
							 gCursor_Count--;
						 }
						 else
						 {
							 gCursor_Count = i;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                    
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3E:
				
					 break;
				case 0x3b:			//ȷ��				
					 //������ݳ�������Ҫ���������һ������
					 if(i==6)
					 { 						
						 //��ʼ����,ѡ������㷨
						 if(memcmp(gPin_Save,gPin_Key,6)==0)
						 {
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                          	                        
#endif
                             //vLcd_String_Clear();
							 vLcd_String_Display("-o-o-o-",7); 
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif							 
							 Key_Input = 0;
							 while(Key_Input > 0x3F || Key_Input < 0x30)
							 {
								 Key_Input = ucGetKeyValue();
							 }
							
							 gUseCase = 3;													
							 bInputPin = 0;
						 }
						 else
						 {
							 for(j=0;j<8;j++)
						     {
						 	     vLcd_Bcd_Clear(j);
						     }
							 gPinErr_Count++; 
							 //gErr[6] =(gPinErr_Count/10)+0x30;
							 //gErr[7] =(gPinErr_Count%10)+0x30; 
							 gErr[7] =gPinErr_Count+0x30; 			 

							 //��ʾ������Ϣ																						
							 if(gPinErr_Count>=6)
							 {
								 gPinErr_6TimeFlag = 1;
								 //gPinErr_Time = gTimer_Count_Base+TBC_COUNT_VALUE ; //��¼������ʱ�䣬nСʱ���Զ�����
								 //���ñ��ݼ�������ʱ
			                     vTbc_ReadBakCounter();

								 gPinErr_Time = (TBC_COUNT_VALUE) ;
								 //��ʾ��������
#ifdef _FPGA_DEMO_EN_
	                             vLcd_SetStopMode();	                             		                             
#endif
                                 vLcd_Flag_Clear();
								 vLcd_Flag_Display(6,1);
								 //vLcd_Flag_Display(0,1);
#ifdef _FPGA_DEMO_EN_  
	                             vLcd_SetDisplayMode();		
#endif

								 Key_Input = 0;
								 while(Key_Input > 0x3F || Key_Input < 0x30)
								 {
									 Key_Input = ucGetKeyValue();
								 }
								 gUseCase = 4;
								 bInputPin = 0;

								 gPinErr_Count=0;
							 }
							 else
							 {
								 //���׿���
#ifdef _FPGA_DEMO_EN_
	                             vLcd_SetStopMode();	                             		                            
#endif
                                 vLcd_Flag_Clear();
								 //vLcd_String_Clear();
								 vLcd_Flag_Display(3,1);
								 vLcd_Flag_Display(11,1);
								 vLcd_String_Display(gErr,8);
#ifdef _FPGA_DEMO_EN_  
	                             vLcd_SetDisplayMode();		
#endif
								 //Key_Input = 0;
								 //while(Key_Input > 0x3F || Key_Input < 0x30)
								 //{
									 //Key_Input = ucGetKeyValue();
								 //}
							 }							 							
						 }						
						 i = 0;
						 gCursor_Flag = 0;
					 }
					 break;
			  }
						
		 }
		 else
		 {
			 //�͹���STANDBY MODE 			 
			 //vScu_SuspendModeEn();
			 vFlag1S_DispClr();
		 }
	}
	vScu_TbcIntDis();
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
//����PIN ����
void vToken_SetPin(void)
{
	
	UINT8 Key_Input=15,j,i=0,KBuff[8],bSetPin = 1,bSecondInput=0;
	gUseCase = 2;			//ϵͳ��ʾΪ����PIN
	i = 0;
	gCursor_Flag = 0;
	gPinErr_Count = 0; //����Pin��������



	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(8,1);
	vLcd_Flag_Display(12,1);
	vLcd_Flag_Display(13,1);
	vLcd_Flag_Display(14,1);
	vLcd_Flag_Display(15,1);

	vLcd_Flag_Display(3,1);
	vLcd_Flag_Display(4,1);
	vLcd_Flag_Display(9,1);	

	
	vLcd_Bcd_Display(10,19);
	vLcd_Bcd_Display(9,20);
    vLcd_Bcd_Display(8,1);

#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s���¶�ʱ��ʼ
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	gSecCount_10S=0;


	//SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();

	while(bSetPin==1)
	{  
		    
		
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3F)
		{

			//�ڶ�������
//			if(bSecondInput)
//			{
//#ifdef _FPGA_DEMO_EN_
//	            vLcd_SetStopMode();
//	            vLcd_String_Clear();
//	            vLcd_SetDisplayMode();
//				vDelay1ms(200);
//	            vLcd_SetStopMode();
//#endif					
//				vLcd_Flag_Display(3,1);
//	            vLcd_Flag_Display(4,1);
//	            vLcd_Flag_Display(9,1);
//#ifdef _FPGA_DEMO_EN_  
//	            vLcd_SetDisplayMode();		
//#endif
//			}
			//��������������ݣ������������
			//����ƶ��˹��λ�ã����޸Ĺ��λ�õ�����
			switch(Key_Input)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					 if(gCursor_Flag )		//�й���Ƶ������޸Ĺ���ƶ�λ�õ�����
					 {
						  gCursor_Flag = 0;
						  gPin_Key[i-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                      vLcd_SetStopMode();	                      	                  
#endif
                          //vLcd_String_Clear();
						  vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                      vLcd_SetDisplayMode();		
#endif
						  gCursor_Count = 0;
							
					 }
					 else
					 {
						 if(i<6)
						 {
						     //IOMP0DOUT|=(0x08);

							 gPin_Key[i] = Key_Input;
							 i++;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                         	                        
#endif
                             //vLcd_String_Clear();
							 vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif
                             
							 //IOMP0DOUT&=(~0x08);
						
						 }
					 }
					 break;
				case 0x3a:   //����Ļ
					 i = 0;
					 for(j=0;j<6;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                    
#endif
                         //vLcd_String_Clear();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//���ƶ����
					 if(i>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count < i)
						 {
							 gCursor_Count++;
						 }
						 else
						 {
							 gCursor_Count = 1;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                    
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3d:			//���ƶ����
					 if(i>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count > 1)
						 {
							 gCursor_Count--;
						 }
						 else
						 {
							 gCursor_Count = i;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3E:
					
					 break;
				case 0x3b:			//ȷ��
					 //������ݳ�������Ҫ���������һ������
					 if(i==6)
					 {
						 for(j=0;j<6;j++)
						 {
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode(); 	                         	                         
#endif
                             //vLcd_String_Clear();
							 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif
						 }
						 i = 0;
						 gCursor_Flag = 0;
						 if(bSecondInput == 0)		//��һ������
						 {
							 bSecondInput = 1;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                         	                         
#endif
                             //vLcd_String_Clear();
							 vLcd_String_Display("P----1",6);
							 
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif
							 memcpy(KBuff,gPin_Key,6);
							 
						 }
						 else										//�ڶ�������
						 {
							 //��ʼ����,ѡ������㷨
							 if(memcmp(KBuff,gPin_Key,6)==0)
							 {
								 memcpy(gPin_Save,KBuff,6);
#ifdef _FPGA_DEMO_EN_
	                             vLcd_SetStopMode(); 	                             	                             
#endif
                                 //vLcd_String_Clear();
								 vLcd_String_Display("P----2",6);
#ifdef _FPGA_DEMO_EN_  
	                             vLcd_SetDisplayMode();		
#endif
								 gUseCase = 3;
								 bSetPin = 0;

								 vDelay1ms(200);
							 }
							 else
							 {
								 //�������벻һ��
#ifdef _FPGA_DEMO_EN_
	                             vLcd_SetStopMode();	                             	                             
#endif
                                 //vLcd_String_Clear();
								 vLcd_String_Display("P---no",6);
#ifdef _FPGA_DEMO_EN_  
	                             vLcd_SetDisplayMode();		
#endif
								 bSecondInput = 0;

								 vDelay1ms(200);								
							 }
						 }
						 Key_Input = 0;
						 /*while(Key_Input > 0x3F || Key_Input < 0x30)
						 {
							 Key_Input = ucGetKeyValue();
						 }*/
						 for(j=0;j<6;j++)
						 {
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                         	                         
#endif
                             //vLcd_String_Clear();
							 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	
#endif
                         vLcd_Flag_Clear();
	                     vLcd_String_Clear();
	                     vLcd_Flag_Display(8,1);
	                     vLcd_Flag_Display(12,1);
	                     vLcd_Flag_Display(13,1);
	                     vLcd_Flag_Display(14,1);
	                     vLcd_Flag_Display(15,1);

	                     vLcd_Flag_Display(3,1);
	                     vLcd_Flag_Display(4,1);
	                     vLcd_Flag_Display(9,1);
						 
						  if(bSecondInput == 0)		//��һ������
						  {
						      vLcd_Bcd_Display(10,19);
	                          vLcd_Bcd_Display(9,20);
		                      vLcd_Bcd_Display(8,1);	
						 }
						 else
						 {
						 	 vLcd_Bcd_Display(10,19);
	                         vLcd_Bcd_Display(9,20);
		                     vLcd_Bcd_Display(8,2);
						 }
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
                         //10s���¶�ʱ��ʼ
	                     gSecCount_10S=0;
					 }
					 break;
			 }
						
		 }
		 else
		 {
			  //�͹���suspend MODE			  
			  //vScu_SuspendModeEn();
			   vFlag1S_DispClr();
		 }
	 }
	 vScu_TbcIntDis();
	 vScu_TbcWakeupDis();
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
//�����������
void vToken_UnlockPin(void)
{
	UINT8 Key_Input=15,j,i=0,bUnlockPin=1;
	i = 0;
	gCursor_Flag = 0;

	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(8,1);
	vLcd_Flag_Display(12,1);
	vLcd_Flag_Display(13,1);
	vLcd_Flag_Display(14,1);
	vLcd_Flag_Display(15,1);
		
	vLcd_Flag_Display(6,1);	
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s���¶�ʱ��ʼ
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	gSecCount_10S=0;

	//SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();
	while(bUnlockPin==1)
	{
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{
			//��������������ݣ������������ 
			//����ƶ��˹��λ�ã����޸Ĺ��λ�õ�����
			switch(Key_Input)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					 if(gCursor_Flag )		//�й���Ƶ������޸Ĺ���ƶ�λ�õ�����
					 {
						  gCursor_Flag = 0;
						  gPin_Key[i-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                      vLcd_SetStopMode();	                      	                     
#endif
                          //vLcd_String_Clear();
						  vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                      vLcd_SetDisplayMode();		
#endif
						  gCursor_Count = 0;							
					 }
					 else
					 {
						 if(i<6)
						 {
							 gPin_Key[i] = Key_Input;
							 i++;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode(); 	                          	                         
#endif
                             //vLcd_String_Clear();
							 vLcd_String_Display(gPin_Key,i);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif
						 }
					 }					
					 break;
				case 0x3a:
					 i = 0;
					 for(j=0;j<6;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                     	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//���ƶ����
					 if(i>0)
					 {
						gCursor_Flag = 1;
						if(gCursor_Count < i)
						{
							gCursor_Count++;
						}
						else
						{
							gCursor_Count = 1;
						}
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();		                    	                    
#endif
                       // vLcd_String_Clear();
						vLcd_String_Display(gPin_Key,i);
						vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3d:			//���ƶ����
					 if(i>0)
					 {
						gCursor_Flag = 1;
						if(gCursor_Count > 1)
						{
							gCursor_Count--;
						}
						else
						{
							gCursor_Count = i;
						}
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                    	                    
#endif
                        //vLcd_String_Clear();
						vLcd_String_Display(gPin_Key,i);
						vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3E:
					
					break;
				case 0x3F:
					{
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                    	                   
#endif
                        //vLcd_String_Clear();
						vLcd_String_Display(gPin_Save,6);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
						Key_Input = 0;
						while(Key_Input > 0x3F || Key_Input < 0x30)
						{
							Key_Input = ucGetKeyValue();
						}
					}
					break;
				case 0x3b:			//ȷ��
					//������ݳ�������Ҫ���������һ������
					
					if(i==6)
					{
						//LCD_CLR_ALL();
						//��ʼ����,ѡ������㷨
						if(strncmp(gPin_Save,gPin_Key,6) == 0)
						{
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                        	                       
#endif
                            //vLcd_String_Clear(); 
							vLcd_Flag_Clear();
                            vLcd_Flag_Display(6,1);
						    vLcd_Flag_Display(10,1);
							vLcd_String_Display("P------O",8);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
							Key_Input = 0;
//							while(Key_Input > 0x3F || Key_Input < 0x30)
//							{
//								Key_Input = ucGetKeyValue();
//							}
							for(j=0;j<8;j++)
						    {
#ifdef _FPGA_DEMO_EN_
	                            vLcd_SetStopMode();	                             	                          
#endif
                                //vLcd_String_Clear();
						 	    vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                            vLcd_SetDisplayMode();		
#endif
						    }
							gUnlockPinErr_Count = 0;
							gPinErr_Count = 0;
							bUnlockPin = 0;
							gUseCase = 2;
						}
						else
						{
							gPinErr_Count++; 							
							 
							gErr[6] =(gPinErr_Count/10)+0x30;
							gErr[7] =(gPinErr_Count%10)+0x30;
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                        	                       
#endif
                            //vLcd_String_Clear();
							vLcd_Flag_Clear();
                            vLcd_Flag_Display(6,1);
							vLcd_Flag_Display(11,1);
							vLcd_String_Display(gErr,8);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
							Key_Input = 0;
							while(Key_Input > 0x3F || Key_Input < 0x30)
							{
								Key_Input = ucGetKeyValue();
							}
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                        	                        
#endif						                            
							vLcd_String_Clear();
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
							if(gPinErr_Count>=6)
							{
								//���� 								
								//gUseCase = 5;
								gUseCase = 6;
								bUnlockPin = 0;
								
							}
							else
							{
#ifdef _FPGA_DEMO_EN_
	                            vLcd_SetStopMode();	                             	
#endif
                                vLcd_Flag_Clear();
	                            vLcd_String_Clear();
	                            vLcd_Flag_Display(8,1);
	                            vLcd_Flag_Display(12,1);
	                            vLcd_Flag_Display(13,1);
	                            vLcd_Flag_Display(14,1);
	                            vLcd_Flag_Display(15,1);

	                            vLcd_Flag_Display(6,1);
#ifdef _FPGA_DEMO_EN_  
	                            vLcd_SetDisplayMode();		
#endif
                                //10s���¶�ʱ��ʼ
	                            gSecCount_10S=0;
							
							}
						}
						i=0;
						gCursor_Flag = 0;
					}
					break;
			}			
		
		}
		else
		{
			 //�͹���SUSPEND MODE			 
			 //vScu_SuspendModeEn();
			 vFlag1S_DispClr();
		}
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
//������ս�벢��������PIN����
void vToken_RunMode(void)
{
	UINT8 Key_Input=15,i=0,j,bNormal=1;
	//UINT32	tmp; 	
	UINT8 k = 0;
	gCursor_Flag = 0;
	gUnlockPinErr_Count = 0;

	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 	
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(8,1);
	vLcd_Flag_Display(12,1);
	vLcd_Flag_Display(13,1);
	vLcd_Flag_Display(14,1);
	vLcd_Flag_Display(15,1);

	vLcd_Flag_Display(5,1);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s���¶�ʱ��ʼ
	vScu_TbcWakeupEn();
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	gSecCount_10S=0;

	//SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();

	//ϵͳ��������ģʽ
	while(bNormal == 1)
	{
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{

			//����ƶ��˹��λ�ã����޸Ĺ��λ�õ�����
			switch(Key_Input)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					 //if(bAlwaysbdisplay)break;
					 if(gCursor_Flag )		//�й���Ƶ������޸Ĺ���ƶ�λ�õ�����
					 {
						gCursor_Flag = 0;
						gPin_Key[k-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                                       
#endif
                        //vLcd_String_Clear();	
						vLcd_String_Display(gPin_Key,k);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
						gCursor_Count = 0;
							
					 }
					 else
					 {
						 if(k<8)
						 {
							gPin_Key[k] = Key_Input;
							k++;
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                         	                       
#endif
                            //vLcd_String_Clear();
							vLcd_String_Display(gPin_Key,k);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
							
						 }
					 }					
					 break;
				case 0x3a:	 //ȡ��
					 k = 0;
//#ifdef _FPGA_DEMO_EN_
//	                 vLcd_SetStopMode();	                 	                
//#endif
//                     vLcd_Flag_Clear();
//					 vLcd_Flag_Display(4,0);
//#ifdef _FPGA_DEMO_EN_  
//	                 vLcd_SetDisplayMode();		
//#endif
					 for(j=0;j<8;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                     	                    
#endif
                         //vLcd_String_Clear();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//���ƶ����
					 if(k>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count < k)
						 {
							 gCursor_Count++;
						 }
						 else
						 {
							 gCursor_Count = 1;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,k);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3d:			//���ƶ����
					 if(k>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count > 1)
						 {
							 gCursor_Count--;
						 }
						 else
						 {
							 gCursor_Count = k;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_String_Display(gPin_Key,k);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3E: //�л���EWatch
					 gUseCase=7;
					 bNormal =0;
					 break;
				case 0x3F:
					 //�л�����սģʽ���л�������ģʽ��������
					 for(j=0;j<8;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                     	                     
#endif
                         //vLcd_String_Clear();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 if(bAlwaysbdisplay)
					 {
						 bAlwaysbdisplay = 0;	//�رճ���ģʽ
						 bOpenHash = 0;
						 bCloseLCD = 1;			//��ʱ�ر�LCD
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                     	                     
#endif
                         vLcd_Flag_Clear();							 
						 vLcd_Flag_Display(5,1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif

					 }
					 else
					 {
						 bAlwaysbdisplay = 1;	//�򿪳���ģʽ
						 bOpenHash = 1;			//��ʼ���м���
						 bCloseLCD = 0;			//����LCD
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                    
#endif
						 vLcd_Flag_Clear();
						 vLcd_Flag_Display(4,1); 
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
				case 0x3b:			//ȷ��
					//if(bAlwaysbdisplay)break;
					//������ݳ�������Ҫ���������һ������
					if(k==8)
					{  

//#ifdef _FPGA_DEMO_EN_
//	                    vLcd_SetStopMode();		                     	                   
//#endif
//                        vLcd_Flag_Clear();
//						vLcd_Flag_Display(4,1);
//#ifdef _FPGA_DEMO_EN_  
//	                    vLcd_SetDisplayMode();		
//#endif
						//��ʼ����,ѡ������㷨	
						
						//���ñ��ݼ�������ʱ
			            vTbc_ReadBakCounter();					
						//����SM3�㷨������  
						vScu_Sm3CipherDis();						
						vScu_SetSysClk64K();
						
						//tmp=gTimer_Count_Base+TBC_COUNT_VALUE; 													
						//tmp= tmp/60;
								            

						gTimer_Count=((gTimer_Count_Base+TBC_COUNT_VALUE)-gBackupInitVal)/60;  													
						
						MessageExpand(4,8);
						DynPassWord_SM3(BCD);

#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();		                     	                   
#endif
                        vLcd_Flag_Clear();
						vLcd_Flag_Display(4,1);
//#ifdef _FPGA_DEMO_EN_  
//	                    vLcd_SetDisplayMode();		
//#endif						                    
//							
//#ifdef _FPGA_DEMO_EN_
//	                    vLcd_SetStopMode();	                    	                    
//#endif					
                        //vLcd_String_Clear(); 						
						vLcd_String_Display(BCD+2,8);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif					
                        vScu_SetSysClk32K();
						//10s���¶�ʱ��ʼ
	                    vScu_TbcIntEn();
	                    vTbc_Int1HzEn(); 
                        //10s���¶�ʱ��ʼ
	                    gSecCount_10S=0;
									
						gUseCase = 3;
						gUnlockPinErr_Count = 0;
						k=0;
					}
					else
					{  

//#ifdef _FPGA_DEMO_EN_
//	                    vLcd_SetStopMode();	                    	                   
//#endif
//                        vLcd_Flag_Clear();
//						vLcd_Flag_Display(4,1);
//#ifdef _FPGA_DEMO_EN_  
//	                    vLcd_SetDisplayMode();		
//#endif				//���ñ��ݼ�������ʱ
			            vTbc_ReadBakCounter();
									
						vScu_SetSysClkOsc500K();
						//tmp=gTimer_Count_Base+TBC_COUNT_VALUE; 													
						//tmp= tmp/60;
						gTimer_Count=((gTimer_Count_Base+TBC_COUNT_VALUE)-gBackupInitVal)/60; 													
						//MessageExpand(gUserInfo,tmp,Event,gPin_Key,0); 						
						MessageExpand(4,0); 						
						DynPassWord_SM3(BCD); 

#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                    	                   
#endif
                        vLcd_Flag_Clear();
						vLcd_Flag_Display(4,1);
//#ifdef _FPGA_DEMO_EN_  
//	                    vLcd_SetDisplayMode();		
//#endif												                    
//						 					
//						 					
//#ifdef _FPGA_DEMO_EN_
//	                    vLcd_SetStopMode();	                        
//#endif					
                        //vLcd_String_Clear();	
						vLcd_String_Display(BCD+2,8);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();													
#endif	  				
                        vScu_SetSysClk32K();
						//10s���¶�ʱ��ʼ
	                    vScu_TbcIntEn();
	                    vTbc_Int1HzEn();
                        //10s���¶�ʱ��ʼ
	                    gSecCount_10S=0;		
						gUseCase = 3; 
						k=0;
                        
					}
					Key_Input = 0;
					while(Key_Input > 0x3F || Key_Input < 0x30)
					{
						Key_Input = ucGetKeyValue();
					}
#ifdef _FPGA_DEMO_EN_
	                vLcd_SetStopMode();		                	                
#endif
                    vLcd_Flag_Clear();
					vLcd_Flag_Display(4,0);
#ifdef _FPGA_DEMO_EN_  
	                vLcd_SetDisplayMode();		
#endif
					for(j=0;j<8;j++)
					{
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                   	                   
#endif
                        vLcd_String_Clear(); 
						vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
					}

#ifdef _FPGA_DEMO_EN_
	                vLcd_SetStopMode();		                	
#endif
                    vLcd_Flag_Clear(); 
	                vLcd_String_Clear();
	                vLcd_Flag_Display(8,1);
	                vLcd_Flag_Display(12,1);
	                vLcd_Flag_Display(13,1);
	                vLcd_Flag_Display(14,1);
	                vLcd_Flag_Display(15,1);

	                vLcd_Flag_Display(5,1);
#ifdef _FPGA_DEMO_EN_  
	                vLcd_SetDisplayMode();		
#endif
                    //10s���¶�ʱ��ʼ
	                gSecCount_10S=0;
					k = 0;
					gCursor_Flag = 0;				
				    break;
			 }			
			
		}
		else
		{			
			//�͹���SUSPEND MODE			
			//vScu_SuspendModeEn();
			 vFlag1S_DispClr();
		}
	}
	vScu_TbcIntDis();

}

/********************************************************************
Function name:       void vToken_Active(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP ACTIVE MODE PROCESS 	
********************************************************************/
//����OTP
void vToken_Active(void)
{
    UINT8 Key_Input=15,i=0,j,bActive=1;	 	
	UINT8 k = 0;
	UINT8 ActiveBuf[6]={"012345"};
	gCursor_Flag = 0;
	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	 	
#endif
    vLcd_Flag_Clear();
	vLcd_String_Clear();
	vLcd_Flag_Display(8,1);//�����־
	vLcd_Flag_Display(12,1); //ʱ�������
	vLcd_Flag_Display(13,1);
	vLcd_Flag_Display(14,1);
	vLcd_Flag_Display(15,1);

	vLcd_Flag_Display(7,1);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s���¶�ʱ��ʼ
	
	vScu_TbcIntEn();
	vTbc_Int1HzEn();
	vScu_TbcWakeupEn();
	gSecCount_10S=0;

	//SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();
	
	//ϵͳ��������ģʽ
	while(bActive == 1)
	{

		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{ 		
			//����ƶ��˹��λ�ã����޸Ĺ��λ�õ�����
			switch(Key_Input)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':									 
					 if(gCursor_Flag )		//�й���Ƶ������޸Ĺ���ƶ�λ�õ�����
					 {
						gCursor_Flag = 0;
						gPin_Key[k-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                    vLcd_SetStopMode();	                                     
#endif                        	
						vLcd_String_Display(gPin_Key,k);
#ifdef _FPGA_DEMO_EN_  
	                    vLcd_SetDisplayMode();		
#endif
						gCursor_Count = 0;
							
					 }
					 else
					 {
						 if(k<8)
						 {
							gPin_Key[k] = Key_Input;
							k++;
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                         	                       
#endif					                               
							vLcd_String_Display(gPin_Key,k);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
							
						 }
					 }					
					 break;
				case 0x3a:	 //ȡ��
					 k = 0;	
					 for(j=0;j<8;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                     	                    
#endif                          
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 //10s���¶�ʱ��ʼ
	                 //gSecCount_10S=0;

					 break;
				case 0x3c:			//���ƶ����
					 if(k>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count < k)
						 {
							 gCursor_Count++;
						 }
						 else
						 {
							 gCursor_Count = 1;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();	                      	                     
#endif					                          
						 vLcd_String_Display(gPin_Key,k);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3d:			//���ƶ����
					 if(k>0)
					 {
						 gCursor_Flag = 1;
						 if(gCursor_Count > 1)
						 {
							 gCursor_Count--;
						 }
						 else
						 {
							 gCursor_Count = k;
						 }
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                     
#endif					                          
						 vLcd_String_Display(gPin_Key,k);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
						
					 }
					 break;
				case 0x3E:
					
					 break;
				case 0x3F:
					 break;
				case 0x3b:			//ȷ�� 					
					//������ݳ�������Ҫ���������һ������
					if(k==8)
					{					    
						if(0==memcmp(gPin_Key,ActiveBuf,6))	//Add Your Code Here
						{
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                         	                   
#endif
                            vLcd_Flag_Clear();
                            vLcd_String_Clear();
						    vLcd_Flag_Display(7,1);
						    vLcd_Flag_Display(10,1);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif
                            
							//10s���¶�ʱ��ʼ
	                        gSecCount_10S=0;
							bActive=0;
							//gActive=TRUE;		
						    gUseCase = 2;
						    gUnlockPinErr_Count = 0;
						    k=0;
                        }
						else
						{
#ifdef _FPGA_DEMO_EN_
	                        vLcd_SetStopMode();	                                           
#endif
                            vLcd_Flag_Clear(); 
							vLcd_String_Clear();	
						    vLcd_Flag_Display(7,1);
						    vLcd_Flag_Display(11,1);
#ifdef _FPGA_DEMO_EN_  
	                        vLcd_SetDisplayMode();		
#endif	  					
						
							Key_Input = 0;
							while(Key_Input > 0x3F || Key_Input < 0x30)
							{
								Key_Input = ucGetKeyValue();
							}
		#ifdef _FPGA_DEMO_EN_
			                vLcd_SetStopMode();				                	                
		#endif
		                    vLcd_Flag_Clear();
							vLcd_Flag_Display(7,0);
		#ifdef _FPGA_DEMO_EN_  
			                vLcd_SetDisplayMode();		
		#endif
							for(j=0;j<8;j++)
							{
		#ifdef _FPGA_DEMO_EN_
			                    vLcd_SetStopMode();				                     	                   
		#endif
		                        //vLcd_String_Clear();
								vLcd_Bcd_Clear(j);
		#ifdef _FPGA_DEMO_EN_  
			                    vLcd_SetDisplayMode();		
		#endif
							}
		
		#ifdef _FPGA_DEMO_EN_
			                vLcd_SetStopMode();				                 	
		#endif
		                    vLcd_Flag_Clear();
			                vLcd_String_Clear();
			                vLcd_Flag_Display(8,1);
			                vLcd_Flag_Display(12,1);
			                vLcd_Flag_Display(13,1);
			                vLcd_Flag_Display(14,1);
			                vLcd_Flag_Display(15,1);
		
			                vLcd_Flag_Display(7,1);
		#ifdef _FPGA_DEMO_EN_  
			                vLcd_SetDisplayMode();		
		#endif			

		                    //10s���¶�ʱ��ʼ
	                        gSecCount_10S=0;
							bActive=1;		
						    gUseCase = 5;
						    gUnlockPinErr_Count = 0;
						    k=0;
	                    }		
                       
					}							
				    break;
			 }			
			
		}
		else
		{			
			//�͹���SUSPEND MODE			
			//vScu_SuspendModeEn();
			 vFlag1S_DispClr();
		}
	}
	vScu_TbcIntDis();

}

/********************************************************************
Function name:       void vOtp_ScrapProcess(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP SCRAP PROCESS WHEN OTP UNLOCK TIME IN MANUAL PASSED SIX TIME	
********************************************************************/
void vToken_Invalid(void)
{
#ifdef _FPGA_DEMO_EN_ 
	vLcd_SetStopMode();
#endif
    vLcd_Flag_Clear();			
	vLcd_String_Clear();
	vLcd_Flag_Display(6,1);
	vLcd_String_Display("EEEEEEEEEEEEE",13); 
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif	

    //SUSPEND��ʱ2���ӽ���STANDBY MODE
	vScu_SetSuspendOT_120S();
	vScu_SuspendOT_ResultStandby();
	vScu_SuspendOTEn();			
	while(1)
	{
		bCloseLCD = 0;
		//vScu_StandbyModeEn();
		vScu_SuspendModeEn();
	}	
}


#endif


