global eager_asm
global  lazy_asm

section .text
eager_asm:
	or rdi, rsi
	sete dil
	movzx rax, dil
	ret

lazy_asm:
	xor rax, rax
	test dil, dil
	sete al
	test sil, sil
	sete sil
	movzx rsi, sil
	and rax, rax
	ret
