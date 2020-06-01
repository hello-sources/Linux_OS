.data
Prompt1: 	.asciiz 	"please enter the principal, interest rate percentage, and number of years" 
Prompt: 	.asciiz 	"input a positive integer : " 
PrintfFormat:	.asciiz 	"The sum of your principal and interest is : %g "
.align	 2
PrintfPar:	.word	 PrintfFormat
PrintfValue:	.space	 1024          

.text
.global	main

main:

                            addi     r2,r0,3  ;r2寄存器表示只能输入3个正整数（本金，利率百分比，年数）
                            addi     r3,r0,0
                         
InputLoop:
                            ;*** Read value from stdin into R1   
                     	    ;r2寄存器中的值为0则跳转向ProcessPart标识的地址
                            beqz     r2, ProcessPart       
                            addi     r1,r0,Prompt
                            jal      InputUnsigned
                            sw       PrintfValue(r3),r1      
                            addi     r3,r3,4     
                            subi     r2,r2,1      
                            ;无条件跳转向InputArray标识的指令地址
                            j        InputLoop

ProcessPart:                addi     r3,r0,0

calculate:                  ;计算行列式
                            lf       f1,PrintfValue(r3) ;将r3中的数保存到f1中，第一个数
                            cvti2d   f0,f1
                            addi     r3,r3,4
                            lf       f3,PrintfValue(r3) ;将r3中的数保存到f3中，第二个数  
                            cvti2d   f2,f3  
                            addi     r4,r0,100
                            movi2fp  f16,r4
                            cvti2d   f14,f16
                            divd     f6,f2,f14 
                            addi     r3,r3,4
                            lf       f8,PrintfValue(r3) ;将r3中的数保存到f8中，第三个数 
                            movfp2i  r5,f8
                              
Loop:                      
                            beqz     r5, Finish
                            multd    f8,f0,f6
                            addd     f0,f0,f8
                            subi     r5,r5,1
                            j        Loop

                            
                                                 
                       
                                                
                            
Finish:                    ;*** write result to stdout
                           sd	     PrintfValue,f0
		           addi	     r14,r0,PrintfPar
		           trap	     5
				
		           ;*** end
		           trap	     0