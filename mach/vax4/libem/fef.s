#include "em_abs.h"
.sect .text; .sect .rom; .sect .data; .sect .bss; .sect .text
.define	.fef

        ! $Header$


.fef:
	movl	(sp)+,r3
	cmpl	r0,$4
	bneq	I1
	cvtfd	(sp)+,r0
	jsb	Ihulp
	cvtdf	r0,-(sp)
	jmp	(r3)
I1:
	cmpl	r0,$8
	bneq	I2
	movd	(sp)+,r0
	jsb	Ihulp
	movd	r0,-(sp)
	jmp	(r3)
I2:
	pushl	$EILLINS
	jmp	.fat
Ihulp:
	movl	(sp)+,r2
	tstd	r0
	bneq	1f
	clrl	-(sp)
	jmp	(r2)
1:
	extzv	$7,$8,r0,-(sp)
	subl2	$128,(sp)
	insv	$128,$7,$8,r0
	jmp	(r2)
