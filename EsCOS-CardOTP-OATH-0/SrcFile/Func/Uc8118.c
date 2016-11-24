 /********************************************************************
File: Uc8118.c
History:
Version                      Author                     Date
V0.1                         Colin		              2016-11-11

Description:
     Uc8118 Drive Source Code File;

 *******************************************************************/
#include "Uc8118.h"
#include "REG_Z8D16R_2.h"
#include "string.H"
#include <intrins.h>


/***********************************************************
*********************°×µ×ºÚ×Övcom-waveform******************
***********************************************************/
code unsigned int LUT_T1_VCOM[]= {0x48,0x8D,0x01,0x8D,0x90,0x01,0x48,0x90,0x01,0x46,0x46,0x01,0x4A,0x54,0x01};
code unsigned int LUT_T1_BLACK[]={0x88,0x8D,0x01,0x8D,0x90,0x01,0x88,0x50,0x01,0x46,0x46,0x01,0x4A,0x54,0x01};
code unsigned int LUT_T1_WHITE[]={0x48,0x4D,0x01,0x4D,0x50,0x01,0x48,0x90,0x01,0x86,0x86,0x01,0x8A,0x94,0x01};
/***********************************************************
*********************ºÚµ×°××Övcom-waveform******************
***********************************************************/
code unsigned int LUT_T1_V[]={  0x50, 0x90, 0x01, 0x50, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
code unsigned int LUT_T1_K[]={  0x90, 0x50, 0x01, 0x50, 0x50, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
code unsigned int LUT_T1_W[]={  0x90, 0x50, 0x01, 0x90, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

code UINT8 Paper_Num_Assic[10] = {'0','1','2','3','4','5','6','7','8','9'};

UINT8 Pixel_Set[96];
UINT8 Send_Num[12];
code UINT8 Pixel_Addr[9][7] = 				// SEG address
{
	{82},						//DOT
	{27,26,65,64,63,29,28},		// P1
	{23,22,68,67,66,25,24},		// P2
	{19,18,71,70,69,21,20},		// p3
	{15,14,74,73,72,17,16},		// p4
	{11,10,77,76,75,13,12},		// p5
	{8,7,81,80,79,78,9},		// p6
	{5,4,86,85,84,83,6},		// p7
	{2,1,90,89,88,87,3}			// p8
};

code UINT8 Num_Seg_Set[10][7] = 	// Number of seg set
{
 	{1,1,1,1,1,0,1},		// 0
 	{0,1,1,0,0,0,0},		// 1
 	{1,1,0,1,1,1,0},		// 2
 	{1,1,1,1,0,1,0},		// 3
 	{0,1,1,0,0,1,1},		// 4
 	{1,0,1,1,0,1,1},		// 5
 	{1,0,1,1,1,1,1},		// 6
 	{1,1,1,0,0,0,0},		// 7
 	{1,1,1,1,1,1,1},		// 8
 	{1,1,1,1,0,1,1},		// 9
};


void driver_delay_xms(UINT8 xms)
{
	UINT16 i ;
	
	for(; xms>0; xms--)
	{
		for(i=0; i<78; i++);
	}
}
/***********************************************************
*************************IC²ÎÊýÉè¶¨*************************
***********************************************************/
void ElcPaper_Init(void)
{
  	SPI_Init();
	EPD_W21_Init();
	UC8118_SETTING();
}
void SPI_Init(void)
{
	vScu_SetSysClkOsc2M();
		
	vScu_IomClkEn();

	
	EPD_SPI_SHD_N_OUT_EN();
	EPD_SPI_RST_N_OUT_EN();
	EPD_SPI_SDA_OUT_EN();
	EPD_SPI_CSB_OUT_EN();
	EPD_SPI_SCL_OUT_EN();

	EPD_SPI_BUSY_IN_EN();
	//EPD_SPI_SCL_IN_EN();

	EPD_SPI_SHD_N_PU_EN();
	EPD_SPI_RST_N_PU_EN();
	EPD_SPI_SDA_PU_EN();
	EPD_SPI_SCL_PU_EN();
	EPD_SPI_CSB_PU_EN();
	
	
}

void UC8118_SETTING(void)
{
	EPD_W21_Write_CMD_DATA(0,0x04);  //c
	driver_delay_xms(50);		
	EPD_W21_Write_CMD_DATA(0,0x00);  //c
	driver_delay_xms(10);		
	EPD_W21_Write_CMD_DATA(1,0x8F);  //d
	driver_delay_xms(10);					
	EPD_W21_Write_CMD_DATA(0,0x30);	//c  PLL Control 
	driver_delay_xms(10);
	EPD_W21_Write_CMD_DATA(1,0x03);	//d frame rate =50
	driver_delay_xms(10);
	LUT_spi_write(0);
}

void SPI_Delay(unsigned long xrate)
{
    int i,j;
    for(i=0;i<=xrate;i++)
      for(j=0;j<=1;j++);
}

void EPD_W21_Init(void)
{
	EPD_SPI_SHD_N_HIGH();
	EPD_SPI_CSB_HIGH();
	SPI_Delay(10);
	EPD_SPI_RST_N_LOW();
	SPI_Delay(100);
	EPD_SPI_RST_N_HIGH();
	SPI_Delay(10);
}

void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

	EPD_SPI_SCL_HIGH();
//	SPI_Delay(2);	
	EPD_SPI_SCL_LOW();
//	SPI_Delay(2);

    for(i=0; i<8; i++)   
    {
        if(value & 0x80)
        {
			EPD_SPI_SDA_HIGH();
        }
		else
		{
        	EPD_SPI_SDA_LOW();
		}
//		SPI_Delay(2);
//		EPD_SPI_SCL_OUT_EN();
        EPD_SPI_SCL_HIGH();		
        value <<= 1; 		 
//        SPI_Delay(2);
		EPD_SPI_SCL_LOW();
//		SPI_Delay(2);
    }
}

void EPD_W21_Write_CMD_DATA(int dc,unsigned char command)
{
    EPD_SPI_CSB_HIGH();
//	SPI_Delay(2);
	EPD_SPI_CSB_LOW();
//	SPI_Delay(2);
	if(dc) 
	{
	EPD_SPI_SDA_HIGH();
	}
	else 
	{
		EPD_SPI_SDA_LOW();
	}	                  
	SPI_Write(command);
//	SPI_Delay(2);
	EPD_SPI_CSB_HIGH();
}

/***********************************************************
****************IC poweroff and go to sleep*****************
***********************************************************/
void poweroff_sleep(void)
{
EPD_W21_Write_CMD_DATA(0,0x02);
EPD_W21_Write_CMD_DATA(1,0x03);
}

/***********************************************************
*************************updata*****************************
***********************************************************/
void display(void)
{
EPD_W21_Write_CMD_DATA(0,0x11);
EPD_W21_Write_CMD_DATA(0,0x12);

}

/***********************************************************
***************************´«Êäwaveform*********************
***********************************************************/
void LUT_spi_write(unsigned char k)
{
	if(k==0)
	{
	LUT_spi_write_wb_bw();
	}
	else
	{LUT_spi_write_bb_ww();}
}

/***********************************************************
*******************´«ÊäºÚµ×°××Öwaveform*********************
***********************************************************/
void LUT_spi_write_bb_ww(void)
{
	LUT_spi_write_bb_ww_vcom();
	driver_delay_xms(2);
	LUT_spi_write_bb_ww_b();
	driver_delay_xms(2);
	LUT_spi_write_bb_ww_w();
	driver_delay_xms(2);
}
/***********************************************************
*******************´«ÊäºÚµ×°××Övcom_waveform****************
***********************************************************/
void LUT_spi_write_bb_ww_vcom(void)
{
	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x20);
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_V[i]);
	}
	driver_delay_xms(10);
}
/***********************************************************
******************´«ÊäºÚµ×°××Öb-waveformwaveform************
***********************************************************/
void LUT_spi_write_bb_ww_b(void)
{

	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x21);
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_K[i]);
	}
	driver_delay_xms(10);
}
/***********************************************************
*******************´«ÊäºÚµ×°××Öw-waveform*******************
***********************************************************/
void LUT_spi_write_bb_ww_w(void)
{
	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x22);
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_W[i]);
	}
	driver_delay_xms(10);
}

/***********************************************************
*******************´«Êä°×µ×ºÚ×Öwaveform*********************
***********************************************************/
void LUT_spi_write_wb_bw(void)
{
	LUT_spi_write_wb_bw_vcom();
	driver_delay_xms(2);
	LUT_spi_write_wb_bw_b();
	driver_delay_xms(2);
	LUT_spi_write_wb_bw_w();
	driver_delay_xms(2);
}
/***********************************************************
*****************´«Êä°×µ×ºÚ×Övcom-waveform******************
***********************************************************/
void LUT_spi_write_wb_bw_vcom(void)
{
	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x20);//c
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_VCOM[i]);//d
	}
	driver_delay_xms(2);
}
/***********************************************************
*****************´«Êä°×µ×ºÚ×Öb-waveform*********************
***********************************************************/
void LUT_spi_write_wb_bw_b(void)
{
	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x21);
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_BLACK[i]);

	}
	driver_delay_xms(2);
}
/***********************************************************
*****************´«Êä°×µ×ºÚ×Öw-waveform*********************
***********************************************************/
void LUT_spi_write_wb_bw_w(void)
{
	unsigned long i;
	EPD_W21_Write_CMD_DATA(0,0x22);
	for(i=0;i<15;i++)
	{
	EPD_W21_Write_CMD_DATA(1,LUT_T1_WHITE[i]);
	//	EPD_W21_Write_CMD_DATA(1,0);
	}
	driver_delay_xms(2);
}


void vPaper_Bcd_Display(UINT8 position,UINT8 num)
{
	UINT8 i;


	
	for(i=0;i<7;i++)
	  {
		*(Pixel_Set+Pixel_Addr[position][i]-1)=!Num_Seg_Set[num][i];
	  }
}


void vPaper_String_Display(UINT8 cString[],UINT8 cLen)			
{
	UINT8 i,j;
	UINT8 m;
	vScu_SetSysClkOsc2M();	
			
	memset(Pixel_Set,1,96);
	if(cLen>9||(cLen==0))
	{
		return;
	}

	
	for(i=0;i<cLen;i++)
	{
		for(j=0;j<10;j++)				
		{	
			if(cString[i] ==  Paper_Num_Assic[j])
			{
				vPaper_Bcd_Display(9-cLen+i,j);
				continue;
			}
		}
	}

	
	for(m=0;m<12;m++)
	{
		Send_Num[m]=(Pixel_Set[m*8]<<7)+(Pixel_Set[m*8+1]<<6)+(Pixel_Set[m*8+2]<<5)+(Pixel_Set[m*8+3]<<4) \
					 +(Pixel_Set[m*8+4]<<3)+(Pixel_Set[m*8+5]<<2)+(Pixel_Set[m*8+6]<<1)+(Pixel_Set[m*8+7]);
	}

   	EPD_W21_Write_CMD_DATA(0,0x10);	
   for(j=0; j<12; j++)	
	{
	 EPD_W21_Write_CMD_DATA(1,Send_Num[j]);
	}				
	EPD_W21_Write_CMD_DATA(1,0xFF);

	display();
	 while(EPD_SPI_BUSY==0);
	vScu_SetSysClk32K();
	return;
}
void Paper_All_Clear(void)	
{
	UINT8 j;
		vScu_SetSysClkOsc2M();
	EPD_W21_Write_CMD_DATA(0,0x10);		

	for(j=0; j<12; j++)	
	{
	 //EPD_W21_Write_CMD_DATA(1,Send_Num[j]);
	   EPD_W21_Write_CMD_DATA(1,0xFF);
	}				
	EPD_W21_Write_CMD_DATA(1,0xFF);

	display();
	 while(EPD_SPI_BUSY==0);
   vScu_SetSysClk32K();

}

