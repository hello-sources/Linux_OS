.data
Prompt: 	.asciiz 	"Please input three elements : "
PromptElements:   .asciiz       "Please input the elements:"
PromptResult:	.asciiz		"Result = %g "
.align	 2
PrintfPar:	.word	 PromptResult
;Printf:      .space       8
PrintfValue:	.space	 1024
             
.text
.global	main

main:
                            ;把prompt内容存到r1，即对应循环输入的参数个数
                            addi	 r1,r0,Prompt
							;跳转到input.s
                            jal	                  InputUnsigned
                            add                  r2,r0,r1    
                            addi                 r3,r0,0   
                            
                            ;循环输入数组的各个值
							;判断是否等于0
InputArray:         	    beqz                 r2, ProcessPart      ;如果r2=0，则跳转到processpart 
                            addi                 r1,r0,PromptElements
                            jal                  InputUnsigned
                            sw                   PrintfValue(r3),r1    ;  
                            addi                 r3,r3,4     ;32位整形，即4B
                            subi                 r2,r2,1      
                            j                    InputArray
                                                 
ProcessPart:        		addi                 r3,r0,0    
							
calculate:                	;下面进行的就是主要的圆台体积求解
                            lf       f1,PrintfValue(r3) ;将r3中的数保存到f1中,第一个参数
                            cvti2d   f0,f1
                            addi     r3,r3,4
                            lf       f3,PrintfValue(r3)	;将r3中的数保存到f1中,第二个参数
                            cvti2d   f2,f3
                            addi     r3,r3,4	
                            lf       f5,PrintfValue(r3) ;将r3中的数保存到f1中,第三个参数
                            cvti2d   f4,f5
                            addi     r3,r3,4	
                            lf       f7,PrintfValue(r3) ;将r3中的数保存到f1中,第四个参数
                            cvti2d   f6,f7
							
							
							;分别存入两个立即数314和100，用来计算并且表示pi
							addi				 r7, r0, 314 ;立即数314存到r7中
							addi				 r8, r0, 100 ;立即数100存入到r8中
							movi2fp 			 f16, r7
							cvti2d				 f18, f16 ;单精度数314转换为双精度数存到f18中
							movi2fp				 f16, r8
							cvti2d				 f20, f16 ;单精度数100转换为双精度数存到f20中
							divd				 f18, f18, f20 ;两个对应双精度数相除就是对应的pi
							
							multd 				 f8, f2, f0 ; f0 存储的是圆台上底的半径 
							multd				 f10, f0, f0 ; f2 存储的是下底的半径
							multd				 f12, f2, f2
							multd 				 f4, f18, f4 ; f4 存储的是圆台的高
							addd 				 f0, f12, f10
							addd 				 f12, f0, f8 ; 对应的就是r1 * r1 + r2 * r2 + r1 * r2
							multd				 f14, f12, f4
							divd				 f0, f14, f6 ; 第三个参数就是三，求解对应圆台体积
							
End:						; 输出对应f0中的结果
							sd 					 PrintfValue, f0
							addi 				 r14, r0, PrintfPar 
							trap				 5
							trap	 			 0	

