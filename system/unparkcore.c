#include <stddef.h>
#include <thread.h>
extern void unparkcore(int, void *);
extern void Core1Setup (void) __attribute__((naked));
extern void Core2Setup (void) __attribute__((naked));
extern void Core3Setup (void) __attribute__((naked));
typedef void (*fn)(void);

void *corestart;
int numcore;

void unparkcore(int num, void *procaddr) {
    corestart = procaddr;
    numcore = num;
    if (num == 1)
    	*(volatile fn *)(0x4000008C + 0x10 * num) = Core1Setup;
    if (num == 2)
    	*(volatile fn *)(0x4000008C + 0x10 * num) = Core2Setup;
    if (num == 3)
    	*(volatile fn *)(0x4000008C + 0x10 * num) = Core3Setup;
}

void createnullthread(void)
{
    kprintf("Beginning of createnullthread\r\n");
    kprintf("Corestart is now %x\r\n", corestart);
    create(corestart, INITSTK, 5, "null thread", 0, NULL);
    kprintf("End of createnullthread\r\n");
}



