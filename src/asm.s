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

##TAC_BEGINFUN
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp

##TAC_ENDFUN
	popq	%rbp
	ret

##TAC_BEGINFUN
	.globl	mainn
	.type	mainn, @function
mainn:
	pushq	%rbp
	movq	%rsp, %rbp

##TAC_ENDFUN
	popq	%rbp
	ret
