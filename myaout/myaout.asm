; nasm -f aout myaout.asm 
; nasm -f aoutb myaout.asm 
; for supported nasm format nasm -hf
; see an a.out format
; http://stackoverflow.com/questions/8303536/generating-a-out-file-format-with-gcc
; http://docs.cs.up.ac.za/programming/asm/derick_tut/ 
section .text
	global _start
_start:
	mov eax, 1
	mov ebx, 0
	int 80h
	
