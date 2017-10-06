#include <assert.h>
#include <stdlib.h>

#include "pcb.h"
#include "pcbinternals.h"

/* Allocate/initialize PCB queue. */
struct pcbqueue *makepcbqueue() {
	/* Allocate for the queue, and fail if that does. */
	struct pcbqueue *pQueue = malloc(sizeof(struct pcbqueue));
	assert(pQueue != NULL);

	/* Initialize the queue. */
	pQueue->nprocs = 0;
	pQueue->pHead  = NULL;

	return pQueue;
}

/* Deinitialize/deallocate PCB queue. */
void killpcbqueue(struct pcbqueue *pqQueue) {
	/* PCBs in queue. Kill them. */
	if(pqQueue->pHead != NULL) {
		/* The current PCB to deal with. */
		struct pcb *pPCB;

		/* Initialize it. */
		pPCB = pqQueue->pHead;
		/* Break queue linkage. */
		pPCB->pPrev->pNext = NULL;

		do {
			/* Previous PCB. */
			struct pcb *tmp;

			/* Advance queue. */
			tmp  = pPCB;
			pPCB = pPCB->pNext;

			/* Free PCB. */
			free(tmp);
		} while(pPCB != NULL);
	}

	/* Free the queue. */
	free(pqQueue);
}

/* Do something for every PCB in a queue. */
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
