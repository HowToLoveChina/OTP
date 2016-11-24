
#line 1 "SrcFile\Func\UartFunc.c" /0



















 
 
  
#line 1 ".\SRCFILE\FUNC\INC\IOMFUNC.H" /0
 
 
  
#line 1 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0














 
  
#line 1 "\WORKINGCOPY\NZ3182-1A验证\验证工程\NZ3182-1A\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /0
 
 
 
 
 
 typedef unsigned char      BOOL;
 typedef unsigned char      UINT8;
 typedef unsigned int       UINT16;   
 typedef unsigned long      UINT32;
 
 typedef signed char        INT8;
 typedef signed int         INT16;
 typedef signed long        INT32;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
#line 16 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
  
#line 1 "C:\KEIL4\C51\INC\ABSACC.H" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 24 "C:\KEIL4\C51\INC\ABSACC.H" /1
  
  
  
  
 
#line 29 "C:\KEIL4\C51\INC\ABSACC.H" /0
 
 
 
 
 
 
 
#line 17 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
 
 
 
 
 sfr SP     = 0x81;
 sfr DPL    = 0x82;
 sfr DPH    = 0x83;
 sfr DPL2   = 0x84;
 sfr DPH2   = 0x85;
 sfr DPS    = 0x86;
 sfr PCON   = 0x87; 
 
 
 sfr TCON   = 0x88;
 
 sbit IT0 = 0x88;
 sbit IE0 = 0x89;
 sbit IT1 = 0x8A;
 sbit IE1 = 0x8B;
 sbit TF0 = 0x8D;
 sbit TF1 = 0x8F;
 
 sfr IE   = 0xA8;
 
 sbit EA            = 0xAF;
 sbit ES            = 0xAC;
 sbit ET1           = 0xAB;
 sbit EX1           = 0xAA;
 sbit ET0           = 0xA9;
 sbit EX0           = 0xA8;
 
 sfr IP   = 0xB8;
 
 sbit PS            = 0xBC;
 sbit PT1           = 0xBB;
 sbit PX1           = 0xBA;
 sbit PT0           = 0xB9;
 sbit PX0           = 0xB8;
 
 sfr PSW   = 0xD0;
 
 sbit CY            = 0xD7;
 sbit AC            = 0xD6;
 sbit F0            = 0xD5;
 sbit RS1           = 0xD4;
 sbit RS0           = 0xD3;
 sbit OV            = 0xD2;
 sbit P             = 0xD0;
 
 sfr ACC   = 0xE0;
 
 sfr B     = 0xF0;
 
 
 
 sfr SCUOSCCTRL	= 0x90;
 sfr SCUMCG	    = 0x91;
 sfr SCUPM	    = 0x92;
 sfr SCUPMP	    = 0x93;
 sfr SCUSO	    = 0x94;
 sfr SCUWT	    = 0x95;
 sfr SCUWE0	    = 0x96;
 sfr SCUWE1	    = 0x97;
 sfr SCUIE0	    = 0x98;
 sfr SCUIE1	    = 0x99;
 sfr SCUIS0	    = 0x9A;
 sfr SCUIS1	    = 0x9B;
 sfr SCUCTRL	    = 0x9C;
 sfr SCURNGC	    = 0x9D;
 sfr SCUPC	    = 0x9E;
 sfr SCUSWITCHEN =0X9F;
 sfr SCUTMRCG    =0XF8;
 
 
 
 sfr UARTBUF	  = 0xD4;
 sfr UARTCON	  = 0xD5;
 sfr UARTSTAT  = 0xD6;
 
 
 
 sfr IOMCR		= 0xC0;
 sfr IOMP0DIR	= 0xC1;
 sfr IOMP0PU		= 0xC2;
 sfr IOMP0PD		= 0xC3;
 sfr IOMP0INTS1	= 0xC4;
 sfr IOMP0INTS2	= 0xC5;
 sfr IOMP0DIN	= 0xC6;
 sfr IOMP0DOUT	= 0xC7;
 sfr IOMP0ISR	= 0xC8;
 sfr IOMP1CR		= 0xC9;
 sfr IOMP1DIN	= 0xCA;
 sfr IOMP1ISR	= 0xCB;
 sfr IOMP2DOUT	= 0xCC;
 
 
 
 sfr TM0C		= 0xB0;
 sfr TM1C	    = 0xB1;
 sfr TMMOD		= 0xB2;
 sfr TM0CON		= 0xB3;
 sfr TM1CON		= 0xB4;
 sfr TMINTSTS		= 0xB5;
 
 
 
 sfr TBCADJL		= 0xD8;
 sfr TBCADJH		= 0xD9;
 sfr TBCSTART	= 0xDA;
 sfr TBCDR0      = 0xDE;
 sfr TBCDR1      = 0xDD;
 sfr TBCDR2      = 0xDC;
 sfr TBCDR3      = 0xDB;
 sfr TBCINTSTS	= 0xDF;
 
 
 
 sfr WDTCON		= 0xE4;
 sfr WDTMOD		= 0xE5;
 sfr WDTINTSTS   = 0xE6;
 
 
 
 sfr KBCSTAT	    = 0xA0;
 sfr KBCCR		= 0xA1;
 sfr KBCDAT1		= 0xA2;
 sfr KBCDAT2		= 0xA3;
 
 
 
 sfr SM3CON	    = 0xF4;
 sfr SM3INT		= 0xF5;
 
 
 
 sfr RNGSTART    = 0xEC;
 sfr RNGMODE		= 0xED;
 sfr RNGINT		= 0xEE;
 sfr RNGDATA		= 0xEF;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 3 ".\SRCFILE\FUNC\INC\IOMFUNC.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 158 ".\SRCFILE\FUNC\INC\IOMFUNC.H" /1
 
 
 
 
 
 
#line 164 ".\SRCFILE\FUNC\INC\IOMFUNC.H" /0
 extern void vIom_GpioConfig(UINT8 GPn,UINT8 InOut);
 extern UINT8 uIom_GetGpioVal(UINT8 GPn);
 extern void vIom_SetGpioVal(UINT8 GPn,UINT8 Val);
 
 
 
#line 22 "SrcFile\Func\UartFunc.c" /0
 
  
#line 1 ".\SRCFILE\FUNC\INC\SCUFUNC.H" /0
 
 
  
#line 1 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0














 
  
#line 1 "\WORKINGCOPY\NZ3182-1A验证\验证工程\NZ3182-1A\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /0
 
#line 2 "\WORKINGCOPY\NZ3182-1A验证\验证工程\NZ3182-1A\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /1
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
  
 
  
  
 
 
  
  
  
 
  
  
 
  


 
  
 
  
  
 
 
#line 16 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
#line 16 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
  
#line 1 "C:\KEIL4\C51\INC\ABSACC.H" /0






 
 
 
#line 10 "C:\KEIL4\C51\INC\ABSACC.H" /1
  
 
  
  
  
  
 
  
  
  
  
 
 
 
  
  
  
  
 
  
  
  
  
 
 
 
#line 17 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
#line 17 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
 
#line 19 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /1
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
  
  
  
  
  
  
 
  
  
 
 
 
#line 3 ".\SRCFILE\FUNC\INC\SCUFUNC.H" /0
#line 3 ".\SRCFILE\FUNC\INC\SCUFUNC.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 







 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 322 ".\SRCFILE\FUNC\INC\SCUFUNC.H" /1
 
 
#line 324 ".\SRCFILE\FUNC\INC\SCUFUNC.H" /0
 
 
 
#line 23 "SrcFile\Func\UartFunc.c" /0
 
  
#line 1 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /0
 
 
  
#line 1 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0














 
  
#line 1 "\WORKINGCOPY\NZ3182-1A验证\验证工程\NZ3182-1A\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /0
 
#line 2 "\WORKINGCOPY\NZ3182-1A验证\验证工程\NZ3182-1A\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /1
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
  
 
  
  
 
 
  
  
  
 
  
  
 
  


 
  
 
  
  
 
 
#line 16 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
#line 16 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
  
#line 1 "C:\KEIL4\C51\INC\ABSACC.H" /0






 
 
 
#line 10 "C:\KEIL4\C51\INC\ABSACC.H" /1
  
 
  
  
  
  
 
  
  
  
  
 
 
 
  
  
  
  
 
  
  
  
  
 
 
 
#line 17 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
#line 17 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /0
 
 
#line 19 ".\SRCFILE\STARTUP\CPU\REG_Z8D16R.H" /1
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
  
  
  
  
  
  
  
  
 
  
  
 
 
 
#line 3 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /0
#line 3 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 void vUartInit(void);
 UINT8 ucUartGetByte(void);
 void vUartPutByte(UINT8 Byte);
 void vUartRecvData(UINT8 xdata *InData,UINT16 Len);
 void vUartSendData(UINT8 xdata *OutData,UINT16 Len);
 
#line 21 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /1
 
 
 
 
 
 
#line 27 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /0
 
#line 24 "SrcFile\Func\UartFunc.c" /0
 
 
 








 
 void vUartInit(void)
 {
  {SCUMCG|=(1<<5);};
  {IOMCR|=(1<<1);};
  {SCUMCG|=(1<<0);};
 UARTCON=0x00;
 UARTSTAT=0xff;
 
 
 
#line 46 "SrcFile\Func\UartFunc.c" /1
 
 
 
 
 
 
 
 
 
 
#line 56 "SrcFile\Func\UartFunc.c" /0
 }








 
 
 UINT8 ucUartGetByte(void)
 {
 UARTCON|=(1<<1);
 UARTCON|=0x01;
 while(!(UARTSTAT&(1<<3)));
 UARTSTAT|=(1<<3);
 UARTCON=0x00;
 return ((UINT8)UARTBUF);
 }
 








 
 void vUartPutByte(UINT8 Byte)
 {    
 UARTCON&=0xfd;
 UARTBUF=Byte;
 UARTCON|=0x01;
 while(UARTCON&0x01);
 UARTCON=0x00; 	
 }









 
 
 void vUartSendData(UINT8 xdata *OutData,UINT16 Len)
 {
 UINT16 TmpLen;
 TmpLen=Len-1;
 UARTCON&=0xfd;
 UARTBUF=*OutData++;
 UARTCON|=0x01;
 while(TmpLen--)
 {	    
 while((UARTSTAT&(1<<4)));
 UARTBUF=*OutData++;		
 }
 while(UARTCON&0x01);
 UARTCON=0x00; 
 }
 









 
 void vUartRecvData(UINT8 xdata *InData,UINT16 Len)
 {
 UARTCON|=(1<<1);
 UARTCON|=0x01;
 while(Len--)
 {
 while(!(UARTSTAT&(1<<3))); 	    	    
 *InData++=UARTBUF;
 
 }
 UARTCON=0x00;
 }
