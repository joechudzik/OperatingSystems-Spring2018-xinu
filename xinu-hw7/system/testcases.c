/* TA-BOT:MAILTO joseph.chudzik@marquette.edu */
/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *	
 * and
 *	
 */
/* Embedded XINU, Copyright (C) 2010, 2014.  All rights reserved. */

#include <xinu.h>

void printFreelist(void){
	kprintf("///// FREELIST /////\r\n");
	memblk *curr = freelist.next;
	while( curr != NULL ) {
		kprintf("location 0x%08X: size %d\r\n", curr, curr->length);
		curr = curr->next;
	}	
	kprintf("///// END OF FREELIST /////\r\n");
}

ulong rand(void)
{
    ulong a = 1664525UL;
    ulong b = 1013904223UL;
    static ulong c = 0;

    c = a * c + b;
    return c;
}

syscall sleep(int time)
{
    /* Dumbest sleep ever. */
    int i = 0, j = 0;
    for (i = 0; i < time; i++)
        for (j = 0; j < 1000; j++)
            ;
    return 0;
}

/* BEGIN Textbook code from Ch 5 Programming Project 3, Silberschatz p. 254 */
typedef int buffer_item;
#define BUFFER_SIZE 5

struct boundedbuffer
{
    buffer_item buffer[BUFFER_SIZE];
    int bufferhead;
    int buffertail;

    semaphore empty;
    semaphore full;
    semaphore mutex;
};

int insert_item(struct boundedbuffer *bb, buffer_item item)
{
    // TODO:
    /* insert item into buffer
     * return 0 if successful, otherwise
     * return SYSERR indicating an error condition */

	if( wait(bb->full) != OK )
		return SYSERR;

	if( wait(bb->mutex) != OK )
		return SYSERR;

	bb->buffer[bb->buffertail] = item;
	bb->buffertail = (bb->buffertail+1) % BUFFER_SIZE;

	if( signal(bb->empty) != OK )
		return SYSERR;

	if( signal(bb->mutex) != OK )
		return SYSERR;

	return 0;
}

int remove_item(struct boundedbuffer *bb, buffer_item * item)
{
    // TODO:
    /* remove an object from buffer
     * placing it in item
     * return 0 if successful, otherwise
     * return SYSERR indicating an error condition */

	if( wait(bb->empty) != OK )
		return SYSERR;
	if( wait(bb->mutex) != OK )
		return SYSERR;

	*item = bb->buffer[bb->bufferhead];
	bb->bufferhead = (bb->bufferhead+1) % BUFFER_SIZE;

	if( signal(bb->full) != OK )
		return SYSERR;
	if( signal(bb->mutex) != OK )
		return SYSERR; 

   return 0;
}

void producer(struct boundedbuffer *bb)
{
    buffer_item item;
    enable();
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 100);
        /* generate a random number */
        item = rand();
        if (insert_item(bb, item)){
		mutexAcquire();
            kprintf("report error condition\r\n");
        	mutexRelease();
	}
	else{
		mutexAcquire();
            kprintf("producer %d produced %d\r\n", currpid, item);
    		mutexRelease();
	}
    }
}

void consumer(struct boundedbuffer *bb)
{
    buffer_item item;
    enable();
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 100);
        if (remove_item(bb, &item))
	{
		mutexAcquire();
            kprintf("report error condition\r\n");
		mutexRelease();
	}
        else{
		mutexAcquire();
            kprintf("consumer %d consumed %d\r\n", currpid, item);
    		mutexRelease();
	}
}

/* END Textbook code from Ch 5 Programming Project 3, Silberschatz p. 254 */

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    struct boundedbuffer bbuff;

	mutexInit();
	mutexAcquire();
	kprintf("0) test print freelist\r\n");
	kprintf("Test 1, testing getmem(0)\r\n");
	kprintf("Test 2, testing freemem(<minheap, some size)\r\n");
	kprintf("Test 3, testing freemem(>maxRAM, some size)\r\n");
	kprintf("Test 4, testing getmmem(word), freemem(ptr, word)\r\n");
	kprintf("Test 5, get all of memory in 8 byte chunks, free in 8 byte chunks, get all of memory in 1 big chunk, free all of memory in 1 big chunk\r\n");
	kprintf("Test 6, testing malloc and free\r\n");
	kprintf("Test 7, grab 4 chunks of freelist.length/4\r\n");
	kprintf("Test 8, test malloc and free with 2 32 byte chunks and free 1 chunk and repeat\r\n");
	kprintf("Test 9, test compaction\r\n");
	kprintf("Test A, get all of memory in 8 byte chunks, free in 8 byte chunks, get all of memory in 1 big chunk, free all of memory in 1 big chunk\r\n");
	kprintf("===TEST BEGIN===\r\n");
	mutexRelease();

    // TODO: Test your operating system!

	while('Q' != (c = kgetc()) )
	{
		switch(c){
			case '0':
				mutexAcquire();
				printFreelist();
				mutexRelease();
				break;
			case '1':
				mutexAcquire();
				kprintf("Test 1, testing getmem(0)...\r\n");
				mutexRelease();
				if( (void*)SYSERR == getmem(0) )
					kprintf("Worked\r\n");
				else
					kprintf("Failed\r\n");
				printFreelist();
				break;
			case '2':
				kprintf("Test 2, testing freemem(<minheap, some size)...\r\n");
				if( SYSERR == freemem(0,8) )
					kprintf("Worked\r\n");
				else
					kprintf("Failed\r\n");
				printFreelist();
				break;
			case '3':
				kprintf("Test 3, testing freemem(>maxRAM, some size)...\r\n");
				if( SYSERR == freemem( (ulong *)0xFFFFFFFF, 8) )
					kprintf("Worked\r\n");
				else
					kprintf("Failed\r\n");
				printFreelist();
				break;
			case '4':
				kprintf("Test 4, testing getmem(word), freemem(ptr, word)...\r\n");
				ulong *dummymem = (ulong *)getmem(8);
				printFreelist();
				freemem(dummymem, 8);
				printFreelist();
				break;
			case '5':
				kprintf("Test 5, get all of memory in 8 byte chunks, get all of memory in 1 big chunk, free all of memory in 1 big chunk\r\n");
				kprintf("Starting to get memory\r\n");
				ulong *first = getmem(8);
				while( (void *)SYSERR != getmem(8) ){
					if( freelist.length < 100 )
						printFreelist();
				}
				kprintf("Finished getting memory\r\n");
				printFreelist();

				while( SYSERR != freemem(first+=8/4,8) ){
					if( freelist.length > 16647000 )
						printFreelist();
				}
				kprintf("Finished freeing memory\r\n");
			case 'b':
				kprintf("Test 5b, get all of memory in 1 big chunk, free all of memory in 1 big chunk twice\r\n");
				ulong allSize = freelist.length;
				ulong *all = (ulong *)getmem(allSize);
				printFreelist();
				freemem(all, allSize);
				printFreelist();
				all = (ulong *)getmem(allSize);
				printFreelist();
				freemem(all, allSize);
				printFreelist();
				break;
			case '6':
				kprintf("Test 6, testing malloc and free\r\n");
				ulong *dummy = (ulong *)malloc(32);
				if( (void*)SYSERR == dummy )
					kprintf("MALLOC ERROR\r\n");
				kprintf("dummy: 0x%08X 0x%08X\r\n", dummy, *dummy);
				printFreelist();
				break;
			case '7':
				kprintf("Test 7, grab 4 chunks of freelist.length/4\r\n");
				ulong freespace = freelist.length/4;
				printFreelist();
				kprintf("\r\nGetting memory\r\n");
				void *test1 = getmem(freespace);
				printFreelist();
				void *test2 = getmem(freespace);
				printFreelist();
				void *test3 = getmem(freespace);
				printFreelist();
				void *test4 = getmem(freespace);
				printFreelist();
				kprintf("\r\nNow freeing memory\r\n");
				if( test1 != (void *)SYSERR )
					freemem(test1, freespace);
				printFreelist();
				if( test2 != (void *)SYSERR )
					freemem(test2, freespace);
				printFreelist();
				if( test3 != (void *)SYSERR )
					freemem(test3, freespace);
				printFreelist();
				if( test4 != (void *)SYSERR )
					freemem(test4, freespace);
				printFreelist();
				break;
			case '8':
				kprintf("Test 8, test malloc and free with malloc 2 32 byte chunks and free 1 chunk and repeat\r\n");
				ulong *pointers;
				int i;
				for( i=0; i<1000; i++ ){
					pointers = malloc(32);
					malloc(32);	
					if( (void*)SYSERR != pointers )
						free(pointers);
					printFreelist();
				}
				break;
			case '9':
				kprintf("Test 9, testing compaction\r\n");
				printFreelist();
				ulong *ptrs1 = (ulong *)malloc(32);
				printFreelist();
				ulong *ptrs2 = (ulong *)malloc(32);
				printFreelist();
				ulong *ptrs3 = (ulong *)malloc(32);
				printFreelist();
				free(ptrs1);
				printFreelist();
				free(ptrs3);
				printFreelist();
				free(ptrs2);
				printFreelist();
				break;
			case 'A':
				kprintf("Test A, get all of memory in 8 byte chunks, free in 8 byte chunks, get all of memory in 1 big chunk, free all of memory in 1 big chunk\r\n");
				kprintf("Starting to get memory\r\n");
				ulong *firstmalloc = malloc(8);
				while( (void *)SYSERR != malloc(8) ){
					if( freelist.length < 100 )
						printFreelist();
				}
				kprintf("Finished getting memory\r\n");
				printFreelist();
				
				while( SYSERR != free(firstmalloc+=8/2) ){
					if( freelist.length > 16647000 )
						printFreelist();
				}
				kprintf("Finished freeing memory\r\n");
				break;
			default:
				break;
			}
		}

    while (numproc > 2)
    {
        resched();
    }
    kprintf("\r\n===TEST END===\r\n");
    return;
}
}
