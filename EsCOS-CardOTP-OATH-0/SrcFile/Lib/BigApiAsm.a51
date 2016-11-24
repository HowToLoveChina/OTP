;$INCLUDE  		(BigApiAsm.h)

PUBLIC  _move_xdata_buffer
PUBLIC  _clear_xdata_buffer

sfr	AUXR1		=		0x86;
sfr	DPL2		=		0x84;
sfr	DPH2		=		0x85;

?PR?_BigApiAsm		SEGMENT 	CODE
RSEG				?PR?_BigApiAsm

; dst <--- src; 
; void move_xdata_buffer(char *dst, char *src, unsigned char len)
; dst(R6,R7), src(R4,R5),len(R3)
; NOTE: 1,len could be 0-255, 0 represents 256 bytes
; 	2,dst,src must not be the same buffer because if it is, INC DPTR once per iteration is enough.
_move_xdata_buffer: ;12 clk/byte
	MOV	DPH, R4
	MOV	DPL, R5
    MOV	DPH2, R6
	MOV	DPL2, R7
	CJNE R3, #0,	move_xdata_buffer_1
	ret
move_xdata_buffer_1:        
    MOVX A,@DPTR
	INC DPTR
    INC AUXR1 
    MOVX @DPTR,A
	INC DPTR
    INC AUXR1
    DJNZ R3, move_xdata_buffer_1            
    ret

; dst <--- 0; 
; void clear_xdata_buffer(char *buf, unsigned char len)
; buf(R6,R7), len(R5)
; NOTE: len could be 0-255, 0 represents 256 bytes
_clear_xdata_buffer: ;12 clk/byte
	MOV	DPH, R6
	MOV	DPL, R7
	CLR     A
	CJNE R5, #0,	clear_xdata_buffer_1
	ret
clear_xdata_buffer_1:        
        MOVX 	@DPTR,A
	INC 	DPTR     
        DJNZ 	R5, clear_xdata_buffer_1    
        ret

END


