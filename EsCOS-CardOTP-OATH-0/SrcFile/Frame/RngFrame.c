#define _RNGRAME_C_	
#include "RNGFRAME.H" 
#include "Debug.H"
#include "UARTFRAME.H"
#include "UARTFUNC.H"

#if 0
void vRngVerify(void)
{
#if 1
   UINT8 rngMode;
   do{
	   for(rngMode=0;rngMode<8;rngMode++)
	   {	      
	   	    vRngStartInit(rngMode<<1,(rngMode<<1));
	   		vRngData(g_UART_COM_BUF,64);
			vRngClose();
			DebugPrintf("Rng Mode %02bx\r\n",rngMode);
			//sprintf(g_UART_COM_BUF,"%10lu",RNGDATA);
			DebugPrintBuf(g_UART_COM_BUF, 64, "RNGDATA:");

            vDelay(1000);		
			
	   }
   }while(0);
#endif
}
#endif