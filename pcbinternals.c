#include <stdlib.h>
/* Debugging. */
#include <stdio.h>

#include "pcb.h"
#include "pcbinternals.h"

void foreachpcb(struct pcbqueue *pQueue, void(*pcbitr)(struct pcb *, void *), void *arg) {
	/* ID of the initial PCB. */
	int fid;
	
	/* Current PCB. */
	struct pcb *pPCB;

	/* Skip empty queues. */
	if(pQueue->pHead == NULL) return;

	/* Initialize iteration. */
	pPCB = pQueue->pHead;
	fid  = pPCB->id;

	do {
		pcbitr(pPCB, arg);

		pPCB = pPCB->pNext;
	} while(pPCB->id != fid);
}
