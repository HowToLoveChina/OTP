#define _CMDPROC_C_
#include "CMDPROC.H" 

void vCmdErr(void)
{
    //DebugPrintf("No Such Cmd\r\n");
    DebugPrintf("No Such Cmd:CMD_CLA=%02bx CMD_INS=%02bx\r\n",CMD_CLA,CMD_INS);

	DebugPrintf("Enter vTokenApp Test\r\n");
	vTokenApp();
  
}
			   
				 
//cmd_handle_array
code CMD_HANDLE_STRUCTURE Cmd_Handle_Array[] = {
	{0x01, vScuVerify},
	{0x02, vTbcVerify},
	{0x03, vLcdVerify},
	{0x04, vWdtVerify},
	{0x05, vRngVerify},
	{0x06, vAdcVerify},
	{0x07, vTimerVerify},
	{0x08, vIomVerify},
	{0x09, vOtpVerify},	
	{0x10, vSm3Verify},
	{0xFF, vCmdErr}
};


void vCmdHandle(void)
{
    unsigned int i=0;
    unsigned char cmd_len;
	DebugPrintf("vCmdHandle test!\r\n");
    cmd_len = sizeof(Cmd_Handle_Array)/sizeof(Cmd_Handle_Array[0]);
	g_UART_BUF_LEN=0;
	while(1)
	{
	    
	   	while(1)
		{
		    vUartRecvData(g_UART_COM_BUF,5);  			
			g_UART_RECEIVE_OK=0x55;
			break;
		} 
		//vUartRecvCmd();
				
	    if(0x55==g_UART_RECEIVE_OK)		 
		{		    
			g_UART_RECEIVE_OK=0;
			g_UART_BUF_LEN=0;
			for(i = 0; i < cmd_len; i++)
			{
		    	if( CMD_CLA == Cmd_Handle_Array[i].cmd)
	    		{
					Cmd_Handle_Array[i].cmd_handle();
					i = cmd_len;
		    	}
				else if(i == (cmd_len - 1))	//no such cmd
				{
					Cmd_Handle_Array[i].cmd_handle();
				}
				
				//g_UART_COM_BUF[0]=0;
	
			}		
		}

	}
}

