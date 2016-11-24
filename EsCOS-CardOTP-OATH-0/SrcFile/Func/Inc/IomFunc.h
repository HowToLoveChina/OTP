#ifndef _IOMFUNC_H_
#define _IOMFUNC_H_
#include "REG_Z8D16R_2.h"
#define GP0  0
#define GP1  1
#define GP2  2
#define GP3  3
#define GP4  4
#define GP5  5
#define GP6  6
#define GP7  7

#define GP8   8
#define GP9   9
#define GP10  10
#define GP11  11

#define GP12   12
#define GP13   13
#define GP14   14
#define GP15   15



//REG : IOMCR
#define LSCLK                   (1<<0)
#define UARTEN                  (1<<1)	

#define vIom_UartEn()           {IOMCR|=UARTEN;}
#define vIom_UartDis()          {IOMCR&=(~UARTEN);}
#define vIom_LsclkOutEn()       {IOMCR|=LSCLK;}
#define vIom_LsclkOutDis()      {IOMCR&=(~LSCLK);}
//REG : IOMP0DIR
#define vIom_Gp0OutEn()			{IOMP0DIR|=0x01;}
#define vIom_Gp1OutEn()			{IOMP0DIR|=0x02;}
#define vIom_Gp2OutEn()			{IOMP0DIR|=0x04;}
#define vIom_Gp3OutEn()			{IOMP0DIR|=0x08;}
#define vIom_Gp4OutEn()			{IOMP0DIR|=0x10;}
#define vIom_Gp5OutEn()			{IOMP0DIR|=0x20;}
#define vIom_Gp6OutEn()			{IOMP0DIR|=0x40;}
#define vIom_Gp7OutEn()			{IOMP0DIR|=0x80;}

#define vIom_Gp0InEn()			{IOMP0DIR&=0xfe;}
#define vIom_Gp1InEn()			{IOMP0DIR&=0xfd;}
#define vIom_Gp2InEn()			{IOMP0DIR&=0xfb;}
#define vIom_Gp3InEn()			{IOMP0DIR&=0xf7;}
#define vIom_Gp4InEn()			{IOMP0DIR&=0xef;}
#define vIom_Gp5InEn()			{IOMP0DIR&=0xdf;}
#define vIom_Gp6InEn()			{IOMP0DIR&=0xbf;}
#define vIom_Gp7InEn()			{IOMP0DIR&=0x7f;}
//REG : IOMP0PU
#define vIom_Gp0PuEn()			{IOMP0PU|=0x01;}
#define vIom_Gp1PuEn()			{IOMP0PU|=0x02;}
#define vIom_Gp2PuEn()			{IOMP0PU|=0x04;}
#define vIom_Gp3PuEn()			{IOMP0PU|=0x08;}
#define vIom_Gp4PuEn()			{IOMP0PU|=0x10;}
#define vIom_Gp5PuEn()			{IOMP0PU|=0x20;}
#define vIom_Gp6PuEn()			{IOMP0PU|=0x40;}
#define vIom_Gp7PuEn()			{IOMP0PU|=0x80;}

#define vIom_Gp0PuDis()			{IOMP0PU&=0xfe;}
#define vIom_Gp1PuDis()			{IOMP0PU&=0xfd;}
#define vIom_Gp2PuDis()			{IOMP0PU&=0xfb;}
#define vIom_Gp3PuDis()			{IOMP0PU&=0xf7;}
#define vIom_Gp4PuDis()			{IOMP0PU&=0xef;}
#define vIom_Gp5PuDis()			{IOMP0PU&=0xdf;}
#define vIom_Gp6PuDis()			{IOMP0PU&=0xbf;}
#define vIom_Gp7PuDis()			{IOMP0PU&=0x7f;}
//REG : IOMP0PD
#define vIom_Gp0PdEn()			{IOMP0PD|=0x01;}
#define vIom_Gp1PdEn()			{IOMP0PD|=0x02;}
#define vIom_Gp2PdEn()			{IOMP0PD|=0x04;}
#define vIom_Gp3PdEn()			{IOMP0PD|=0x08;}
#define vIom_Gp4PdEn()			{IOMP0PD|=0x10;}
#define vIom_Gp5PdEn()			{IOMP0PD|=0x20;}
#define vIom_Gp6PdEn()			{IOMP0PD|=0x40;}
#define vIom_Gp7PdEn()			{IOMP0PD|=0x80;}

#define vIom_Gp0PdDis()			{IOMP0PD&=0xfe;}
#define vIom_Gp1PdDis()			{IOMP0PD&=0xfd;}
#define vIom_Gp2PdDis()			{IOMP0PD&=0xfb;}
#define vIom_Gp3PdDis()			{IOMP0PD&=0xf7;}
#define vIom_Gp4PdDis()			{IOMP0PD&=0xef;}
#define vIom_Gp5PdDis()			{IOMP0PD&=0xdf;}
#define vIom_Gp6PdDis()			{IOMP0PD&=0xbf;}
#define vIom_Gp7PdDis()			{IOMP0PD&=0x7f;}
//REG : IOMP0INTS1
#define vIom_Gp0DoubEdgeIntEn() {IOMP0INTS1|=0x03;}
#define vIom_Gp1DoubEdgeIntEn() {IOMP0INTS1|=0x0c;}
#define vIom_Gp2DoubEdgeIntEn() {IOMP0INTS1|=0x30;}
#define vIom_Gp3DoubEdgeIntEn() {IOMP0INTS1|=0xc0;}
//REG : IOMP0INTS2
#define vIom_Gp4DoubEdgeIntEn() {IOMP0INTS2|=0x03;}
#define vIom_Gp5DoubEdgeIntEn() {IOMP0INTS2|=0x0c;}
#define vIom_Gp6DoubEdgeIntEn() {IOMP0INTS2|=0x30;}
#define vIom_Gp7DoubEdgeIntEn() {IOMP0INTS2|=0xc0;}
//REG : IOMP0INTS1
#define vIom_Gp0RiseEdgeIntEn() {IOMP0INTS1&=0xfc;IOMP0INTS1|=0x01;}
#define vIom_Gp1RiseEdgeIntEn() {IOMP0INTS1&=0xf3;IOMP0INTS1|=0x04;}
#define vIom_Gp2RiseEdgeIntEn() {IOMP0INTS1&=0xcf;IOMP0INTS1|=0x10;}
#define vIom_Gp3RiseEdgeIntEn() {IOMP0INTS1&=0x3f;IOMP0INTS1|=0x40;}
//REG : IOMP0INTS2
#define vIom_Gp4RiseEdgeIntEn() {IOMP0INTS2&=0xfc;IOMP0INTS2|=0x01;}
#define vIom_Gp5RiseEdgeIntEn() {IOMP0INTS2&=0xf3;IOMP0INTS2|=0x04;}
#define vIom_Gp6RiseEdgeIntEn() {IOMP0INTS2&=0xcf;IOMP0INTS2|=0x10;}
#define vIom_Gp7RiseEdgeIntEn() {IOMP0INTS2&=0x3f;IOMP0INTS2|=0x40;}
//REG : IOMP0INTS1
#define vIom_Gp0FallEdgeIntEn() {IOMP0INTS1&=0xfc;IOMP0INTS1|=0x02;}
#define vIom_Gp1FallEdgeIntEn() {IOMP0INTS1&=0xf3;IOMP0INTS1|=0x08;}
#define vIom_Gp2FallEdgeIntEn() {IOMP0INTS1&=0xcf;IOMP0INTS1|=0x20;}
#define vIom_Gp3FallEdgeIntEn() {IOMP0INTS1&=0x3f;IOMP0INTS1|=0x80;}
//REG : IOMP0INTS2
#define vIom_Gp4FallEdgeIntEn() {IOMP0INTS2&=0xfc;IOMP0INTS2|=0x02;}
#define vIom_Gp5FallEdgeIntEn() {IOMP0INTS2&=0xf3;IOMP0INTS2|=0x08;}
#define vIom_Gp6FallEdgeIntEn() {IOMP0INTS2&=0xcf;IOMP0INTS2|=0x20;}
#define vIom_Gp7FallEdgeIntEn() {IOMP0INTS2&=0x3f;IOMP0INTS2|=0x80;}
//REG : IOMP0INTS1
#define vIom_Gp0IntDis()        {IOMP0INTS1&=0xfc;}
#define vIom_Gp1IntDis()        {IOMP0INTS1&=0xf3;}
#define vIom_Gp2IntDis()        {IOMP0INTS1&=0xcf;}
#define vIom_Gp3IntDis()        {IOMP0INTS1&=0x3f;}
//REG : IOMP0INTS2
#define vIom_Gp4IntDis()        {IOMP0INTS2&=0xfc;}
#define vIom_Gp5IntDis()        {IOMP0INTS2&=0xf3;}
#define vIom_Gp6IntDis()        {IOMP0INTS2&=0xcf;}
#define vIom_Gp7IntDis()        {IOMP0INTS2&=0x3f;}
//REG : IOMP1DIR
#define vIom_Gp8OutEn()			{IOMP1DIR|=0x01;}
#define vIom_Gp9OutEn()			{IOMP1DIR|=0x02;}
#define vIom_Gp10OutEn()			{IOMP1DIR|=0x04;}
#define vIom_Gp11OutEn()			{IOMP1DIR|=0x08;}
#define vIom_Gp12OutEn()			{IOMP1DIR|=0x10;}
#define vIom_Gp13OutEn()			{IOMP1DIR|=0x20;}
#define vIom_Gp14OutEn()			{IOMP1DIR|=0x40;}
#define vIom_Gp15OutEn()			{IOMP1DIR|=0x80;}

#define vIom_Gp8InEn()			{IOMP1DIR&=0xfe;}
#define vIom_Gp9InEn()			{IOMP1DIR&=0xfd;}
#define vIom_Gp10InEn()			{IOMP1DIR&=0xfb;}
#define vIom_Gp11InEn()			{IOMP1DIR&=0xf7;}
#define vIom_Gp12InEn()			{IOMP1DIR&=0xef;}
#define vIom_Gp13InEn()			{IOMP1DIR&=0xdf;}
#define vIom_Gp14InEn()			{IOMP1DIR&=0xbf;}
#define vIom_Gp15InEn()			{IOMP1DIR&=0x7f;}
//REG : IOMP1PU	 IOMP1PD
#define vIom_Gp8PuEn()			{IOMP1PU|=0x01;}
#define vIom_Gp9PuEn()			{IOMP1PU|=0x02;}
#define vIom_Gp10PuEn()			{IOMP1PU|=0x04;}
#define vIom_Gp11PuEn()			{IOMP1PU|=0x08;}
#define vIom_Gp12PuEn()			{IOMP1PU|=0x10;}
#define vIom_Gp13PuEn()			{IOMP1PU|=0x20;}
#define vIom_Gp14PuEn()			{IOMP1PU|=0x40;}
#define vIom_Gp15PuEn()			{IOMP1PU|=0x80;}

#define vIom_Gp8PuDis()			{IOMP1PU&=0xfe;}
#define vIom_Gp9PuDis()			{IOMP1PU&=0xfd;}
#define vIom_Gp10PuDis()			{IOMP1PU&=0xfb;}
#define vIom_Gp11PuDis()			{IOMP1PU&=0xf7;}
#define vIom_Gp12PuDis()			{IOMP1PU&=0xef;}
#define vIom_Gp13PuDis()			{IOMP1PU&=0xdf;}
#define vIom_Gp14PuDis()			{IOMP1PU&=0xbf;}
#define vIom_Gp15PuDis()			{IOMP1PU&=0x7f;}
//REG :  IOMP1PD
#define vIom_Gp8PdEn()			{IOMP0PD|=0x01;}
#define vIom_Gp9PdEn()			{IOMP0PD|=0x02;}
#define vIom_Gp10PdEn()			{IOMP0PD|=0x04;}
#define vIom_Gp11PdEn()			{IOMP0PD|=0x08;}
#define vIom_Gp12PdEn()			{IOMP0PD|=0x10;}
#define vIom_Gp13PdEn()			{IOMP0PD|=0x20;}
#define vIom_Gp14PdEn()			{IOMP0PD|=0x40;}
#define vIom_Gp15PdEn()			{IOMP0PD|=0x80;}

#define vIom_Gp8PdDis()			{IOMP0PD&=0xfe;}
#define vIom_Gp9PdDis()			{IOMP0PD&=0xfd;}
#define vIom_Gp10PdDis()			{IOMP0PD&=0xfb;}
#define vIom_Gp11PdDis()			{IOMP0PD&=0xf7;}
#define vIom_Gp12PdDis()			{IOMP0PD&=0xef;}
#define vIom_Gp13PdDis()			{IOMP0PD&=0xdf;}
#define vIom_Gp14PdDis()			{IOMP0PD&=0xbf;}
#define vIom_Gp15PdDis()			{IOMP0PD&=0x7f;} 


//REG : IOMP1INTS1
#define vIom_Gp8DoubEdgeIntEn() {IOMP1INTS1|=0x03;}
#define vIom_Gp9DoubEdgeIntEn() {IOMP1INTS1|=0x0c;}
#define vIom_Gp10DoubEdgeIntEn() {IOMP1INTS1|=0x30;}
#define vIom_Gp11DoubEdgeIntEn() {IOMP1INTS1|=0xc0;}
//REG : IOMP1INTS2
#define vIom_Gp12DoubEdgeIntEn() {IOMP1INTS2|=0x03;}
#define vIom_Gp13DoubEdgeIntEn() {IOMP1INTS2|=0x0c;}
#define vIom_Gp14DoubEdgeIntEn() {IOMP1INTS2|=0x30;}
#define vIom_Gp15DoubEdgeIntEn() {IOMP1INTS2|=0xc0;}
//REG : IOMP1INTS1
#define vIom_Gp8RiseEdgeIntEn() {IOMP1INTS1&=0xfc;IOMP1INTS1|=0x01;}
#define vIom_Gp9RiseEdgeIntEn() {IOMP1INTS1&=0xf3;IOMP1INTS1|=0x04;}
#define vIom_Gp10RiseEdgeIntEn() {IOMP1INTS1&=0xcf;IOMP1INTS1|=0x10;}
#define vIom_Gp11RiseEdgeIntEn() {IOMP1INTS1&=0x3f;IOMP1INTS1|=0x40;}
//REG : IOMP1INTS2
#define vIom_Gp12RiseEdgeIntEn() {IOMP1INTS2&=0xfc;IOMP1INTS2|=0x01;}
#define vIom_Gp13RiseEdgeIntEn() {IOMP1INTS2&=0xf3;IOMP1INTS2|=0x04;}
#define vIom_Gp14RiseEdgeIntEn() {IOMP1INTS2&=0xcf;IOMP1INTS2|=0x10;}
#define vIom_Gp15RiseEdgeIntEn() {IOMP1INTS2&=0x3f;IOMP1INTS2|=0x40;}
//REG : IOMP1INTS1
#define vIom_Gp8FallEdgeIntEn() {IOMP1INTS1&=0xfc;IOMP1INTS1|=0x02;}
#define vIom_Gp9FallEdgeIntEn() {IOMP1INTS1&=0xf3;IOMP1INTS1|=0x08;}
#define vIom_Gp10FallEdgeIntEn() {IOMP1INTS1&=0xcf;IOMP1INTS1|=0x20;}
#define vIom_Gp11FallEdgeIntEn() {IOMP1INTS1&=0x3f;IOMP1INTS1|=0x80;}
//REG : IOMP1INTS2
#define vIom_Gp12FallEdgeIntEn() {IOMP1INTS2&=0xfc;IOMP1INTS2|=0x02;}
#define vIom_Gp13FallEdgeIntEn() {IOMP1INTS2&=0xf3;IOMP1INTS2|=0x08;}
#define vIom_Gp14FallEdgeIntEn() {IOMP1INTS2&=0xcf;IOMP1INTS2|=0x20;}
#define vIom_Gp15FallEdgeIntEn() {IOMP1INTS2&=0x3f;IOMP1INTS2|=0x80;}
//REG : IOMP1INTS1
#define vIom_Gp8IntDis()        {IOMP1INTS1&=0xfc;}
#define vIom_Gp9IntDis()        {IOMP1INTS1&=0xf3;}
#define vIom_Gp10IntDis()        {IOMP1INTS1&=0xcf;}
#define vIom_Gp11IntDis()        {IOMP1INTS1&=0x3f;}
//REG : IOMP1INTS2
#define vIom_Gp12IntDis()        {IOMP1INTS2&=0xfc;}
#define vIom_Gp13IntDis()        {IOMP1INTS2&=0xf3;}
#define vIom_Gp14IntDis()        {IOMP1INTS2&=0xcf;}
#define vIom_Gp15IntDis()        {IOMP1INTS2&=0x3f;}


#ifdef 	_SCUFUNC_C_


void vIom_GpioConfig(UINT8 GPn,UINT8 InOut);
UINT8 uIom_GetGpioVal(UINT8 GPn);
void vIom_SetGpioVal(UINT8 GPn,UINT8 Val);
#else
extern void vIom_GpioConfig(UINT8 GPn,UINT8 InOut);
extern UINT8 uIom_GetGpioVal(UINT8 GPn);
extern void vIom_SetGpioVal(UINT8 GPn,UINT8 Val);

#endif
#endif