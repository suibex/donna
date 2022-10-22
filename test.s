.globl _start
.align 2
_start:
	
	mov x0,#40
	mov x30,#20
	sub x0,x30,#2
	add x0,x30,#1
	
	brk #0x1
