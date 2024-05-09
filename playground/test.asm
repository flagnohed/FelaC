
global _start
_start:
	mov rax, 60   ; exit syscall number
	mov rdi, 69   ; put error_code in rdi
	syscall
	 


