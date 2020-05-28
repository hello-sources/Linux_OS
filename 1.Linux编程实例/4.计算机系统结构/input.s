;*********** WINDLX Ex.1: Read a positive integer number *************
;*********** (c) 1991 G黱ther Raidl			 *************
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
		sw		SaveR2,r2;保存寄存器内容，子程序结束后还原寄存器内容
		sw		SaveR3,r3
		sw		SaveR4,r4
		sw		SaveR5,r5

		;*** 输出Prompt
		sw		PrintfPar,r1;把r1内容保存到printfpar中
		addi		r14,r0,PrintfPar  ;printfpar放到r14中
		trap		5   ;输出r14内容

		;*** call Trap-3 to read line读取的是字符串，字符串为1B
		addi		r14,r0,ReadPar
		trap		3

		;*** determine value
		addi		r2,r0,ReadBuffer	;将键入的内容保存到r2指向的位置，也就是readbuffer中
		addi		r1,r0,0
		addi		r4,r0,10	;r4值位10，计算用，10也是回车的ascii码

Loop:		;*** reads digits to end of line
		lbu		r3,0(r2);输入数据转32位2进制，放到r3中，比如r2中为5，那么r3中将保存5的ascii码的二进制
		seqi		r5,r3,10	;判断r3是否等于回车，若等于则浮点状态位fps为1
		bnez		r5,Finish	;若fps=1，则跳转到finish
		subi		r3,r3,48	;r3减去48，将ascii码转换为10进制
		
		multu		r1,r1,r4	;Shift decimal
		add		r1,r1,r3
		addi		r2,r2,1 	;increment pointer
		j		Loop
		
Finish: 	;*** restore old register contents
		lw		r2,SaveR2
		lw		r3,SaveR3
		lw		r4,SaveR4
		lw		r5,SaveR5
		jr		r31		; Return，因为r31自动保存着原来的代码位置