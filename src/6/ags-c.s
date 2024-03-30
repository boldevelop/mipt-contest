	.file	"ags.c"
	.intel_syntax noprefix
	.text
	.globl	caller
	.type	caller, @function
caller:
.LFB0:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	DWORD PTR -24[rbp], esi
	mov	DWORD PTR -28[rbp], edx
	mov	DWORD PTR -8[rbp], 1
	cmp	DWORD PTR -24[rbp], 0
	jne	.L2
	mov	eax, DWORD PTR -8[rbp]
	jmp	.L3
.L2:
	mov	eax, DWORD PTR -20[rbp]
	mov	edx, 0
	div	DWORD PTR -28[rbp]
	mov	DWORD PTR -4[rbp], edx
	mov	DWORD PTR -8[rbp], 1
	jmp	.L4
.L12:
	nop
.L5:
	mov	edx, DWORD PTR -28[rbp]
	mov	ecx, DWORD PTR -4[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, ecx
	mov	edi, eax
	call	callee@PLT
	mov	DWORD PTR -4[rbp], eax
	shr	DWORD PTR -24[rbp]
.L6:
	cmp	DWORD PTR -24[rbp], 0
	je	.L11
	nop
.L4:
	mov	eax, DWORD PTR -24[rbp]
	and	eax, 1
	test	eax, eax
	jne	.L12
	mov	edx, DWORD PTR -28[rbp]
	mov	ecx, DWORD PTR -4[rbp]
	mov	eax, DWORD PTR -8[rbp]
	mov	esi, ecx
	mov	edi, eax
	call	callee@PLT
	mov	DWORD PTR -8[rbp], eax
	sub	DWORD PTR -24[rbp], 1
	jmp	.L6
.L11:
	nop
.L7:
	mov	eax, DWORD PTR -8[rbp]
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	caller, .-caller
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
