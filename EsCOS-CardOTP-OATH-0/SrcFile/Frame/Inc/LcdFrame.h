#ifndef _LCDFRAME_H_
#define _LCDFRAME_H_
#include "LCDFUNC.H"
//#include "TokenApp.H"



#ifdef _LCDRAME_C_
UINT8 _LCD_FLAG_;

void vLcdVerify(void);
void vTimer_Counter_Disp(UINT32 Time);
void vFloat_Disp(float Count);

#else

extern UINT8 _LCD_FLAG_;

extern void vLcdVerify(void);
extern void vTimer_Counter_Disp(UINT32 Time);
extern void vFloat_Disp(float Count);


#endif	
#endif