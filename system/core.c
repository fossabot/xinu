

/*
 *This is a test method that identifies which core the operating system is
 *running from requires that multiple core cannot access this part at the
 *same time or else we run into a race condition.
 *
 */
int printcore()
{
	unsigned int ret;
	asm("mrs	%0, mpidr_el1"
			: "=r"(ret)
			);
	kprintf("Processor Number %d\r\n", ret);
	return ret;
}
