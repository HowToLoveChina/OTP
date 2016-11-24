/********************************************************************
                          Nationz Co.,Ltd
        Copyright (c) 2009£­2013 All Rights Reserved
 ********************************************************************
File:
History£º
Version                      Author                     Date
V0.1                         nationz              2013-03-13

Description:
     NZ3182 Header File
Function list:
Special statement:
     NULL
 *******************************************************************/
#include "TYPEDEFINE.H"
#include <absacc.h>
#ifndef _REG_Z8D16R_2_H_
#define _REG_Z8D16R_2_H_ 

//SFR REG START
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr DPL2   = 0x84;
sfr DPH2   = 0x85;
sfr DPS    = 0x86;
sfr PCON   = 0x87; 


sfr TCON   = 0x88;
//TCON Bit Dsfine
sbit IT0 = 0x88;
sbit IE0 = 0x89;
sbit IT1 = 0x8A;
sbit IE1 = 0x8B;
sbit TF0 = 0x8D;
sbit TF1 = 0x8F;

sfr IE   = 0xA8;
//IE Bit Define
sbit EA            = 0xAF;
sbit ES            = 0xAC;
sbit ET1           = 0xAB;
sbit EX1           = 0xAA;
sbit ET0           = 0xA9;
sbit EX0           = 0xA8;

sfr IP   = 0xB8;
//IP Bit Define
sbit PS            = 0xBC;
sbit PT1           = 0xBB;
sbit PX1           = 0xBA;
sbit PT0           = 0xB9;
sbit PX0           = 0xB8;

sfr PSW   = 0xD0;
//PSW Bit Define
sbit CY            = 0xD7;
sbit AC            = 0xD6;
sbit F0            = 0xD5;
sbit RS1           = 0xD4;
sbit RS0           = 0xD3;
sbit OV            = 0xD2;
sbit P             = 0xD0;

sfr ACC   = 0xE0;

sfr B     = 0xF0;
//SFR REG END

//SCU MODULE REG DEFINE START 
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
sfr SCUTMRCG    = 0xF8;
sfr SCULCDSWEN  = 0xF9;
sfr SCURAMSWEN  = 0xFA;
//SCU MODULE REG DEFINE END

//UART MODULE REG DEFINE START
sfr UARTBUF	  = 0xD4;
sfr UARTCON	  = 0xD5;
sfr UARTSTAT  = 0xD6;
//UART MODULE REG DEFINE END

//IOM MODULE REG DEFINE START
sfr IOMCR		= 0xC0;
sfr IOMP0DIR	= 0xC1;
sfr IOMP0PU		= 0xC2;
sfr IOMP0PD		= 0xC3;
sfr IOMP0INTS1	= 0xC4;
sfr IOMP0INTS2	= 0xC5;
sfr IOMP0DIN	= 0xC6;
sfr IOMP0DOUT	= 0xC7;
sfr IOMP0ISR	= 0xC8;

sfr IOMP1DIR    = 0xC9;
sfr IOMP1PU     = 0xCA;
sfr IOMP1PD     = 0xCB;
sfr IOMP1INTS1  = 0xCC;
sfr IOMP1INTS2  = 0xCD;
sfr IOMP1DIN    = 0xCE;
sfr IOMP1DOUT   = 0xCF;
sfr IOMP1ISR    = 0xBC;


//IOM MODULE REG DEFINE END

//TIMER MODULE REG DEFINE START
sfr TM0CDATA		= 0xB0;
sfr TM1CDATA	    = 0xB1;
sfr TM1MOD		= 0xB2;
sfr TM0CON		= 0xB3;
sfr TM1CON		= 0xB4;
sfr TM1INTSTS		= 0xB5;

sfr TM2CDATA    = 0xA0;
sfr TM3CDATA    = 0xA1;
sfr TM2MOD       = 0xA2;
sfr TM2CON       = 0xA3;
sfr TM3CON       = 0xA4;
sfr TM2INTSTS		= 0xA5;
//TIMER MODULE REG DEFINE END

//TBC MODULE REG DEFINE START
sfr TBCADJL		= 0xD8;
sfr TBCADJH		= 0xD9;
sfr TBCSTART	= 0xDA;
sfr TBCDR0      = 0xDE;
sfr TBCDR1      = 0xDD;
sfr TBCDR2      = 0xDC;
sfr TBCDR3      = 0xDB;
sfr TBCINTSTS	= 0xDF;

//TBC MODULE REG DEFINE END

//WDT MODULE REG DEFINE START
sfr WDTCON		= 0xE4;
sfr WDTMOD		= 0xE5;
sfr WDTINTSTS   = 0xE6;
//WDT MODULE REG DEFINE END

//KBC MODULE REG DEFINE START
sfr KBCSTAT	    = 0xA0;
sfr KBCCR		= 0xA1;
sfr KBCDAT1		= 0xA2;
sfr KBCDAT2		= 0xA3;
//KBC MODULE REG DEFINE END

//SM3 MODULE REG DEFINE START
sfr SM3CON	    = 0xF4;
sfr SM3INT		= 0xF5;

#define ARAM_BASE       (0x0B80)
#define SM3_A_BASE 		(&XBYTE[ARAM_BASE])
#define SM3_MSG_BASE 	(&XBYTE[ARAM_BASE + 32])
#define SM3_T_BASE 		(&XBYTE[ARAM_BASE + 96])
#define SM3_VA_BASE 	SM3_MSG_BASE
//SM3 MODULE REG DEFINE END

//RNG MODULE REG DEFINE START
sfr RNGSTART    = 0xEC;
sfr RNGMODE		= 0xED;
sfr RNGINT		= 0xEE;
sfr RNGDATA		= 0xEF;
//RNG MODULE REG DEFINE END



//ANALOGCTRL MODULE REG DEFINE START
#define ANALOGCTRLCR        XBYTE[0x1400]
#define ANALOGCTRLADCCR     XBYTE[0x1401]
#define ANALOGCTRLIER       XBYTE[0x1402]
#define ANALOGCTRLSR1       XBYTE[0x1403]
#define ANALOGCTRLSR2       XBYTE[0x1404]
#define ANALOGCTRLSDLR      XBYTE[0x1405]
#define ANALOGCTRLSDHR      XBYTE[0x1406]
#define ANALOGCTRLTRIMR1    XBYTE[0x1407]
#define ANALOGCTRLTRIMR2    XBYTE[0x1408]

#define LCDOPT              XBYTE[0x1409]
#define LCDCR               XBYTE[0x1410]
#define LCDDSPMOD           XBYTE[0x1411]
//ANALOGCTRL MODULE REG DEFINE END

#endif