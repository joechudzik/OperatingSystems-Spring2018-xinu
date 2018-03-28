/**
 * @file getmem.c
 * @provides getmem                                                       
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap storage, returning pointer to assigned memory region.
 * @param nbytes number of bytes requested
 * @return pointer to region on success, SYSERR on failure
 */
void *getmem(uint nbytes)
{
	// TODO: Search free list for first chunk large enough to fit.
	//       Break off region of requested size; return pointer
	//       to new memory region, and add any remaining chunk
	//       back into the free list.

	ulong im = disable();

	if( 0>=nbytes ){
		restore(im);
		return (void *)SYSERR;
	}

	nbytes = (ulong)roundmb(nbytes);

	memblk *curr = freelist.next;
	memblk *prev = NULL;

	while(TRUE){
		if( curr==NULL ){
			restore(im);
			return (void *)SYSERR;
		}
		if( curr->length < nbytes ){
			prev = curr;
			curr = curr->next;
		}
		else{
			memblk *newblock;
			newblock = (memblk *)(curr + nbytes/8);
			newblock->next = curr->next;
			newblock->length = curr->length - nbytes;
			if( newblock->length <= 0 )
				newblock = curr->next;
			if( prev==NULL )
				freelist.next = newblock;
			else
				prev->next = newblock;
			freelist.length -= nbytes;
			restore(im);
			return curr;
		}
	}
}
