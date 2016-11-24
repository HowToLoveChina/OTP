
#line 1 "SrcFile\StartUp\Main.c" /0
  
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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 1 "SrcFile\StartUp\Main.c" /0
 
  
#line 1 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /0
 
 
  
#line 1 "C:\KEIL4\C51\INC\STDIO.H" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
 
 
 
#line 3 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /0
 
  
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
 
 
 
#line 4 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /0
 
  
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
 
 
 
 
 
 
 
 
 
 
 
 
#line 15 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /1
 
 
 
 
 
 
#line 21 ".\SRCFILE\FUNC\INC\UARTFUNC.H" /0
 extern void vUartInit(void);
 extern UINT8 ucUartGetByte(void);
 extern void vUartPutByte(UINT8 Byte);
 extern void vUartRecvData(UINT8 xdata *InData,UINT16 Len);
 extern void vUartSendData(UINT8 xdata *OutData,UINT16 Len);
 
 
#line 5 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /0
 
 
 
 
 
 
 
 








 
 
#line 23 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /1
  
  
 
 
#line 27 ".\SRCFILE\STARTUP\CPU\DEBUG.H" /0
 
 
 
#line 2 "SrcFile\StartUp\Main.c" /0
 
 UINT8 idata _FIRST_POWERON_FLAG_; 
 
 void vSysInit(void)
 {
  {SCUOSCCTRL|=(1<<0);while(!(SCUOSCCTRL&(1<<3)));SCUOSCCTRL&=0xfb;SCUOSCCTRL|=(1<<5);while(!(SCUOSCCTRL&(1<<7)));SCUOSCCTRL&=(~(1<<6));SCUOSCCTRL&=(~(1<<5));SCUOSCCTRL&=(~(1<<0));}; 
 vUartInit();
 }
 
 void Main(void)
 {
 vSysInit();
  printf("Hello Z8D16R OTP DEMO!Compile Time(%s-%s)\r\n", "Oct 29 2014", "16:47:43");
 
 
 
 }
