	.file	"test.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	4
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	5
	.globl	d
	.type	d, @object
	.size	d, 1
d:
	.byte	100
	.globl	e
	.align 8
	.type	e, @object
	.size	e, 8
e:
	.quad	6
	.globl	f
	.align 4
	.type	f, @object
	.size	f, 4
f:
	.long	1095342490
	.section	.rodata
.LC0:
	.string	"aaa %d, %d\n"
.LC1:
	.string	"bbb %d, %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$3, a(%rip)
	movl	$4, b(%rip)
	movl	b(%rip), %edx
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	b(%rip), %edx
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	b(%rip), %eax
	sall	$2, %eax
	movl	%eax, a(%rip)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	mainn
	.type	mainn, @function
mainn:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	mainn, .-mainn
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
