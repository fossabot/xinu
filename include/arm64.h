#ifndef  _ARM64_H_
#define  _ARM64_H_

/* Definitions of mode bits in the ARM program status register.  See: A2.2
 * "Processor Modes" of the ARM Architecture Reference Manual; also A2.5.7 "The
 * mode bits".  */
#define ARM_MODE_USR 0x10       /* Normal User Mode                                       */
#define ARM_MODE_FIQ 0x11       /* FIQ Processing Fast Interrupts Mode                    */
#define ARM_MODE_IRQ 0x12       /* IRQ Processing Standard Interrupts Mode                */
#define ARM_MODE_SVC 0x13       /* Supervisor Processing Software Interrupts Mode         */
#define ARM_MODE_ABT 0x17       /* Abort Processing memory Faults Mode                    */
#define ARM_MODE_UND 0x1B       /* Undefined Processing Undefined Instructions Mode       */
#define ARM_MODE_SYS 0x1F       /* System Running Priviledged Operating System Tasks Mode */

/* Definitions of interrupt disable bits in the ARM program status register.
 * See: A2.5.6 "The interrupt disable bits" of the ARM Architecture Reference
 * Manual.  */
#define ARM_I_BIT 0x80          /* IRQs disabled when set to 1. */
#define ARM_F_BIT 0x40          /* FIQs disabled when set to 1. */


/**
* Process Control Block register constants.
* These are the indices of registers stored in a PCB.
*/

#define PREG_X0     0           /* Parameter and result registers               */
#define PREG_X1     1
#define PREG_X2     2           
#define PREG_X3     3
#define PREG_X4     4           
#define PREG_X5     5
#define PREG_X6     6
#define PREG_X7     7
#define PREG_XR     8           /* Indirect result location register            */
#define PREG_X9     9           /* Caller-saved temporary registers             */
#define PREG_X10    10
#define PREG_X11    11
#define PREG_X12    12          
#define PREG_X13    13         
#define PREG_X14    14          
#define PREG_X15    15         
#define PREG_IP0    16          /* First intra-procedural scratch register       */
#define PREG_IP1    17          /* Second intra-procedural scratch register      */
#define PREG_PR     18          /* Platform register                             */
#define PREG_X19    19          /* Callee-saved registers                        */
#define PREG_X20    20
#define PREG_X21    21
#define PREG_X22    22
#define PREG_X23    23
#define PREG_X24    24
#define PREG_X25    25
#define PREG_X26    26
#define PREG_X27    27
#define PREG_X28    28
#define PREG_FP     29
#define PREG_LR     30          /* Link register                                 */


#define PREGS       16          /* Regs stored by context switches               */


#endif                          /* _ARM_H_ */
