
/**
 * @file tset_processcreation.c
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */


#include <thread.h>
#include <arm.h>
#include <stdint.h>
#include <xinu.h>
#define CORE1_MAILBOX ((volatile __attribute__((aligned(4))) uint32_t*) (0x4000009C))

extern void main(int, char *);
void testproc(void);
void printtid(int tid);
void testbigargs(int, int, int, int, int, int, int, int, int);
void somerand(void) __attribute__((naked));
static int sysinit(void);       /* intializes system structures        */
extern void ExtraCoreSetup (void) __attribute__((naked));

void testproc(void)
{
    // Test job for the process to handle
    kprintf("\r\nTesting random job...\r\n");
    kprintf("Current running thread: %d\r\n\r\n", thrcurrent);
kprintf("	               ___          \r\n");
kprintf("	             _//_||         \r\n");
kprintf("	           ,'    //'.       \r\n");
kprintf("	          /          )      \r\n");
kprintf("	        _/           |      \r\n");
kprintf("	       (.-,--.       |      \r\n");
kprintf("	       (o/  o |     /       \r\n");
kprintf("	       |_|    /  /|/|       \r\n");
kprintf("	       (__`--'   ._)        \r\n");
kprintf("	       /  `-.     |         \r\n");
kprintf("	      (     ,`-.  |         \r\n");
kprintf("	       `-,--|_  ) |-.       \r\n");
kprintf("	        _`.__.'  ,-' )      \r\n");
kprintf("	       || )  _.-'    |      \r\n");
kprintf("	       i-|.'|     ,--+.     \r\n");
kprintf("	     .' .'   |,-'/     )    \r\n");
kprintf("	    / /         /       )   \r\n");
kprintf("	    7_|         |       |   \r\n");
kprintf("	    |/          '(.___.;'   \r\n");
kprintf("	    /            |     |)   \r\n");
kprintf("	   /             |     | )  \r\n");
kprintf("	  /              |     |  | \r\n");
kprintf("	  |              |     |  | \r\n");
kprintf("	  |____          |     |-;' \r\n");
kprintf("	   |   """"----""|     | |  \r\n");
kprintf("	   (           ,-'     |/   \r\n");
kprintf("	    `.         `-,     |    \r\n");
kprintf("	     |`-._      / /| |} )   \r\n");
kprintf("	     |    `-.   `' | ||`-'  \r\n");
kprintf("	     |      |      `-'|     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     |      |         |     \r\n");
kprintf("	     )`-.___|         |     \r\n");
kprintf("	   .'`-.____)`-.___.-'(     \r\n");
kprintf("	 .'        .'-._____.-|     \r\n");
kprintf("	/        .'           |     \r\n");
kprintf("	`-------/         .   |     \r\n");
kprintf("	        `--------' '--'     \r\n");
}
extern void core_wakeup(void);
typedef void (*fn)(void);
void wakeupc(int num) {
    *(volatile fn *)(0x4000008C + 0x10 * num) = ExtraCoreSetup;
}

extern void core_wakeup(void)
{
    init_led();
    platforminit();
    int i;
	struct thrent *thrptr;      /* thread control block pointer  */
	struct memblock *pmblock;   /* memory block pointer          */

	/* Initialize system variables */
	/* Count this NULLTHREAD as the first thread in the system. */
	thrcount = 1;

	/* Initialize free memory list */
	memheap = roundmb(memheap);
	platform.maxaddr = truncmb(platform.maxaddr);
	memlist.next = pmblock = (struct memblock *)memheap;
	memlist.length = (uint)(platform.maxaddr - memheap);
	pmblock->next = NULL;
	pmblock->length = (uint)(platform.maxaddr - memheap);

	/* Initialize thread table */
	for (i = 0; i < NTHREAD; i++)
	{
		thrtab[i].state = THRFREE;
	}

	/* initialize null thread entry */
	thrptr = &thrtab[NULLTHREAD];
	thrptr->state = THRCURR;
	thrptr->prio = 0;
	strlcpy(thrptr->name, "prnull", TNMLEN);
	thrptr->stkbase = (void *)&_end;
	thrptr->stklen = (ulong)memheap - (ulong)&_end;
	thrptr->stkptr = 0;
	thrptr->memlist.next = NULL;
	thrptr->memlist.length = 0;
	thrcurrent = NULLTHREAD;

	/* Initialize monitors */
	for (i = 0; i < NMON; i++)
	{
		montab[i].state = MFREE;
	}

	/* Initialize buffer pools */
	for (i = 0; i < NPOOL; i++)
	{
		bfptab[i].state = BFPFREE;
	}

	/* initialize thread ready list */
	readylist = queinit();

#ifdef UHEAP_SIZE
	/* Initialize user memory manager */
	{
		void *userheap;             /* pointer to user memory heap   */
		userheap = stkget(UHEAP_SIZE);
		if (SYSERR != (int)userheap)
		{
			userheap = (void *)((uint)userheap - UHEAP_SIZE + sizeof(int));
			memRegionInit(userheap, UHEAP_SIZE);

			/* initialize memory protection */
			safeInit();

			/* initialize kernel page mappings */
			safeKmapInit();
		}
	}
#endif

#if USE_TLB
	/* initialize TLB */
	tlbInit();
	/* register system call handler */
	exceptionVector[EXC_SYS] = syscall_entry;
#endif                          /* USE_TLB */

#if NDEVS
	for (i = 0; i < NDEVS; i++)
	{
		devtab[i].init((device*)&devtab[i]);
	}
#endif

#if GPIO
	gpioLEDOn(GPIO_LED_CISCOWHT);
#endif
    kprintf("I'm here\r\nI'm here\r\nI'm here\r\nI'm here\r\nI'm heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeere");
    
	return OK;
}
void testmain()
{
	wakeupc(1);

	int tid;
        kprintf("\r\n------------------------------- This is thread 1 -------------------------------\r\n");
	tid=create((void *) testproc, INITSTK, 1, "THREAD 1", 0, NULL);
	ready(tid, 0);
	kprintf("Done readying thread: %d\r\n", tid);
	printtid(tid);
       
       	kprintf("\r\n------------------------------- This is thread 2 -------------------------------\r\n");
	tid=create((void *) testbigargs, INITSTK, 2, "THREAD 2", 9, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	ready(tid, 0);
	kprintf("Done readying thread: %d\r\n", tid);
	printtid(tid);
       	
	kprintf("\r\n------------------------------- This is thread 3 -------------------------------\r\n");
	tid=create((void *) testbigargs, INITSTK, 3, "THREAD 3", 9, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	ready(tid, 0);
	kprintf("Done readying thread: %d\r\n", tid);
	printtid(tid);

	while(1)
	{
		kprintf("Rescheding...\r\n");
		resched(); 
		kprintf("Resched complete.\r\n");
	}
}

void somerand(void)
{
    uint mode, cpuid;
	int i;

	init_led();

	/* Platform-specific initialization (system/platforms/arm-rpi3/platforminit.c) */
	platforminit();

	/* General initialization  */
	sysinit();
	kprintf("\r\n***********************************************************\r\n");
	kprintf("******************** Hello Xinu World! ********************\r\n");
	kprintf("***********************************************************\r\n");
	/* Print memory usage (located in system/main.c) */
	print_os_info();
	while(1)
	{
		kprintf("dude\r\n");
	}
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    kprintf("Current running thread: %d\r\n", thrcurrent);
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
void printtid(int tid)
{
    struct thrent *tthrent = NULL;

    /* Using the Thread ID, access it in the TID table. */
    tthrent = &thrtab[tid];

    /* Printing TID */
    kprintf("Thread name              : %s \r\n", tthrent->name);
    switch (tthrent->state)
    {
    case THRFREE:
        kprintf("State of the thread      : FREE \r\n");
        break;
    case THRCURR:
        kprintf("State of the thread      : CURRENT \r\n");
        break;
    case THRSUSP:
        kprintf("State of the thread      : SUSPENDED \r\n");
        break;
    case THRREADY:
        kprintf("State of the thread      : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print TID contents and registers */
    kprintf("Base of run time stack   : 0x%08X \r\n", tthrent->stkbase);
    kprintf("Stack pointer of thread  : 0x%08X \r\n",
            tthrent->stkptr);
    kprintf("Stack length of thread   : %8u\r\n", tthrent->stklen);
}

static int sysinit(void)
{
	int i;
	struct thrent *thrptr;      /* thread control block pointer  */
	struct memblock *pmblock;   /* memory block pointer          */

	/* Initialize system variables */
	/* Count this NULLTHREAD as the first thread in the system. */
	thrcount = 1;

	/* Initialize free memory list */
	memheap = roundmb(memheap);
	platform.maxaddr = truncmb(platform.maxaddr);
	memlist.next = pmblock = (struct memblock *)memheap;
	memlist.length = (uint)(platform.maxaddr - memheap);
	pmblock->next = NULL;
	pmblock->length = (uint)(platform.maxaddr - memheap);

	/* Initialize thread table */
	for (i = 0; i < NTHREAD; i++)
	{
		thrtab[i].state = THRFREE;
	}

	/* initialize null thread entry */
	thrptr = &thrtab[NULLTHREAD];
	thrptr->state = THRCURR;
	thrptr->prio = 0;
	strlcpy(thrptr->name, "prnull", TNMLEN);
	thrptr->stkbase = (void *)&_end;
	thrptr->stklen = (ulong)memheap - (ulong)&_end;
	thrptr->stkptr = 0;
	thrptr->memlist.next = NULL;
	thrptr->memlist.length = 0;
	thrcurrent = NULLTHREAD;

	/* Initialize monitors */
	for (i = 0; i < NMON; i++)
	{
		montab[i].state = MFREE;
	}

	/* Initialize buffer pools */
	for (i = 0; i < NPOOL; i++)
	{
		bfptab[i].state = BFPFREE;
	}

	/* initialize thread ready list */
	readylist = queinit();

#ifdef UHEAP_SIZE
	/* Initialize user memory manager */
	{
		void *userheap;             /* pointer to user memory heap   */
		userheap = stkget(UHEAP_SIZE);
		if (SYSERR != (int)userheap)
		{
			userheap = (void *)((uint)userheap - UHEAP_SIZE + sizeof(int));
			memRegionInit(userheap, UHEAP_SIZE);

			/* initialize memory protection */
			safeInit();

			/* initialize kernel page mappings */
			safeKmapInit();
		}
	}
#endif

#if USE_TLB
	/* initialize TLB */
	tlbInit();
	/* register system call handler */
	exceptionVector[EXC_SYS] = syscall_entry;
#endif                          /* USE_TLB */

#if NDEVS
	for (i = 0; i < NDEVS; i++)
	{
		devtab[i].init((device*)&devtab[i]);
	}
#endif

#if GPIO
	gpioLEDOn(GPIO_LED_CISCOWHT);
#endif
	return OK;
}
