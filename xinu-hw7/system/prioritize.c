#include <xinu.h>
#include <queue.h>

short prioritize( short pid, qid_typ q, ulong key )
{
	if( isbadqueue(q) || isbadpid(pid) )
	{
		return SYSERR;
	}
	int head, index;
	head = queuehead(q);
	index = queuetab[queuetail(q)].prev;
	while( index != head && key > queuetab[index].key )
	{
		index = queuetab[index].prev;
	}
	queuetab[pid].prev = index;
	queuetab[pid].next = queuetab[index].next;
	queuetab[index].next = pid;
	queuetab[queuetab[pid].next].prev = pid;

	queuetab[pid].key = key;

	return pid;
}
