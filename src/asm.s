_a:
	.globl	_a
	.type  _a, @object
	.long	5
	.align   4
	.size	_a, 4
_b:
	.globl	_b
	.type  _b, @object
	.long	3
	.align   4
	.size	_b, 4
_i:
	.globl	_i
	.type  _i, @object
	.long	1
	.align   4
	.size	_i, 4
_k:
	.globl	_k
	.type  _k, @object
	.long	6
	.align   4
	.size	_k, 4
	.section	.rodata
.LC_int:
	.string	"%d"
_1:
	.globl	_1
	.type  _1, @object
	.long	1
	.align   4
	.size	_1, 4
_3:
	.globl	_3
	.type  _3, @object
	.long	3
	.align   4
	.size	_3, 4
_5:
	.globl	_5
	.type  _5, @object
	.long	5
	.align   4
	.size	_5, 4
_6:
	.globl	_6
	.type  _6, @object
	.long	6
	.align   4
	.size	_6, 4
	.section	.rodata
.LC0:
	.string	"\n"

##TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

##TAC_PRINT
	movl	_a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC_int(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

##TAC_PRINT
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT

##TAC_ENDFUN
	popq	%rbp
	ret
