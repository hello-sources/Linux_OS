.data
Prompt: 	.asciiz 	"input An integer which is array's size value >1 : "
PromptLast:   .asciiz       "input an integer :"
PrintfFormat:	.asciiz 	"Number : %g "
.align	 2
PrintfPar:	.word	 PrintfFormat
Printf:      .space       8

PrintfValue:	.space	 1024
             

.text
.global	main


main:
                            ;把prompt内容存到r1
                            addi	 r1,r0,Prompt
							;跳转到input.s
                            jal	                  InputUnsigned
                            add                  r2,r0,r1    
                            add                  r8,r0,r1   
                            add                  r6,r0,r1   
                            add                  r7,r0,r1    
                            addi                 r3,r0,0   
                            
                            ;循环输入数组的各个值
InputArray:         	    beqz                 r2, ProcessPart      ;如果r2=0，则跳转到processpart 
                            addi                 r1,r0,PromptLast
                            jal                  InputUnsigned
                            sw                   PrintfValue(r3),r1    ;  
                            addi                 r3,r3,4     ;32位整形，即4B
                            subi                 r2,r2,1      
                            j                    InputArray
  
   
                           ;开始排序 ，从内存中取数字，放在R4和R5中，比较之后，换的方法就是对调写回, 循环的限制数外层R6，内层R7, 取内存的偏移量仍然用R3来存
                          
                         
                           
ProcessPart:        addi                 r3,r0,0        
 OutLoop:              
                            beqz                 r6,End     ;r6为外层循环剩余次数，r6为0时退出循环
                            addi                  r3,r0,0
                            subi                  r7,r6,1   ;r7作为内层循环剩余次数            
                            add                   r6,r0,r7
 InnerLop:             
                            beqz                 r7, OutLoop
                            lf                       f10,PrintfValue(r3) ;将r3中的数保存到f10中，
                            addi                  r3,r3,4	
                            lf                       f11,PrintfValue(r3)
                            lef                     f11 ,f10	;如果f11<=f10，则fps为1，否则为0
                            bfpt                  HuHuan  ;如果fps为1则跳转
back:
                            sf                      PrintfValue(r3),f11
                            subi                  r3,r3,4
                            sf                      PrintfValue(r3),f10
                            addi                  r3,r3,4
                            subi                   r7,r7,1
                            j                         InnerLop
HuHuan:             
                          movi2fp              f1,r0   ;拷贝r0一个字长的数到f1
                          
                          addf                    f2,f10,f1
                          addf                    f10,f1,f11
                          addf                    f11,f1,f2
                          j                           back 
                          
End:	        addi                     r3,r0,0 
output:              subi                     r8,r8,1
                             lf                        f2,   PrintfValue(r3)
                            cvti2d                 f0,f2
                            sd	                     Printf,f0
                            addi	    r14,r0,PrintfPar
                            trap	    5
                            addi                     r3,r3,4
                            beqz                    r8,over

                            j                           output
                            
                            



over:	           trap	 0	