<<<<<<< Updated upstream
_a:
	.globl	_a
	.type  _a, @object
	.long	1
	.align   4
	.size	_a, 4
_i:
	.globl	_i
	.type  _i, @object
	.long	2
	.align   4
	.size	_i, 4
_b:
	.globl	_b
	.type  _b, @object
	.long	3
	.align   4
	.size	_b, 4
_boo:
	.globl	_boo
	.type  _boo, @object
	.long	1
	.align   4
	.size	_boo, 4
_baite:
	.globl	_baite
	.type  _baite, @object
	.byte	120
	.size	_baite, 1
_lung:
	.globl	_lung
	.type  _lung, @object
	.quad	127
	.align   8
	.size	_lung, 8
_floating:
	.globl	_floating
	.type  _floating, @object
	.long	126
	.align   4
	.size	_floating, 4
_TRUE:
	.globl	_TRUE
	.type  _TRUE, @object
	.long	1
	.align   4
	.size	_TRUE, 4
_1:
	.globl	_1
	.type  _1, @object
	.long	1
	.align   4
	.size	_1, 4
_2:
	.globl	_2
	.type  _2, @object
	.long	2
	.align   4
	.size	_2, 4
_3:
	.globl	_3
	.type  _3, @object
	.long	3
	.align   4
	.size	_3, 4
	.section	.rodata
_LC0:
	.string	"oioioi"
_126:
	.globl	_126
	.type  _126, @object
	.long	126
	.align   4
	.size	_126, 4
_127:
	.globl	_127
	.type  _127, @object
	.long	127
	.align   4
	.size	_127, 4
=======
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
>>>>>>> Stashed changes

##TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
<<<<<<< Updated upstream

##TAC_ENDFUN
	popq	%rbp
	ret

##TAC_BEGINFUN
	.globl	mainn
	.type	mainn, @function
mainn:
	pushq	%rbp
	movq	%rsp, %rbp

=======

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
	movl %eax, __temp3__(%rip)

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
	movl %eax, __temp4__(%rip)

##TAC_AND
	movl	__temp3__(%rip), %eax
	movl	__temp4__(%rip), %edx
	orl %eax, %edx
	jz .L4
	movl $1, %eax
	jmp .L5
.L4:
	movl $0, %eax
.L5:
	movl %eax, __temp5__(%rip)

##TAC_IFZ
	movl	 __temp5__(%rip), %eax
	movl $1, %edx
	andl %eax, %edx
	jz .__if_label1__

##TAC_MOVE
	movl	5(%rip), %eax
	movl	%eax, a(%rip)
	movl $0, %eax

.__if_label1__:

##TAC_MOVE
	movl	8(%rip), %eax
	movl	%eax, i(%rip)
	movl $0, %eax

>>>>>>> Stashed changes
##TAC_ENDFUN
	popq	%rbp
	ret
