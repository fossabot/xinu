/**
 * @file initialize.c
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null thread remains always in
 * a ready (THRREADY) or running (THRCURR) state.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>
#include <platform.h>

/* Function prototypes */
extern thread main(void);       /* main is the first thread created    */
static int sysinit(void);       /* intializes system structures        */

/* Declarations of major kernel variables */
struct thrent thrtab[NTHREAD];  /* Thread table                   */
struct sement semtab[NSEM];     /* Semaphore table                */
struct monent montab[NMON];     /* Monitor table                  */
qid_typ readylist;              /* List of READY threads          */
struct memblock memlist;        /* List of free memory blocks     */
struct bfpentry bfptab[NPOOL];  /* List of memory buffer pools    */

/* Active system status */
int thrcount;                   /* Number of live user threads         */
tid_typ thrcurrent;             /* Id of currently running thread      */

/* Params set by startup.S */
void *memheap;                  /* Bottom of heap (top of O/S stack)   */
ulong cpuid;                    /* Processor id                        */

struct platform platform;       /* Platform specific configuration     */

#define IO_BASE     0x3f000000
#define GP_BASE     (IO_BASE + 0x200000)

#define GPFSEL1     (*(volatile unsigned *)(GP_BASE + 0x04))
#define GPPUD       (*(volatile unsigned *)(GP_BASE + 0x94))
#define GPPUDCLK0   (*(volatile unsigned *)(GP_BASE + 0x98))
#define GPSET0      (*(volatile unsigned *)(GP_BASE + 0x1C))
#define GPCLR0      (*(volatile unsigned *)(GP_BASE + 0x28))
#define GPLEV0		(*(volatile unsigned *)(GP_BASE + 0x34))

#define _UART_CLK    48000000	/* UART CLOCK is set to 48MHz, which is the UART clock of
				 * all Raspberry Pis (as of updated 2016 firmware) */
void init_led(void)
{
	GPFSEL1 &= ~(7 << 18); // GPIO Pin 16
	GPFSEL1 |= 1 << 18;    // Set as output
}

void init_button(void)
{
	GPFSEL1 &= ~(7 << 21); // GPIO Pin 17 as input
}

void led_on(void)
{
	GPSET0 = 1 << 16;
}

void led_off(void)
{
	GPCLR0 = 1 << 16;
}

int button_lev(void)
{
	return (GPLEV0 & (1 << 17));
}

interrupt gpio_handler(void)
{
	kprintf("gpio interrupt received... turning on led\r\n");
	led_on();
}

/*
 * Intializes the system and becomes the null thread.
 * This is where the system begins after the C environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  This routine turns itself into the null thread
 * after initialization.  Because the null thread must always remain ready
 * to run, it cannot execute code that might cause it to be suspended, wait
 * for a semaphore, or put to sleep, or exit.  In particular, it must not
 * do I/O unless it uses kprintf for synchronous output.
 */
void nulluser(void)
{
	int lev;

	init_led();
	init_button();

	/* Platform-specific initialization (system/platforms/arm-rpi3/platforminit.c) */
	platforminit();

	/* General initialization  */
	sysinit();
	kprintf("Hello Xinu World!\r\n-----------------\r\n");
    
    ulong *add = 989855732;
    int i = 0;
    
    for (i = 0; i <= 1000; i += 1)
    {
        add[i] = 0;
        kprintf("Memory address %d has %d\r\n", i, add[i]);
        kprintf("Memory address %d is available\r\n", i);
    }
    
	/* Print memory usage (located in system/main.c) */
	print_os_info();

	/* Call to test method (located in system/main.c) */
	testmain();

	/* Enable interrupts  */
	enable();

	interruptVector[IRQ_TIMER] = 0;
	enable_irq(IRQ_TIMER);
	clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);
   

	/* Spawn the main thread  */
	//ready(create(main, INITSTK, INITPRIO, "MAIN", 0), RESCHED_YES);

	/* null thread has nothing else to do but cannot exit  */
	while (TRUE){}
}

/**
 * Intializes all Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
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
	//	readylist = queinit();

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
