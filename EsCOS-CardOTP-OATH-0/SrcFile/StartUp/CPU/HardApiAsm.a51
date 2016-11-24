;HardApiAsm.a51
;V0.1
;$Lee 2008.04.01

PUBLIC 			vSaveCurrent
PUBLIC			vLoadCurrent 
	

DPL2		DATA	084H
DPH2		DATA	085H
DPS		    DATA	086H

?PR?_HardApiAsm		SEGMENT 	CODE
RSEG				?PR?_HardApiAsm


;**********************************ROM BANK SELECT********************************
vSaveCurrent:
	POP 01H
	POP 02H

	PUSH DPS
	PUSH DPH2
	PUSH DPL2
	MOV DPS,	#00H

	PUSH 02H
	PUSH 01H

	RET
	
;**********************************RAM BANK SELECT *******************************
vLoadCurrent:
	POP 01H
	POP 02H

	POP DPL2
	POP DPH2
	POP DPS

	PUSH 02H
	PUSH 01H

	RET


END
