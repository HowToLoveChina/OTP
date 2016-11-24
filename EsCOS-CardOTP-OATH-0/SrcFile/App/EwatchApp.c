#define _EWATCHAPP_C_	
#include "EWATCHAPP.H" 
#include "String.H"
//#include "TokenApp.H" 

#include "Debug.H"

/********************************************************************
Function name:       void vOTP_Ewatch(void)
Input parameters:
    void  
Return:
    void
Function:
    OTP e-Watch 	
********************************************************************/
UINT8 volatile CurTimeBuf[9]={"00-00-00"};
UINT8 volatile SetTimeBuf[10]={"0000000000"};
//UINT32 gTbcBackUpTime;

#define hTime	  ((((gTimer_Count_Base+TBC_COUNT_VALUE)-gBackupInitVal)/3600)%24)
#define mTime	  ((((gTimer_Count_Base+TBC_COUNT_VALUE)-gBackupInitVal)/60)%60)
#define sTime	  (((gTimer_Count_Base+TBC_COUNT_VALUE)-gBackupInitVal)%60)

void vTimer_Disp(void)
{
	//UINT8 TmpVal[6]; 
	vScu_TbcClkEn();
	//采用备份计数器计时
	vTbc_ReadBakCounter();
	//sprintf(TmpVal,"%02lu",hTime);	
	//CurTimeBuf[0]=TmpVal[0];
	//CurTimeBuf[1]=TmpVal[1];
	sprintf(CurTimeBuf,"%02lu",hTime);

	//sprintf(TmpVal,"%02lu",mTime);	
	//CurTimeBuf[3]=TmpVal[0];
	//CurTimeBuf[4]=TmpVal[1];
	sprintf(&CurTimeBuf[3],"%02lu",(mTime));
	
	//sprintf(TmpVal,"%02lu",sTime); 	
	//CurTimeBuf[6]=TmpVal[0];
	//CurTimeBuf[7]=TmpVal[1];
	sprintf(&CurTimeBuf[6],"%02lu",sTime);
	
	CurTimeBuf[2]='-';
	CurTimeBuf[5]='-';

#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();	                    	                    
#endif					
    //vLcd_String_Clear();
	
	vLcd_String_Display(CurTimeBuf,9);

#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif		

} 

void vUtc_To_GTM8(void)
{
	 gUseCase = 1;			//系统提示为输入PIN
	 vScu_TbcClkEn();
	 vScu_TbcIntEn();
	 vTbc_Int1HzEn();
	 ET0=1;
	 EA=1;
	 vLcd_Clear_All();
	 while(1)
	 {
		 if(gFlag_1S==0x5a)
		 {
			 gFlag_1S=0x00;
			 vTimer_Disp();
		 }
	 }
}

void vOTP_Ewatch(void)
{

    UINT8 Key_Input=15,j,i=0,bWatch = 1;
	//TBC开启
	//vTbcInit();


	gUseCase = 1;			//系统提示为输入PIN
	i = 8;
	
	gCursor_Flag = 0;
	gPinErr_Count = 0; 
	vScu_TbcIntDis();	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		 	                    
#endif
    vLcd_Clear_All();
	vLcd_String_Display(CurTimeBuf,8);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s重新定时开始
	
	gSecCount_10S=0;
	gFlag_1S=0x00;
	while(bWatch==1)
	{
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{ 			//如果是正常的数据，存入接受数组
			//如果移动了光标位置，则修改光标位置的数据
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
				     if(gCursor_Flag )		//有光标移到，则修改光标移动位置的数据
				     {
					     gCursor_Flag = 0;
					     CurTimeBuf[i-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                    
#endif
                         vLcd_Clear_All();
					     vLcd_String_Display(CurTimeBuf,i);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					     gCursor_Count = 0;
						 i=8;
							
				     }
			 	     else
				     {
					     if(i<8)
					     {
						     

						     CurTimeBuf[i] = Key_Input;
						     i++;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                         	                         
#endif
                             vLcd_Clear_All();
						     vLcd_String_Display(CurTimeBuf,i);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif						
                             
					     }						
				     }					
				     break;
				case 0x3a:					     
					 //i = 0;
					 for(j=0;j<8;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();
	                      	                    
#endif
                         vLcd_Clear_All();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//左移动光标 				
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
                         vLcd_Clear_All();
						 vLcd_String_Display(CurTimeBuf,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3d:			//右移动光标
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
                         vLcd_Clear_All();
						 vLcd_String_Display(CurTimeBuf,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3E:
					 vScu_TbcIntDis();//暂停，开始调整校准时间
					 

					 i = 8;
					 gCursor_Flag = 0;
					 break;
				case 0x3b:			//确认
				    //vTbcInit();
								
					 vScu_TbcIntEn();//设置完成时间初始值，开始启动TBC
					 						 						
					 i = 8;
					 gCursor_Flag = 0; 
					 
					 
					 break;
				case 0x3f:
				     gUseCase=3;
					 bWatch=0;
					 vTimer_Counter_Disp(TBC_COUNT_VALUE);
				     break;
			  }
						
		 }
		 else
		 {
		        if(gFlag_1S==0x5a)
				{
				    gFlag_1S=0x00;
			        CurTimeBuf[7]+=1;
									
					if(0x3a==CurTimeBuf[7])	//秒
					{
					    CurTimeBuf[7]=0x30;
						CurTimeBuf[6]+=1;
					}
					if(0x36==CurTimeBuf[6])
					{
						CurTimeBuf[6]=0x30;
						CurTimeBuf[4]+=1;
					}
					if(0x3a==CurTimeBuf[4])//分
					{
						CurTimeBuf[4]=0x30;
						CurTimeBuf[3]+=1;

						//vFreqAdj();
					}
					if(0x36==CurTimeBuf[3])
					{
						CurTimeBuf[3]=0x30;
						CurTimeBuf[1]+=1;
						//频率补偿
						//EA=0;
	                    //vFreqAdj();
						//EA=1;
						
					}
					if(CurTimeBuf[0]<0x32) //小时
					{
						if(0x3a==CurTimeBuf[1])
						{
							 CurTimeBuf[1]=0x30;
							 CurTimeBuf[0]+=1; 							 
						}
					}
					else
					{
						if(0x34==CurTimeBuf[1])
						{
						    CurTimeBuf[1]=0x30;
							CurTimeBuf[0]=0x30;
						}
					}
	#ifdef _FPGA_DEMO_EN_
		             vLcd_SetStopMode(); 		             	                         
	#endif
	                 //vLcd_Clear_All();
					 vLcd_String_Display(CurTimeBuf,8);
	#ifdef _FPGA_DEMO_EN_  
		             vLcd_SetDisplayMode();		
	#endif						
			   }
		 }
	}
	vScu_TbcIntDis(); 	  
} 

/*void vOTP_Ewatch(void)
{
	
    UINT8 Key_Input=15,j,i=0,bWatch = 1;
	//TBC开启
	vTbcInit();


	gUseCase = 1;			//系统提示为设置PIN
	i = 10;
	
	gCursor_Flag = 0;
	gPinErr_Count = 0; 
	vScu_TbcIntDis();	
#ifdef _FPGA_DEMO_EN_
	vLcd_SetStopMode();		 	                    
#endif
    vLcd_Clear_All();
	vLcd_String_Display(SetTimeBuf,10);
#ifdef _FPGA_DEMO_EN_  
	vLcd_SetDisplayMode();		
#endif
    //10s重新定时开始
	
	gSecCount_10S=0;
	gFlag_1S=0x00;
	while(bWatch==1)
	{
		Key_Input = ucGetKeyValue();	
		if(Key_Input>=0x30 && Key_Input<=0x3f)
		{ 			//如果是正常的数据，存入接受数组
			//如果移动了光标位置，则修改光标位置的数据
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
				     if(gCursor_Flag )		//有光标移到，则修改光标移动位置的数据
				     {
					     gCursor_Flag = 0;
					     SetTimeBuf[i-gCursor_Count] = Key_Input;
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode(); 	                      	                    
#endif
                         vLcd_Clear_All();
					     vLcd_String_Display(SetTimeBuf,i);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					     gCursor_Count = 0;
						 i=10;
							
				     }
			 	     else
				     {
					     if(i<10)
					     {
						     

						     SetTimeBuf[i] = Key_Input;
						     i++;
#ifdef _FPGA_DEMO_EN_
	                         vLcd_SetStopMode();	                         	                         
#endif
                             vLcd_Clear_All();
						     vLcd_String_Display(SetTimeBuf,i);
#ifdef _FPGA_DEMO_EN_  
	                         vLcd_SetDisplayMode();		
#endif						
                             
					     }						
				     }					
				     break;
				case 0x3a:					     
					 //i = 0;
					 for(j=0;j<10;j++)
					 {
#ifdef _FPGA_DEMO_EN_
	                     vLcd_SetStopMode();
	                      	                    
#endif
                         vLcd_Clear_All();
						 vLcd_Bcd_Clear(j);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif
					 }
					 break;
				case 0x3c:			//左移动光标 				
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
                         vLcd_Clear_All();
						 vLcd_String_Display(SetTimeBuf,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3d:			//右移动光标
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
                         vLcd_Clear_All();
						 vLcd_String_Display(SetTimeBuf,i);
						 vLcd_Bcd_Clear(gCursor_Count-1);
#ifdef _FPGA_DEMO_EN_  
	                     vLcd_SetDisplayMode();		
#endif						 
					 }
					 break;
				case 0x3E:
					 vScu_TbcIntDis();//暂停，开始调整校准时间
					 

					 i = 10;
					 gCursor_Flag = 0;
					 break;
				case 0x3b:			//确认
				     
					 gTimer_Count_Base=((UINT32)(SetTimeBuf[0]-0x30))*10*10*10*10*10*10*10*10*10+((UINT32)(SetTimeBuf[1]-0x30))*10*10*10*10*10*10*10*10+((UINT32)(SetTimeBuf[2]-0x30))*10*10*10*10*10*10*10+
					                   ((UINT32)(SetTimeBuf[3]-0x30))*10*10*10*10*10*10+((UINT32)(SetTimeBuf[4]-0x30))*10*10*10*10*10+((UINT32)(SetTimeBuf[5]-0x30))*10*10*10*10+
									   ((UINT32)(SetTimeBuf[6]-0x30))*10*10*10+((UINT32)(SetTimeBuf[7]-0x30))*10*10+((UINT32)(SetTimeBuf[8]-0x30))*10+((UINT32)(SetTimeBuf[9]-0x30));

					 //sprintf(gTimer_Count_Base,"%lx",SetTimeBuf);
					 //memcpy(&gTimer_Count_Base,SetTimeBuf,10);
					 //sprintf(&gTimer_Count_Base,"%lx",(UINT32)SetTimeBuf);


				     vTbcInit();
					 //vTimer_Counter_Disp(TBC_COUNT_VALUE);			
					 vScu_TbcIntEn();//设置完成时间初始值，开始启动TBC
					 						 						
					 i = 10;
					 gCursor_Flag = 0; 
					 
					 vLcd_Clear_All();
					 break;
				case 0x3f:
				     gUseCase=3;
					 bWatch=0;
					 vTimer_Counter_Disp(TBC_COUNT_VALUE);
				     break;
			  }
						
		 }
		 else
		 {
		        if(gFlag_1S==0x5a)
				{
				    gFlag_1S=0x00;
			        vTimer_Disp(); 				
			   }
		 }
	}
	vScu_TbcIntDis(); 	

}*/







#endif
