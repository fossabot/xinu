
void testcore1(void);
void testcore2(void);
void testcore3(void);
void testcore4(void);

int semaphore = 0;

void testcore1(void)
{
	kprintf("I am core 1\r\n");
}

void testcore2(void)
{
	while(1)
		kprintf("I am core 2\r\n");
}

void testcore3(void)
{
	while(1)
		kprintf("I am core 3\r\n");
}

void testcore4(void)
{
	while(1)
		kprintf("I am core 4\r\n");
}
