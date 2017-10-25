#include <assert.h>
#include <stdlib.h>

#include "pcb.h"
#include "pcbinternals.h"

/* Allocate/initialize normal (QT_NORMAL) PCB queue. */
struct pcbqueue *makepcbqueue() {
	return maketypedpcbqueue(QT_NORMAL);
}

/* Allocate/initialize PCB queue of a specific type. */
struct pcbqueue *maketypedpcbqueue(enum queuetype type) {
	switch(type) {
	case QT_NORMAL:
		{
			/* Allocate for the queue, and fail if that does. */
			struct pcbqueue *pqQueue = malloc(sizeof(struct pcbqueue));
			assert(pqQueue != NULL);

			/* Initialize the queue. */
			pqQueue->type   = type;
			pqQueue->nprocs = 0;
			pqQueue->pHead  = NULL;

			return pqQueue;
		}
	case QT_PRIORITY:
		{
			/* Loop idx for queue init. */
			int i;

			/* Allocate for the queue, and fail if that does. */
			struct pcbqueueprior *pqrQueue = malloc(sizeof(struct pcbqueueprior));
			assert(pqrQueue != NULL);

			/* Initialize the queue. */
			pqrQueue->type   = type;
			pqrQueue->nprocs = 0;

			/* Set up the the list of queues. */
			pqrQueue->apqQueues = calloc(PCB_MAXPRIOR + 1, sizeof(struct pcbqueue));
			assert(pqrQueue->apqQueues != NULL);
			for(i = 0; i <= PCB_MAXPRIOR; i++) {
				/* Initialize each queue. */
				pqrQueue->apqQueues[i] = makepcbqueue();
			}

			return (struct pcbqueue *)pqrQueue;
		}
	default:
		/* Shouldn't happen. */
		assert(0);
	}
}

/* Deinitialize/deallocate PCB queue. */
void killpcbqueue(struct pcbqueue *pqQueue) {
	switch(pqQueue->type) {
	case QT_NORMAL:
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
		break;
	case QT_PRIORITY:
		{
			/* Queue loop idx. */
			int i;
			/* Cast queue. */
			struct pcbqueueprior *pqrQueue;

			pqrQueue = (struct pcbqueueprior *)pqQueue;
			/* Free each queue. */
			for(i = 0; i <= PCB_MAXPRIOR; i++)
				killpcbqueue(pqrQueue->apqQueues[i]);
			/* Free array. */
			free(pqrQueue->apqQueues);
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Free the queue. */
	free(pqQueue);
}

/* Do something for every PCB in a queue. */
void foreachpcb(struct pcbqueue *pqQueue, void(*pcbitr)(struct pcb *, void *), void *arg) {
	switch(pqQueue->type) {
	case QT_NORMAL:
		{
			/* ID of the initial PCB. */
			int fid;

			/* Current PCB. */
			struct pcb *pPCB;

			/* Skip empty queues. */
			if(pqQueue->pHead == NULL) return;

			/* Initialize iteration. */
			pPCB = pqQueue->pHead;
			fid  = pPCB->id;

			do {
				pcbitr(pPCB, arg);

				pPCB = pPCB->pNext;
			} while(pPCB->id != fid);
		}
		break;
	case QT_PRIORITY:
		{
			/* Loop idx for queues. */
			int i;
			/* Cast queue. */
			struct pcbqueueprior *pqrQueue;

			pqrQueue = (struct pcbqueueprior *)pqQueue;
			for(i = PCB_MAXPRIOR; i >= 0; i--) {
				foreachpcb(pqrQueue->apqQueues[i], pcbitr, arg);
			}
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}
}

/* Remove the first PCB in a queue. */
struct pcb *poppcb(struct pcbstate *pState, struct pcbqueue *pqQueue) {
	switch(pqQueue->type) {
	case QT_NORMAL:
		{
			/* The removed PCB. */
			struct pcb *pPCB;

			/* Don't remove NULL PCBs. */
			if(pqQueue->pHead == NULL) {
				return NULL;
			}

			/* Remove the head PCB. */
			pPCB = pqQueue->pHead;
			removepcb(pState, pPCB);

			return pPCB;
		}
		break;
	case QT_PRIORITY:
		{
			/* The cast queue. */
			struct pcbqueueprior *pqrQueue;
			/* The loop index. */
			int i;

			/* Cast the queue. */
			pqrQueue = (struct pcbqueueprior *)pqQueue;
			/*
			 * @NOTE
			 * 	Which way did PCB priorities go again?
			 */
			for(i = PCB_MAXPRIOR; i >= 0; i--) {
				/* The PCB to hand back. */
				struct pcb *pPCB;

				pPCB = poppcb(pState, pqrQueue->apqQueues[i]);

				if(pPCB != NULL) return pPCB;
			}

			/* We didn't find a PCB. */
			return NULL;
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}
}

/* Allocate/initialize PCB state. */
struct pcbstate *makepcbstate() {
	/* Allocate the state, and fail if allocation fails. */
	struct pcbstate *pState = malloc(sizeof(struct pcbstate));
	assert(pState != NULL);

	/* Setup name table. */
	pState->ptPCBNames = makeinterntab();
	/* Setup image table. */
	pState->ptPCBImages = makeinterntab();
	/* Initialize proc. ids. */
	pState->nProcid    = 1;

	/* Setup queues. */
	pState->pqReady    = maketypedpcbqueue(QT_PRIORITY);
	pState->pqBlocked  = makepcbqueue();
	pState->pqsReady   = maketypedpcbqueue(QT_PRIORITY);
	pState->pqsBlocked = makepcbqueue();

	return pState;
}

/* Deinitialize/deallocate PCB state. */
void killpcbstate(struct pcbstate *pState) {
	/* Free associated queues. */
	killpcbqueue(pState->pqReady);
	killpcbqueue(pState->pqBlocked);
	killpcbqueue(pState->pqsReady);
	killpcbqueue(pState->pqsBlocked);

	/* Free interned names. */
	killinterntab(pState->ptPCBNames);

	/* Free state. */
	free(pState);
}

