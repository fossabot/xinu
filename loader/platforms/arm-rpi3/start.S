/**
 * @file start.S
 *
 * Initialization code for Embedded Xinu on the Raspberry Pi.
 *
 * Embedded Xinu, Copyright (C) 2013.  All rights reserved.
 *
 * -----------------------------------------------------------------------------------------
 * 
 * Specific for Raspberry Pi 3 (BCM2837) - Memory Layout (Not to scale)
 * 1GB OF RAM (PHYSICAL MEMORY) AVAILABLE. 1GB == 0x3FFFFFFF
 * +----------+
 * |	      |
 * | IO SPACE | -> 0x3F000000 to 0x3FFFFFFF reserved for IO (GPIO, UART, SYS TIMER, USB CORE)
 * |          |
 * +----------+
 * |          |
 * |          |
 * |   HEAP   | -> getmem allocates from here
 * |          |
 * |          |
 * +----------+
 * | OS STACK | -> becomes the null process' stack
 * +----------+
 * |   BSS    | -> needed for C environment
 * +----------+
 * |   TEXT   | -> XINU code
 * |          |
 * +----------+
 * | RESERVED | -> interrupt handler and vectors go here
 * +----------+
 */

#include <arm64.h>     /* For ARM_MODE_SYS */

#define IO_BASE 0x3f000000
#define GP_BASE 0x3f200000
#define GPFSEL1 0x3f200004
#define GPSET0  0x3f20001C
#define NULLSTK 8192

.section .init
	.globl _start

	/* _start:  Entry point of the Xinu kernel.  This will be the very first
	 * byte of the kernel image and on the Raspberry Pi 3 will be loaded by
	 * the GPU at address 0x80000.  */
	.func _start
_start:
	/* Save the pointer to the atags (ARM boot tags). The standard protocol for
	 * Aarch64 (ARMv8-A) is to pass the atags pointer into register 0 (x0). And registers
	 * x1 through x3 are initialized to zero. Refer to this link:
	 * https://github.com/raspberrypi/tools/blob/master/armstubs/armstub8.S	*/

	ldr 	x4, =atags_ptr
	cmp	x0, #0
	b.ne	_start0
	mov	x0, 0x100

_start0:
	str 	x0, [x4]


	/* Clear the .bss section of the kernel.  */
	ldr x0, =_bss
	ldr x1, =_end
  	mov x2, #0
  	b bssloopa
bssloopb:
  	str x2, [x0, 8]
  	str x2, [x0, 16]
  	str x2, [x0, 24]
  	str x2, [x0, 32]
  	add x0, x0, 32

bssloopa:
	cmp x0, x1 	/* check that we still haven't hit the end of bss yet */
	b.lo bssloopb	/* if still below, go backwards and loop */



	/* Put the null thread's stack directly after the kernel image.  */
	add sp, x1, #NULLSTK

	/* The remaining memory available to the ARM will be Xinu's "memheap"
	 * region, which is used for dynamic memory allocation.  Set its
	 * starting address.  */
	ldr x0, =memheap
        mov x1, sp
        str x1, [x0]

  	mrs x7, mpidr_el1
  	and x7, x7, #3
  	cbz x7, __start_master
	0:  wfe
  	b   0b

	/* Branch to nulluser function, located in xinu/system/initialize.c
	 * The function will initialize the system and become the null thread. */ 
__start_master:
	b	nulluser
	.endfunc
