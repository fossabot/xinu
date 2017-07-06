
/**
 * @file processcreation.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by: Priya Bansal
 *
 * and Ben Mol
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */


#include <thread.h>
#include <arm64.h>

extern void main(int, char *);

int testproc()
{
    // Test job for the process to handle
    kprintf("*TESTPROC*\r\n");
    return 0;
}

void testmain()
{

        kprintf("\r\n------------------------------- This is process 1 -------------------------------\r\n");
	ready(create((void *) testproc, INIT64STK, 1, "PROCESS 1", 0, NULL),0);
	kprintf("Done readying process 1\r\n");
       
       	kprintf("\r\n------------------------------- This is process 2 -------------------------------\r\n");
	ready(create((void *) testproc, INIT64STK, 2, "PROCESS 2", 0, NULL),0);
	kprintf("Done readying process 2\r\n");
	
	while(1)
	{
		kprintf("Rescheding...\r\n");
		resched(); 
		kprintf("Resched complete.\r\n");
	}
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
    kprintf("i = 0x%08X\r\n", i);
}
void printpcb(int tid)
{
    struct thrent *tthrent = NULL;

    kprintf("b4 tthrent=thrtab\r\n");
    /* Using the process ID, access it in the PCB table. */
    tthrent = &thrtab[tid];
    kprintf("after tthrent=thrtab\r\n");

    /* Printing PCB */
     kprintf("Process name                 : %s \r\n", tthrent->name);
    switch (tthrent->state)
    {
    case THRFREE:
        kprintf("State of the process     : FREE \r\n");
        break;
    case THRCURR:
        kprintf("State of the process     : CURRENT \r\n");
        break;
    case THRSUSP:
        kprintf("State of the process     : SUSPENDED \r\n");
        break;
    case THRREADY:
        kprintf("State of the process     : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", tthrent->stkbase);
    //kprintf("Stack pointer of process  : 0x%08X \r\n",
    //        tthrent->regs[PREG_SP]);
    kprintf("Stack length of process   : %8u \r\n", tthrent->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
/*void testcases(void)
{
    int c, pid;
1431116693    kprintf("0) Test creation of one process\r\n");
    kprintf("1) Test passing of many args\r\n");
    kprintf("2) Run 50 processes, resched, run 50 more, and see if anything breaks\r\n");
    kprintf("3) Create three processes and run them\r\n");
    kprintf("===TEST BEGIN===\r\n");
    c = kgetc();
    int i = 0;
    switch (c)
    {
    case '0':
        pid = create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL);
        kprintf("created pid = %d\r\n", pid);
        printpcb(pid);
        break;
    case '1':
        pid = create((void *)testbigargs, INITSTK, "MAIN1", 9,
                     0x11111111, 0x22222222, 0x33333333, 0x44444444,
                     0x55555555, 0x66666666, 0x77777777, 0x88888888, 0x99999999);
        printpcb(pid);
        pcb *pcb_p = &proctab[pid];
        ulong *saddr = (ulong *) pcb_p->regs[PREG_SP];
        for (i = 0; i<4; i++) { // Print out the args in the registers (0-3)
                kprintf("Argument %d : 0x%08X\r\n", i, (int) pcb_p->regs[i]);
        }
        for (i = 0; i<5; i++) { // Print out the args in the stack (4-8)
            kprintf("Argument %d : 0x%08X\r\n", i+4, (int) *(saddr + i));
        }
        kprintf("Padding    : 0x%08X\r\n", (int) *(saddr + 5));
        kprintf("Padding    : 0x%08X\r\n", (int) *(saddr + 6));
        kprintf("Padding    : 0x%08X\r\n", (int) *(saddr + 7));
        kprintf("Stack Base : 0x%08X\r\n", (int) *(saddr + 8));
        kprintf("Stack Size : 0x%08X\r\n", (int) *(saddr + 9));
        kprintf("PID        : 0x%08X\r\n", (int) *(saddr + 10));
        kprintf("StackMAGIC : 0x%08X\r\n", (int) *(saddr + 11));
        ready(pid, 0);
        break;
     case '2':;
        for (i = 0; i < 50; i++)
        {
            ready(create((void *)main, INITSTK, "MOMMY!!!!!!!!!", 2, 0, NULL), 0);
        }
        while (numproc > 1)
            resched();
        for (i = 0; i < 50; i++)
        {
            ready(create((void *)main, INITSTK, "MOMMY!!!!!!!!!", 2, 0, NULL), 0);
        }
        while (numproc > 1)
            resched();
        break;
    default:
        // Create three copies of a process, and let them play.
        ready(create((void *)main, INITSTK, "MAIN1", 2, 0, NULL), 0);
        ready(create((void *)main, INITSTK, "MAIN2", 2, 0, NULL), 0);
        ready(create((void *)main, INITSTK, "MAIN3", 2, 0, NULL), 0);
        while (numproc > 1)
            resched();
        break;
    }
    kprintf("\r\n===TEST END===\r\n");
    return;
}*/
