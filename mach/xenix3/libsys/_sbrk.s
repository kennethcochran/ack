.sect .text; .sect .rom; .sect .data; .sect .bss
.define __sbrk
.sect .text
__sbrk:
	push	bp
	mov	bp,sp
	push	ds
	mov	ax,4(bp)
	cwd
	push	dx
	push	ax
	mov	ax,1
	push	ax
	call	__brkctl
	add	sp,8
	cmp	ax,-1
	je	1f
1:
	mov	dx,4(bp)
	add	(.limhp),dx
	pop	bp
	ret


__brkctl:
	mov	ax,4904
	jmp	sysal
