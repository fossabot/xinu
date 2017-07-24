
void testcore1(void);
void testcore2(void);
void testcore3(void);
void testcore4(void);
void testallcores(void);

int semaphore = 0;

void testallcores(void)
{
	kprintf("Reached testallcores\r\n");
	unparkcore(1, (void *)testcore1);
	kprintf("Unparked core 1\r\n");
	unparkcore(2, (void *)testcore2);
	kprintf("Unparked core 2\r\n");
	unparkcore(3, (void *)testcore3);
	kprintf("Unparked core 3\r\n");
}

void testcore1(void)
{
	while(1)
		kprintf("11111111111111111111111111111111\r\n");
}

void testcore2(void)
{
	while(1)
		kprintf("22222222222222222222222222222222\r\n");
}

void testcore3(void)
{
	while(1)
		kprintf("33333333333333333333333333333\r\n");
}

void testcore4(void)
{
	while(1)
		kprintf("I am core 4\r\n");
}
