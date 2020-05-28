;*********** WINDLX Ex.1: Read a positive integer number *************
;*********** (c) 1991 G�nther Raidl			 *************
;*********** Modified 1992 Maziar Khosravipour		 *************

;-----------------------------------------------------------------------------
;Subprogram call by symbol "InputUnsigned"
;expect the address of a zero-terminated prompt string in R1
;returns the read value in R1
;changes the contents of registers R1,R13,R14
;-----------------------------------------------------------------------------

		.data

		;*** Data for Read-Trap
ReadBuffer:	.space		80
ReadPar:	.word		0,ReadBuffer,80

		;*** Data for Printf-Trap
PrintfPar:	.space		4

SaveR2:		.space		4
SaveR3:		.space		4
SaveR4:		.space		4
SaveR5:		.space		4


		.text

		.global		InputUnsigned
InputUnsigned:	
		sw		SaveR2,r2;����Ĵ������ݣ��ӳ��������ԭ�Ĵ�������
		sw		SaveR3,r3
		sw		SaveR4,r4
		sw		SaveR5,r5

		;*** ���Prompt
		sw		PrintfPar,r1;��r1���ݱ��浽printfpar��
		addi		r14,r0,PrintfPar  ;printfpar�ŵ�r14��
		trap		5   ;���r14����

		;*** call Trap-3 to read line��ȡ�����ַ������ַ���Ϊ1B
		addi		r14,r0,ReadPar
		trap		3

		;*** determine value
		addi		r2,r0,ReadBuffer	;����������ݱ��浽r2ָ���λ�ã�Ҳ����readbuffer��
		addi		r1,r0,0
		addi		r4,r0,10	;r4ֵλ10�������ã�10Ҳ�ǻس���ascii��

Loop:		;*** reads digits to end of line
		lbu		r3,0(r2);��������ת32λ2���ƣ��ŵ�r3�У�����r2��Ϊ5����ôr3�н�����5��ascii��Ķ�����
		seqi		r5,r3,10	;�ж�r3�Ƿ���ڻس����������򸡵�״̬λfpsΪ1
		bnez		r5,Finish	;��fps=1������ת��finish
		subi		r3,r3,48	;r3��ȥ48����ascii��ת��Ϊ10����
		
		multu		r1,r1,r4	;Shift decimal
		add		r1,r1,r3
		addi		r2,r2,1 	;increment pointer
		j		Loop
		
Finish: 	;*** restore old register contents
		lw		r2,SaveR2
		lw		r3,SaveR3
		lw		r4,SaveR4
		lw		r5,SaveR5
		jr		r31		; Return����Ϊr31�Զ�������ԭ���Ĵ���λ��