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
                            ;��prompt���ݴ浽r1
                            addi	 r1,r0,Prompt
							;��ת��input.s
                            jal	                  InputUnsigned
                            add                  r2,r0,r1    
                            add                  r8,r0,r1   
                            add                  r6,r0,r1   
                            add                  r7,r0,r1    
                            addi                 r3,r0,0   
                            
                            ;ѭ����������ĸ���ֵ
InputArray:         	    beqz                 r2, ProcessPart      ;���r2=0������ת��processpart 
                            addi                 r1,r0,PromptLast
                            jal                  InputUnsigned
                            sw                   PrintfValue(r3),r1    ;  
                            addi                 r3,r3,4     ;32λ���Σ���4B
                            subi                 r2,r2,1      
                            j                    InputArray
  
   
                           ;��ʼ���� �����ڴ���ȡ���֣�����R4��R5�У��Ƚ�֮�󣬻��ķ������ǶԵ�д��, ѭ�������������R6���ڲ�R7, ȡ�ڴ��ƫ������Ȼ��R3����
                          
                         
                           
ProcessPart:        addi                 r3,r0,0        
 OutLoop:              
                            beqz                 r6,End     ;r6Ϊ���ѭ��ʣ�������r6Ϊ0ʱ�˳�ѭ��
                            addi                  r3,r0,0
                            subi                  r7,r6,1   ;r7��Ϊ�ڲ�ѭ��ʣ�����            
                            add                   r6,r0,r7
 InnerLop:             
                            beqz                 r7, OutLoop
                            lf                       f10,PrintfValue(r3) ;��r3�е������浽f10�У�
                            addi                  r3,r3,4	
                            lf                       f11,PrintfValue(r3)
                            lef                     f11 ,f10	;���f11<=f10����fpsΪ1������Ϊ0
                            bfpt                  HuHuan  ;���fpsΪ1����ת
back:
                            sf                      PrintfValue(r3),f11
                            subi                  r3,r3,4
                            sf                      PrintfValue(r3),f10
                            addi                  r3,r3,4
                            subi                   r7,r7,1
                            j                         InnerLop
HuHuan:             
                          movi2fp              f1,r0   ;����r0һ���ֳ�������f1
                          
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