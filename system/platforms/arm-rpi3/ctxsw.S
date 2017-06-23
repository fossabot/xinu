
/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm64.h>

.text
        .align 4
        .globl  ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldregs address of outgoing register save area
 * @param  &newregs address of incoming register save area
 * @return special case -- see above
 */
ctxsw:

/*Store registers in respective places in stack.  Link register gets saved into location for stack pointer*/
/*so when process switches back, program will continue execution in correct location*/

    str x0,  [x0, #PREG_X0*8]
    str x1,  [x0, #PREG_X1*8]
    str x2,  [x0, #PREG_X2*8]
    str x3,  [x0, #PREG_X3*8]
    str x4,  [x0, #PREG_X4*8]
    str x5,  [x0, #PREG_X5*8]
    str x6,  [x0, #PREG_X6*8]
    str x7,  [x0, #PREG_X7*8]
    str x8,  [x0, #PREG_XR*8]
    str x9,  [x0, #PREG_X9*8]
    str x10, [x0, #PREG_X10*8]
    str x11, [x0, #PREG_X11*8]
    str x12, [x0, #PREG_X12*8]
    str x13, [x0, #PREG_X13*8]
    str x14, [x0, #PREG_X14*8]
    str x15, [x0, #PREG_X15*8]
    str x16, [x0, #PREG_IP0*8]
    str x17, [x0, #PREG_IP1*8]
    str x18, [x0, #PREG_PR*8]
    str x19, [x0, #PREG_X19*8]
    str x20, [x0, #PREG_X20*8]
    str x21, [x0, #PREG_X21*8]
    str x22, [x0, #PREG_X22*8]
    str x23, [x0, #PREG_X23*8]
    str x24, [x0, #PREG_X24*8]
    str x25, [x0, #PREG_X25*8]
    str x26, [x0, #PREG_X26*8]
    str x27, [x0, #PREG_X27*8]
    str x28, [x0, #PREG_X28*8]
    str x29, [x0, #PREG_FP*8]
    str x30, [x0, #PREG_LR*8]
    
/*Load registers from respective places in stack.  Using x1 and x2, so these need to be pushed onto stack.*/
/*Program counter must be loaded last, since program will jump.*/
/*In this case, returning is same as loading the link register into the program counter*/
    
    ldr x0, [x1, #PREG_X0*8]
    ldr x3, [x1, #PREG_X3*8]
    ldr x4, [x1, #PREG_X4*8]
    ldr x5, [x1, #PREG_X5*8]
    ldr x6, [x1, #PREG_X6*8]
    ldr x7, [x1, #PREG_X7*8]
    ldr x8, [x1, #PREG_XR*8]
    ldr x9, [x1, #PREG_X9*8]
    ldr x10, [x1, #PREG_X10*8]
    ldr x11, [x1, #PREG_X11*8]
    ldr x12, [x1, #PREG_X12*8]
    ldr x13, [x1, #PREG_X13*8]
    ldr x14, [x1, #PREG_X14*8]
    ldr x15, [x1, #PREG_X15*8]
    ldr x16, [x1, #PREG_IP0*8]
    ldr x17, [x1, #PREG_IP1*8]
    ldr x18, [x1, #PREG_PR*8]
    ldr x19, [x1, #PREG_X19*8]
    ldr x20, [x1, #PREG_X20*8]
    ldr x21, [x1, #PREG_X21*8]
    ldr x22, [x1, #PREG_X22*8]
    ldr x23, [x1, #PREG_X23*8]
    ldr x24, [x1, #PREG_X24*8]
    ldr x25, [x1, #PREG_X25*8]
    ldr x26, [x1, #PREG_X26*8]
    ldr x27, [x1, #PREG_X27*8]
    ldr x28, [x1, #PREG_X28*8]
    ldr x29, [x1, #PREG_FP*8]
    ldr x30, [x1, #PREG_LR*8]
    
    /*Push x1, x2 onto the stack, then load into proper registers from there.*/
    /*Could probably just load into x1 and x2 normally, but not sure if there will*/
    /*be issues using x1 to calculate an offset that will be stored in x1*/
    ldr x2, [x1, #PREG_X2*8] 
    str x2, [sp, #-8]
    ldr x2, [x1, #PREG_X1*8]
    str x2, [sp, #-16]
    ldr x2, [sp, #-8]
    ldr x1, [sp, #-16]
    /*Basically load link register into program counter*/
    /*Not returning back to function we came from, but going to new function in different process, */
    /*since we changed value of link register*/
    ret


        .end ctxsw
