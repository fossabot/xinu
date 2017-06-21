/**
 * @file setupStack.c
 */
/* Embedded Xinu, Copyright (C) 2013.  All rights reserved. */

#include <platform.h>
#include <arm64.h>

/* Length of ARM context record in words (includes r0-r11, cpsr, lr, pc).  */
#define CONTEXT_WORDS 29

/* The standard ARMv8 calling convention passes first eight arguments in x0-x7; the
 * rest spill onto the stack.  */
#define MAX_REG_ARGS 8

/** Set up the context record and arguments on the stack for a new thread
 * (ARM version)  */
void *setupStack(void *stackaddr, void *procaddr,
                 void *retaddr, uint nargs, va_list ap)
{
    uint spilled_nargs;
    uint reg_nargs;
    uint i;
    ulong *saddr = stackaddr;

    /* Determine if any arguments will spill onto the stack (outside the context
     * record).  If so, reserve space for them.  */
    if (nargs > MAX_REG_ARGS) {
        spilled_nargs = nargs - MAX_REG_ARGS;
        reg_nargs = MAX_REG_ARGS;
        saddr -= spilled_nargs;
    } else {
        spilled_nargs = 0;
        reg_nargs = nargs;
    }

    // this is zero.
    kprintf("reg_nargs is set to: %d\r\n", reg_nargs);


    kprintf("saddr b4 align: %d\r\n", saddr);
    /* Possibly skip a word to ensure the stack is aligned on 8-byte boundary
     * after the new thread pops off the context record.  */
    if ((ulong)saddr & 0x8)
    {
	kprintf("enter stack align");
        --saddr;
    }

    kprintf("saddr after stack align: %d\r\n", saddr);

    /* Construct the context record for the new thread.  */
    saddr -= CONTEXT_WORDS;
    saddr[0] = 0;
	

    /* Arguments passed in registers (part of context record)  */
    for (i = 0; i < reg_nargs; i++)
    {
	kprintf("in reg nargs loop\r\n");
        saddr[i] = va_arg(ap, ulong);
    }

	kprintf("b4 ctx words loop\r\n");
    for (; i < CONTEXT_WORDS - 3; i++)
    {
        kprintf("saddr= %d\r\n", saddr); 
        saddr[i] = 0;
	kprintf("Value of i: %d\r\n", i);
    }
    

    /* Control bits of program status register
     * (SYS mode, IRQs initially enabled) */
    saddr[CONTEXT_WORDS - 3] = ARM_MODE_SYS | ARM_F_BIT;

    /* return address  */
    saddr[CONTEXT_WORDS - 2] = (ulong)retaddr;

    /* program counter  */
    saddr[CONTEXT_WORDS - 1] = (ulong)procaddr;

    /* Arguments spilled onto stack (not part of context record)  */
    for (i = 0; i < spilled_nargs; i++)
    {
        saddr[CONTEXT_WORDS + i] = va_arg(ap, ulong);
    }

    /* Return "top" of stack (lowest address).  */
    return saddr;
}
