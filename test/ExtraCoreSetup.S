.globl Core1Setup
Core1Setup:  
	mov sp, #4000
/*	ldr r0, =checkcore1
	bl kprintf*/
	b createnullthread

.globl Core2Setup
Core2Setup:
	mov sp, #5000
loopcore2:
	ldr r0, =checkcore2
	bl kprintf
	b createnullthread

.globl Core3Setup
Core3Setup:
	mov sp, #6000
loopcore3:
	ldr r0, =checkcore3
	bl kprintf
	b createnullthread


.section .data
checkcore1:
		.asciz	"1111111111111111111\r\n"
checkcore2:
		.asciz	"2222222222222222222\r\n"
checkcore3:
		.asciz	"3333333333333333333\r\n"
