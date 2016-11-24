
#line 1 "SrcFile\Lib\PUTCHAR.c" /0














 
 
  
#line 1 ".\SRCFILE\STARTUP\CPU\TYPEDEFINE.H" /0
 
 
 
 
 
 typedef unsigned char      BOOL;
 typedef unsigned char      UINT8;
 typedef unsigned int       UINT16;   
 typedef unsigned long      UINT32;
 
 typedef signed char        INT8;
 typedef signed int         INT16;
 typedef signed long        INT32;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 


 
 
 
 
 
 
 
#line 17 "SrcFile\Lib\PUTCHAR.c" /0
 
 








 
 extern void vUartPutByte(UINT8 Byte);
 void putchar(unsigned char x)
 {
 vUartPutByte(x);
 }
 
 
 
