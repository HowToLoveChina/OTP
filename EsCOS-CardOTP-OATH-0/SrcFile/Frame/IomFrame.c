#define _IOMFRAME_C_
#include "IOMFRAME.H"
#include "IOMFUNC.H"
#include "DEBUG.H"

//UINT16 _TIMER_FLAG_;
UINT16 _TIMER_FLAG_1_;

void vKeboardInit(void)
{
	 vScu_IomClkEn();
	 #if 1
	 IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输入
	 IOMP1PU=0xf0; //GP12-GP15上拉
	 IOMP1PD=0x0f; //GP8-GP11下拉
	 	 
	 IOMP1INTS2=0xaa;//GP12-GP15下降沿中断使能//0xaa
	 IOMP1INTS1=0x00; 
	 IOMP1DOUT=0xF0;//GP8-GP11输出低  
	#endif

	#if 0
	 IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输?
	 IOMP1PU=0xb0; //GP12,GP13,GP15上拉
	 IOMP1PU=0x0f; //GP14下拉GP8-GP11下拉
	 
	 IOMP1PD=0xf0; //GP8-GP11上拉
	 	 
	 IOMP1INTS2=0x55;//GP12-GP15上降沿中断使能//0xaa
	 IOMP1INTS1=0x00; 
	 IOMP1DOUT=0xFF;//GP8-GP11输出高
	 #endif

	 vScu_GpioWakeupEn();
	 vScu_GpioIntEn();
	 ES=1;
	 EA=1;
}

UINT8 ucGetKeyValue(void)
{
   static UINT8 KEY_STAT=0,KeyTmp[3]; 
   UINT8 i; 
   if(0x55==_KBC_INT_FLAG_)//在中断被置起
   {
        
        _KBC_INT_FLAG_=0x00;
		
   		if(KEY_STAT==KEY_STAT_PRESS) //有按键按下，开启TIMER定时器
		{	
			LongPressFlag = 0;
		     //vTimer1Start(0,TIMER_MODE2,0x8000,TM1_32KEN); 	//定时1S
		     _TIMER_FLAG_1_ = 0x00;		 	      
			 KeyTmp[0]=_IOM_FLAG_&0xF0; 				 
			 _IOM_FLAG_=0x00;			 
			 if(0x00!=(KeyTmp[0]))
			 {		     

			     KEY_STAT=KEY_STAT_RELEASE; 				  	 
				 IOMP1DIR=0xf0;//GP8-GP11输入，GP12-GP15输出
				 IOMP1PU=0x0f; //GP8-GP11上拉
	             		IOMP1PD=0xf0; //GP12-GP15下拉


				 IOMP1INTS1=0x55;//GP8-GP11上升沿中断使能	             
	             
				 IOMP1INTS2=0x00;
				 IOMP1DOUT=0x0f;//GP12-GP15输出低 
				 
				 vScu_GpioWakeupEn();
	             		vScu_GpioIntEn();				 			  
				 
			 }
			 else
			 {
			 	  //vTimer0_Stop();
				  _TIMER_FLAG_1_ = 0;
			 }
			 
		}
		else if(KEY_STAT==KEY_STAT_RELEASE)//按键释放
		{			 
			 //vTimer0_Stop();
			 KeyTmp[1]=_IOM_FLAG_&0x0f;
			 KEY_STAT=KEY_STAT_PRESS; 
			  
			#if 1
			 //IOMP1DOUT=0x00;//GP8-GP11输出低
			 IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输入
		         IOMP1PU=0xf0; //GP12-GP15上拉
		         IOMP1PD=0x0f; //GP8-GP11下拉
		 	 
		         IOMP1INTS2=0xaa;//GP12-GP15下降沿中断使能		//0xaa
		         IOMP1INTS1=0x00; 
		         IOMP1DOUT=0xF0;//GP8-GP11输出低		
		         #endif

			 #if 0
			 IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输入
			 IOMP1PU=0x0f; //GP12-GP15下拉
			 IOMP1PD=0xf0; //GP8-GP11上拉
			 	 
			 IOMP1INTS2=0x55;//GP12-GP15上降沿中断使能//0xaa
			 IOMP1INTS1=0x00; 
			 IOMP1DOUT=0xFF;//GP8-GP11输出高
			 #endif
			 	

			 _IOM_FLAG_=0x00;  			 
			 	 

			 if(_TIMER_FLAG_1_>=LONG_PRESS_TIME)	 //长按
			 {
			      _TIMER_FLAG_1_=0;
				  for(i=0;i<16;i++)
				  {
				  	  if((KeyTmp[1]|KeyTmp[0])==KeyTable[i]) 
					  {
						   //return (~(0x30+i));
						   LongPressFlag = 1;
						   return (0x30+i);
					  }
				  }
			 	  
			 }
			 else
			 {
			      _TIMER_FLAG_1_=0;
			 	  for(i=0;i<16;i++)
				  {
				  	  if((KeyTmp[1]|KeyTmp[0])==KeyTable[i])
					  {
						   return (0x30+i);
					  }
				  }
			 } 			
				 
		 }
		 else
		 {
			 	 KEY_STAT=KEY_STAT_PRESS; 
				 #if 1
				  IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输入
			         IOMP1PU=0xf0; //GP12-GP15上拉
			         IOMP1PD=0x0f; //GP8-GP11下拉
			 	 
			         IOMP1INTS2=0xaa;//GP12-GP15下降沿中断使能
			         IOMP1INTS1=0x00; 
			         IOMP1DOUT=0xF0;//GP8-GP11输出低
				#endif

				#if 0
				 IOMP1DIR=0x0f;//GP8-GP11输出，GP12-GP15输入
				 IOMP1PU=0x0f; //GP12-GP15下拉
				 IOMP1PD=0xf0; //GP8-GP11上拉
				 	 
				 IOMP1INTS2=0x55;//GP12-GP15上降沿中断使能//0xaa
				 IOMP1INTS1=0x00; 
				 IOMP1DOUT=0xFF;//GP8-GP11输出高
				 #endif

				 //vTimer0_Stop();

				 vScu_GpioIntEn();
				 return 0xff;
		 }
	   
   }
   else
   {
        //超时处理，按键一直没有释放，关定时器，给出常按键值
		 if(_TIMER_FLAG_1_>=PRESS_TIME_OUT)	 //长按超时
		 {
		      
		      //vTimer0_Stop();
			  
			  _TIMER_FLAG_1_=0;
			  KEY_STAT=KEY_STAT_PRESS;
			  _IOM_FLAG_=0;
			   _KBC_INT_FLAG_=0x00;

			  vKeboardInit();	
			  
			  return(0x55);
		 }
   		//低功耗suspend MODE 	
		//_KBC_INT_FLAG_=0x00;
		//vScu_SuspendModeEn(); 
				
		return 0xff;
   }
   return 0xff;

}

#if 0
void vKeyboardTest(void)
{
    UINT8 TmpData;
	vKeboardInit();
    while(1)
    {
	   
	   //DebugPrintf("IOMP1DIN=%02bx\r\n",IOMP1DIN);
       TmpData=ucGetKeyValue();
  	   if(0xff!=TmpData)
	   {
	   	   DebugPrintf("TmpData=%02bx\r\n",TmpData);
	   }
	   else
	   {
	       //DebugPrintf("IOMP1DIN=%02bx\r\n",IOMP1DIN);
		   //DebugPrintf("TmpData=%02bx\r\n",TmpData);
	   }
	    
	  
    }
}



void vIomVerify(void)
{
    //UINT8 TmpData;
	DebugPrintf("Call Function:vIomVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	vScu_IomClkEn();
	switch(CMD_INS)
	{		
		case 0x04:	//08 04 00 00 00 
		      vKeyboardTest();
			  break;
		case 0x05:	//08 05 00 00 00
		 
		      IOMP1DIR=CMD_P1;//GP8-GP11输出，GP12-GP15输入	
			  DebugPrintf("IOMP1DIR=%02bx\r\n",IOMP1DIR); 	            
			  break;
		case 0x06:	//08 06 00 00 00		      

	          IOMP1DOUT=CMD_P1;//GP8-GP11输出低  
			  DebugPrintf("IOMP1DOUT=%02bx\r\n",IOMP1DOUT); 
			  break;

		case 0x07:	//08 05 00 00 00
		 
		      IOMP1PU=CMD_P1;//GP8-GP11输出，GP12-GP15输入	
			  DebugPrintf("IOMP1PU=%02bx\r\n",IOMP1PU); 	            
			  break;
		case 0x08:	//08 06 00 00 00		      

	          IOMP1PD=CMD_P1;//GP8-GP11输出低  
			  DebugPrintf("IOMP1PD=%02bx\r\n",IOMP1PD); 
			  break;

		case 0x09:	//08 05 00 00 00
		 
		      IOMP0DIR=CMD_P1;//GP8-GP11输出，GP12-GP15输入	
			  DebugPrintf("IOMP0DIR=%02bx\r\n",IOMP0DIR); 	            
			  break;
		case 0x10:	//08 06 00 00 00		      

	          IOMP0DOUT=CMD_P1;//GP8-GP11输出低  
			  DebugPrintf("IOMP0DOUT=%02bx\r\n",IOMP0DOUT); 
			  break;
		 case 0x11:	//08 05 00 00 00
		 
		      IOMP0PU=CMD_P1;//GP8-GP11输出，GP12-GP15输入	
			  DebugPrintf("IOMP0PU=%02bx\r\n",IOMP0PU); 	            
			  break;
		case 0x12:	//08 06 00 00 00		      

	          IOMP0PD=CMD_P1;//GP8-GP11输出低  
			  DebugPrintf("IOMP0PD=%02bx\r\n",IOMP0PD); 
			  break;
		case 0x13:	//08 06 00 00 00		      

	          IOMCR=CMD_P1;//GP8-GP11输出低  
			  DebugPrintf("IOMCR=%02bx\r\n",IOMCR); 
			  break;
		case 0x14:	//08 06 00 00 00
              DebugPrintf("IOMCR=%02bx\r\n",IOMCR);

		      DebugPrintf("IOMP0DIR=%02bx\r\n",IOMP0DIR); 		      
			  DebugPrintf("IOMP0DOUT=%02bx\r\n",IOMP0DOUT);
	          DebugPrintf("IOMP0PU=%02bx\r\n",IOMP0PU);  
			  DebugPrintf("IOMP0PD=%02bx\r\n",IOMP0PD); 

			  DebugPrintf("IOMP1DIR=%02bx\r\n",IOMP1DIR); 		      
			  DebugPrintf("IOMP1DOUT=%02bx\r\n",IOMP1DOUT);
	          DebugPrintf("IOMP1PU=%02bx\r\n",IOMP1PU);  
			  DebugPrintf("IOMP1PD=%02bx\r\n",IOMP1PD);
			  break;


		 default:
		      DebugPrintf("vKeyboardTest!\r\n");
		      vKeyboardTest();
		      break;
	}
	DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
}
#endif

