/**
 * @file insert.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <stddef.h>
#include <thread.h>
#include <queue.h>

/**
 * @ingroup threads
 *
 * Insert a thread into a queue in descending key order.
 * @param tid    thread ID to insert
 * @param q      target queue
 * @param key    sorting key
 * @return OK
 */
int insert(tid_typ tid, qid_typ q, int key)
{
	int next;                   /* runs through list         */
	int prev;                   /* follows next through list */

	if (isbadqid(q) || isbadtid(tid))
	{
		kprintf("BAD QID... SYSERR\r\n");
		return SYSERR;
	}

	next = quetab[quehead(q)].next;

	while (quetab[next].key >= key)
	{
		kprintf("key=%d\r\n", key);
		kprintf("quetab[next].key=%d\r\n", quetab[next].key);
		kprintf("enter insert while next loop\r\n");
		next = quetab[next].next;
	}

	/* insert tid between prev and next */
	quetab[tid].next = next;
	quetab[tid].prev = prev = quetab[next].prev;
	quetab[tid].key = key;
	quetab[prev].next = tid;
	quetab[next].prev = tid;

	return OK;
}
