a:
	.globl	a
	.type  a, @object
	.long	5
	.align   4
	.size	a, 4
b:
	.globl	b
	.type  b, @object
	.long	3
	.align   4
	.size	b, 4
i:
	.globl	i
	.type  i, @object
	.long	1
	.align   4
	.size	i, 4
k:
	.globl	k
	.type  k, @object
	.long	6
	.align   4
	.size	k, 4
__temp1__:
	.globl	___temp1__
	.type  ___temp1__, @object
	.long	0
	.align   4
	.size	___temp1__, 4
__temp2__:
	.globl	___temp2__
	.type  ___temp2__, @object
	.long	0
	.align   4
	.size	___temp2__, 4
__temp0__:
	.globl	___temp0__
	.type  ___temp0__, @object
	.long	0
	.align   4
	.size	___temp0__, 4
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
_4:
	.globl	_4
	.type  _4, @object
	.long	4
	.align   4
	.size	_4, 4
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
_7:
	.globl	_7
	.type  _7, @object
	.long	7
	.align   4
	.size	_7, 4
_8:
	.globl	_8
	.type  _8, @object
	.long	8
	.align   4
	.size	_8, 4

##TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

##TAC_LESSER
	movl	a(%rip), %edx
	movl	7(%rip), %eax
	cmpl	%eax, %edx
	jl  .L0
	movl $0, %eax
	jmp .L1
.L0:
	movl $1, %eax
.L1:
	movl %eax, __temp0__(%rip)

##TAC_GREATER
	movl	i(%rip), %edx
	movl	4(%rip), %eax
	cmpl	%eax, %edx
	jg .L2
	movl $0, %eax
	jmp .L3
.L2:
	movl $1, %eax
.L3:
	movl %eax, __temp1__(%rip)

##TAC_AND
	movl	__temp0__(%rip), %eax
	movl	__temp1__(%rip), %edx
	orl %eax, %edx
	jz .L4
	movl $1, %eax
	jmp .L5
.L4:
	movl $0, %eax
.L5:
	movl %eax, __temp2__(%rip)

##TAC_IFZ
	movl	 __temp2__(%rip), %eax
	movl $1, %edx
	andl %eax, %edx
	jz .__label_after_if_0__

##TAC_MOVE
	movl	5(%rip), %eax
	movl	%eax, a(%rip)
	movl $0, %eax

.__label_after_if_0__:

##TAC_MOVE
	movl	8(%rip), %eax
	movl	%eax, i(%rip)
	movl $0, %eax

##TAC_ENDFUN
	popq	%rbp
	ret
