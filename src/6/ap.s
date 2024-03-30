	.file	"ap.c"
	.intel_syntax noprefix
	.text
	.globl	is_prime
	.type	is_prime, @function
is_prime:
.LFB0:
	.cfi_startproc
	endbr64
	cmp	edi, 1
	jbe	composite
	cmp	edi, 3
	jbe	prime
	test	dil, 1
	je	composite
	mov	ecx, 2
loop:
# ja if (l > r) goto; (СF == 0 && ZF == 0)
	mov	eax, ecx
	imul	eax, ecx
	cmp	eax, edi
	ja	prime

	mov	eax, edi
	mov	edx, 0
# div eax % ecx -> eax, reminder edx
	div	ecx
# jne if (l != r) goto: (ZF == 0)
	add	ecx, 1
	test	edx, edx
	jne loop
composite:
	mov	eax, 0
	ret
prime:
	mov	eax, 1
	ret
	.cfi_endproc
.LFE0:
	.size	is_prime, .-is_prime
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
