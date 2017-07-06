/**
 * @file ready.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <thread.h>
#include <queue.h>

/**
 * @ingroup threads
 *
 * Make a thread eligible for CPU service.
 * @param tid target thread
 * @param resch if RESCHED_YES, reschedules
 * @return OK if thread has been added to readylist, else SYSERR
 */
int ready(tid_typ tid, bool resch)
{
    register struct thrent *thrptr;

    if (isbadtid(tid))
    {
        return SYSERR;
    }
    kprintf("\r\nreach ready 1\r\n");
    thrptr = &thrtab[tid];
    thrptr->state = THRREADY;


    kprintf("\r\nreach ready 2\r\n");
    insert(tid, readylist, thrptr->prio);

    kprintf("after insert\r\n");

    if (resch == RESCHED_YES)
    {
	
        kprintf("reach resched = yes\r\n");
        resched();
    }

    return OK;
}
