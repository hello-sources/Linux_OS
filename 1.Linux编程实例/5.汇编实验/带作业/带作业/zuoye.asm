INCLUDE E:\Irvine\Irvine32.inc 
INCLUDE E:\Irvine\Macros.inc
INCLUDELIB E:\Irvine\Irvine32.lib

.data
data SBYTE -100,-65,-32,-50,-30,-60,-80,-120
i BYTE 0
max BYTE 80h
n BYTE 0
stra BYTE "The number of nagative number is:",0
strb BYTE "The max number in the data is :",0

.code
main PROC
mov eax,0
mov ebx,0
mov ecx,0
mov ecx,lengthof data
L1:
		mov i,cl;
		cmp data[eax],0
		jg M2
		inc ebx
		M2:
		inc eax
	loop L1
mov eax,0
mov ecx,lengthof data
L2:
		mov dl,data[eax]
		inc eax
		cmp dl,max
		jl M
		mov max,dl
		M:
	loop L2
	mov edx,offset stra
	call WriteString
	mov eax, ebx
	call writedec
	call crlf 
	mov edx,offset strb
	call WriteString
	mov al,max
	mov ebx,TYPE BYTE
	call writehexb

	exit
	main ENDP
	end main