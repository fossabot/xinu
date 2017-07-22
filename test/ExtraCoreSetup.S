.globl ExtraCoreSetup
ExtraCoreSetup:   
   mov sp, #0x4000                     ;@ Set the stack pointer for that mode
loop:
   ldr r0, =check
   bl kprintf
   b loop
   bx  lr                           ;@ Return


.section .data
check:
		.asciz	"Got to ExtraCoreSetup\r\n"
