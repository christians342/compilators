a:
	.globl	a
	.long	4
	.align 4
	.type  a, @object
	.size	a, 4
i:
	.globl	i
	.long	4
	.align 4
	.type  i, @object
	.size	i, 4
b:
	.globl	b
	.long	4
	.align 4
	.type  b, @object
	.size	b, 4

##TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	popq	%rbp
	ret

##TAC_GREATER
	movl	i(%rip), %edx
	movl	100(%rip), %eax
	cmpl	%eax, %edx
	jle .L0

##TAC_LESSER
	movl	i(%rip), %edx
	movl	100(%rip), %eax
	cmpl	%eax, %edx
	jge .L0

##TAC_OR
	movl	__temp3__(%rip), %eax
	testl %eax, %eax
	jne .L0
	movl	__temp4__(%rip), %eax
	testl %eax, %eax
	je .L1

##TAC_MOVE
	movl	50(%rip), %eax
	movl	%eax, a(%rip)

##TAC_MOVE
	movl	30(%rip), %eax
	movl	%eax, b(%rip)

.L0:

##TAC_MOVE
	movl	88(%rip), %eax
	movl	%eax, i(%rip)
