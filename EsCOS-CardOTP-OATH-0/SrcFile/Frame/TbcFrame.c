#define _TBCFRAME_C_
#include "TBCFRAME.H"
#include "TBCFUNC.H"
#include "DEBUG.H"



void vTbcVerify(void)
{
    UINT16 i,j;	
	DebugPrintf("Call Function:vTbcVerify() in File:(%s),Line:%d,\r\n", __FILE__,(UINT16)__LINE__);
	vScu_TbcClkEn();
	switch(CMD_INS)
	{
		 case 0x02://02 02 xx 00 00 
		      vTbcInit();
			  vScu_TbcWakeupEn();
			  if(CMD_P1==0x00)
			  {
	              vTbc_Int10SEn();
			  }
			  else if(CMD_P1==0x01)
			  {
			  	  vTbc_Int1HzEn();
			  }
			  else if(CMD_P1==0x02)
			  {
			  	  vTbc_Int2HzEn();
			  }
			  else
			  {
			  	  vTbc_Int60SEn();
			  }
			  //DebugPrintf("TBCINTSTS=%02bx\r\n",TBCINTSTS);
	          vScu_TbcIntEn();
			  
			  while(1)
			  {
			  	  //vTbc_ReadCurCounter();
				  vTbc_StartDis();//清TBC 计数器，
				  vTbc_StartEn();//清TBC 计数器，
				  vScu_SuspendModeEn();		    
			 	  DebugPrintf("TBC_COUNT_VALUE Cur=%08lx\r\n",TBC_COUNT_VALUE);
				  DebugPrintf("_TBC_FLAG_=%08lx\r\n",_TBC_FLAG_);
			  }
			   
		      break;
		 case 0x03://02 03 xx xx 00 
		      	      
			  vScu_TbcClkEn();
			  if(CMD_P1==0x00)
			  {
				  for(i=0;i<0x400;i++)
				  {
				      DebugPrintf("ppm=%d\r\n",i);
				  	  vTBC_ADJ(i);
					  //TBCADJL=0x87;
			          //TBCADJH=0x77;
					  DebugPrintf("TBCADJL=%02bx\r\n",TBCADJL);
				  	  DebugPrintf("TBCADJH=%02bx\r\n",TBCADJH);
				  }
			  }
			  else
			  {
			  	  for(i=0;i<0x400;i++)
				  {
				      DebugPrintf("ppm=-%d\r\n",i);
					  i|=0x8000;
				  	  vTBC_ADJ(i);
					  //TBCADJL=0x87;
			          //TBCADJH=0x77;
					  DebugPrintf("TBCADJL=%02bx\r\n",TBCADJL);
				  	  DebugPrintf("TBCADJH=%02bx\r\n",TBCADJH);
					  i&=0x7fff;
				  }
			  }			  
		      break;
		  case 0x04://02 04 xx xx 00		      
			  vTbcInit();
			  if(CMD_P1==0X01)	//+1024PPM
			  {
			      TBCADJL=0xff;
			      TBCADJH=0x87;
			  }
			  else //-1024ppm
			  {
			  	  TBCADJL=0xfe;
			      TBCADJH=0x77;
			  
			  }
			  DebugPrintf("TBCADJL=%02bx\r\n",TBCADJL);
			  DebugPrintf("TBCADJH=%02bx\r\n",TBCADJH);			  
			  
			  vScu_TbcWakeupEn();
	          //vTbc_Int1SEn();			  
	          //vScu_TbcIntEn();
			  
			  while(1)
			  {
			  	  vTbc_ReadCurCounter();
				  //vScu_SuspendModeEn();		    
			 	  DebugPrintf("TBC_COUNT_VALUE Cur=%08lx\r\n",TBC_COUNT_VALUE);
				  //DebugPrintf("_TBC_FLAG_=%08lx\r\n",_TBC_FLAG_);
				  vTbc_ReadBakCounter();
				  DebugPrintf("RTC Bak=%08lx\r\n",TBC_COUNT_VALUE);
			  }		   
		      break;
	     case 0x05://02 05 xx xx 00
		     vTbcInit();
			 vTBC_ADJ(0);
			 i=0;
			 //开启定时器中断
			 //ucSysClkChange((UINT8)i);	
			 //vTimer2Start(0x03,0x01,0xff,(0x01<<4));
			 vScu_SetSysClk32K();
			 DebugPrintf("Start RTC Cur=%08lx\r\n",TBC_COUNT_VALUE);
			 vTbc_ReadBakCounter();
			 DebugPrintf("RTC Bak=%08lx\r\n",TBC_COUNT_VALUE);
		     do{			    
			
				//ucSysClkChange((UINT8)i);
				//vScu_Osc2MEn();
				//vTbc_ReadCurCounter();
				//vScu_SetSysClk32K();		    
			 	//DebugPrintf("RTC Cur=%08lx\r\n",TBC_COUNT_VALUE);
				//vTbc_ReadBakCounter();
				//DebugPrintf("RTC Bak=%08lx\r\n",TBC_COUNT_VALUE);
				switch(i%4)
				{
					case 0:
					     j=0x3ff;
						 break;
					case 1:
					     j=0x7fe;
						 break;
					case 2:
					     j=0x3d18;
						 break;
					case 3:
					     j=0x7a36;
						 break;
					default:
					     break;
				}
			    vDelay(j);
					    
			 	vTbc_ReadCurCounter();
				vScu_SetSysClk32K();		    
			 	DebugPrintf("RTC Cur=%08lx\r\n",TBC_COUNT_VALUE);
				vTbc_ReadBakCounter();
				DebugPrintf("RTC Bak=%08lx\r\n",TBC_COUNT_VALUE);

				ucSysClkChange((UINT8)++i);
			 }while(1);//while(ucSysClkChange(i++));
		     break;

		 default:
		     vTbcInit();
			 vTBC_ADJ(0);
			 i=0;
			 //开启定时器中断
			 //ucSysClkChange((UINT8)i);	
			 vTimer2Start(0x03,0x01,0x7f,(0x01<<4));
			 vScu_SetSysClk32K();
		     do{			    
				vDelay(i++);
				//ucSysClkChange((UINT8)i);
				//vScu_Osc2MEn();
				//vTbc_ReadCurCounter();
				//vScu_SetSysClk32K();		    
			 	//DebugPrintf("RTC Cur=%08lx\r\n",TBC_COUNT_VALUE);
				//vTbc_ReadBakCounter();
				//DebugPrintf("RTC Bak=%08lx\r\n",TBC_COUNT_VALUE);
				//ucSysClkChange(i);
			 }while(1);//while(ucSysClkChange(i++));
		     break;
	}
	DebugPrintf("************Excute CMD_INS:%02bx Over************\r\n",CMD_INS);
}
