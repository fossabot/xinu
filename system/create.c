/**
 * @file create.c
 */
/* Embedded Xinu, Copyright (C) 2007, 2013.  All rights reserved. */

#include <platform.h>
#include <string.h>
#include <thread.h>

static int thrnew(void);

/**
 * @ingroup threads
 *
 * Create a thread to start running a procedure.
 *
 * @param procaddr
 *      procedure address
 * @param ssize
 *      stack size in bytes
 * @param priority
 *      thread priority (0 is lowest priority)
 * @param name
 *      name of the thread, used for debugging
 * @param nargs
 *      number of arguments that follow
 * @param ...
 *      arguments to pass to thread procedure
 * @return
 *      the new thread's thread id, or ::SYSERR if a new thread could not be
 *     @ created (not enough memory or thread entries).
 */
tid_typ create(void *procaddr, uint ssize, int priority,
               const char *name, int nargs, ...)
{
    irqmask im;                 /* saved interrupt state               */
    ulong *saddr;               /* stack address                       */
    tid_typ tid;                /* new thread ID                       */
    struct thrent *thrptr;      /* pointer to new thread control block */
    va_list ap;                 /* list of thread arguments            */
    kprintf("Section 1 - ssize= %d\r\n", ssize);
    im = disable();
    kprintf("Section 2\r\n");

    if (ssize < MINSTK)
    {
        ssize = MINSTK;
    }
    kprintf("Section 3 (b4 stkget): saddr=%d\r\n", saddr);

    /* Allocate new stack.  */
    saddr = stkget(ssize);

    /* saddr is 134217724 (128KB). Ssize is set to INIT64STK which is initialized in thread.h */
    kprintf("Section 3.5 (after stkget): Stack value (saddr)= %d, Address ref= %d\n", saddr, &saddr);
    if (SYSERR == (int)saddr)
    {
        restore(im);
        return SYSERR;
    }
    kprintf("Section 4\r\n");

    /* Allocate new thread ID.  */
    tid = thrnew();
    if (SYSERR == (int)tid)
    {
        stkfree(saddr, ssize);
        restore(im);
        return SYSERR;
    }
    kprintf("Section 5\r\n");

    /* Set up thread table entry for new thread.  */
    thrcount++;
    thrptr = &thrtab[tid];
    kprintf("Section 6\r\n");

    thrptr->state = THRSUSP;
    thrptr->prio = priority;
    thrptr->stkbase = saddr;
    thrptr->stklen = ssize;
    strlcpy(thrptr->name, name, TNMLEN);
    thrptr->parent = gettid();
    thrptr->hasmsg = FALSE;
    thrptr->memlist.next = NULL;
    thrptr->memlist.length = 0;
    kprintf("Section 7\r\n");

    /* Set up default file descriptors.  */
    thrptr->fdesc[0] = CONSOLE; /* stdin  is console */
    thrptr->fdesc[1] = CONSOLE; /* stdout is console */
    thrptr->fdesc[2] = CONSOLE; /* stderr is console */

    kprintf("Section 3.5: Stack value (saddr)= %d, Address ref= %d\n", saddr, &saddr);

    /* Set up new thread's stack with context record and arguments.
     * Architecture-specific.  */
    va_start(ap, nargs);
    kprintf("Section 7.5 (after descriptors are set up)\r\n");
    int i;
    thrptr->stkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    va_end(ap);
    kprintf("Section 8 (after setupstack called)\r\n");

    /* Restore interrupts and return new thread TID.  */
    restore(im);
    kprintf("Section 9 (after restore is called)\r\n");
    return tid;
}

/*
 * Obtain a new (free) thread ID.  Returns a free thread ID, or SYSERR if all
 * thread IDs are already in use.  This assumes IRQs have been disabled so that
 * the contents of the threads table are stable.
 */
static int thrnew(void)
{
    int tid;
    static int nexttid = 0;

    /* check all NTHREAD slots    */
    for (tid = 0; tid < NTHREAD; tid++)
    {
        nexttid = (nexttid + 1) % NTHREAD;
        if (THRFREE == thrtab[nexttid].state)
        {
            return nexttid;
        }
    }
    return SYSERR;
}
