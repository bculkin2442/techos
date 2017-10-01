#include <assert.h>
#include <stdlib.h>

#include "libs/intern.h"

#include "pcb.h"
#include "pcbinternals.h"

/* Allocate and initialize a PCB. */
struct pcb *makepcb(struct pcbstate *pState, char *pszPCBName, enum pcbclass clas, int prior) {
	/* The PCB to return. */
	struct pcb *pPCB;

	/* Make sure priorities are valid. */
	if(prior < PCB_MINPRIOR || prior > PCB_MAXPRIOR) return NULL;

	/* Allocate a PCB and fail if allocation does. */
	pPCB = malloc(sizeof(struct pcb));
	assert(pPCB != NULL);

	/* Set PCB ID. */
	pPCB->id = pState->nProcid++;
	/* Intern the string for the PCB name. */
	pPCB->kName    = internstring(pState->ptPCBNames, pszPCBName);

	/* Initialize passed in properties. */
	pPCB->clas     = clas;
	pPCB->priority = prior;

	/* Initialize PCB status. */
	pPCB->status = PCB_READY;
	pPCB->susp   = PCB_FREE;
	/* Initialize PCB queue. */
	pPCB->pNext = pPCB;
	pPCB->pPrev = pPCB;

	return pPCB;
}

/* Deinitialize and deallocate a PCB. */
void killpcb(struct pcb * pPCB) {
	free(pPCB);
}

/* Find something by number in a PCB queue. */
static struct pcb *queuefindpcbnum(struct pcbqueue *pqQueue, int pcbnum) {
	/* The number of the first PCB. */
	int initPCB;
	/* The current PCB to search. */
	struct pcb *pPCB;

	/* Initialize iteration. */
	pPCB    = pqQueue->pHead;
	initPCB = pPCB->id;

	do {
		/* Return the PCB if it matches. */
		if(pPCB->id == pcbnum) return pPCB;
		/* Advance to the next PCB in the queue. */
		pPCB = pPCB->pNext;
	} while(pPCB->id != initPCB);

	return NULL;
}

/* Find something by name in a PCB queue. */
static struct pcb *queuefindpcbname(struct pcbqueue *pqQueue, int kPCBName) {
	/* The number of the first PCB. */
	int initPCB;
	/* The current PCB to search. */
	struct pcb *pPCB;

	/* Initialize iteration. */
	pPCB    = pqQueue->pHead;
	initPCB = pPCB->id;

	do {
		/* Return the PCB if it matches. */
		if(pPCB->kName == kPCBName) return pPCB;
		/* Advance to the next PCB in the queue. */
		pPCB = pPCB->pNext;
	} while(pPCB->id != initPCB);

	return NULL;
}
/* Find a PCB by number in all of the queues. */
struct pcb *findpcbnum(struct pcbstate *pState, int pcbnum) {
	/* The PCB to return. */
	struct pcb *pPCB;

	/* Fail fast for numbers that don't exist. */
	if(pState->nProcid < pcbnum) return NULL;

	/* Search the queues in order. */
	pPCB                  = queuefindpcbnum(pState->pqReady,    pcbnum);
	if(pPCB == NULL) pPCB = queuefindpcbnum(pState->pqBlocked,  pcbnum);
	if(pPCB == NULL) pPCB = queuefindpcbnum(pState->pqsReady,   pcbnum);
	if(pPCB == NULL) pPCB = queuefindpcbnum(pState->pqsBlocked, pcbnum);

	return pPCB;
}

/* Find a PCB by name in all of the queues. */
struct pcb *findpcbname(struct pcbstate *pState, char *pszPCBName) {
	/* The PCB to return. */
	struct pcb *pPCB;
	/* The interned name. */
	int kPCBName;

	/* Lookup the PCB name. */
	kPCBName = lookupstring(pState->ptPCBNames, pszPCBName);
	if(kPCBName == SIINVALID) return NULL;

	pPCB                  = queuefindpcbname(pState->pqReady,    kPCBName);
	if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqBlocked,  kPCBName);
	if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqsReady,   kPCBName);
	if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqsBlocked, kPCBName);

	return pPCB;
}

/* Fill an queue if it is empty. */
static int fillqueue(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	if(pqQueue->pHead == NULL) {
		pqQueue->pHead = pPCB;
		return 1;
	}

	return 0;
}

/* Insert a PCB into a FIFO queue. */
void fifoinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Fill an queue if it is empty. */
	if(fillqueue(pqQueue, pPCB)) return;

	/* Chain the PCB to the back. */
	pPCB->pNext = pqQueue->pHead;
	pPCB->pPrev = pqQueue->pHead->pPrev;

	/* Adjust the queue members. */
	pqQueue->pHead->pPrev->pNext = pPCB;
	pqQueue->pHead->pPrev        = pPCB;
}

/* Insert a PCB into a FILO queue. */
void filoinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Fill an queue if it is empty. */
	if(fillqueue(pqQueue, pPCB)) return;

	/* Chain the PCB to the queue. */
	fifoinsertpcb(pqQueue, pPCB);

	/* Set the new head. */
	pqQueue->pHead = pPCB;
}

/* Insert a PCB into a priority queue. */
void priorinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Fill an queue if it is empty. */
	if(fillqueue(pqQueue, pPCB)) return;

	/* ID number of the first PCB. */
	int fid;

	/* Current member of the queue. */
	struct pcb *pCur;
	
	/* Initialize state. */
	pCur = pqQueue->pHead;
	fid  = pqQueue->pHead->id;
	
	/* Find a slot to insert it into. */
	/* @TODO ensure this works. */
	do {
		if(pPCB->priority > pCur->priority) {
			/* This is where we insert. */
			/* Chain the PCB to where it goes. */
			pPCB->pNext = pCur;
			pPCB->pPrev = pCur->pPrev;

			/* Adjust the queue members. */
			pCur->pPrev->pNext = pPCB;
			pCur->pPrev        = pPCB;

			/* Update the head if we need to. */
			if(pCur->id == fid) pqQueue->pHead = pPCB;

			return;
		}
	} while(pCur->id != fid);

	/* Insert it at the back then. */
	fifoinsertpcb(pqQueue, pPCB);
}

/* Insert a PCB into the proper queue. */
int insertpcb(struct pcbstate *pState, struct pcb *pPCB) {
	/* The two queues to select from. */
	struct pcbqueue *pqReady;
	struct pcbqueue *pqBlocked;

	switch(pPCB->susp) {
	case PCB_SUSPENDED:
		pqReady   = pState->pqReady;
		pqBlocked = pState->pqBlocked;
		break;
	case PCB_FREE:
		pqReady   = pState->pqsReady;
		pqBlocked = pState->pqsBlocked;
		break;
	default:
		return PCBINVSUSP;
	}

	switch(pPCB->status) {
	case PCB_BLOCKED:
		priorinsertpcb(pqReady, pPCB);
		pqReady->nprocs += 1;
		break;
	case PCB_READY:
		fifoinsertpcb(pqBlocked, pPCB);
		pqReady->nprocs += 1;
		break;
	case PCB_RUNNING:
		return PCBRUNNING;
	default:
		return PCBINVSTAT;
	}

	return 0;
}

/* Remove a PCB from a queue. */
void removepcb(struct pcbstate *pState, struct pcb *pPCB) {
	/* Temporary links that will be restored. */
	struct pcb *pPrev, *pNext;
	/* The queue this PCB came from. */
	struct pcbqueue *pqQueue;

	if(pPCB->susp == PCB_SUSPENDED && pPCB->status == PCB_READY) {
		pqQueue = pState->pqReady;
	} else if(pPCB->susp == PCB_SUSPENDED && pPCB->status == PCB_BLOCKED) {
		pqQueue = pState->pqBlocked;
	} else if(pPCB->susp == PCB_FREE && pPCB->status == PCB_READY) {
		pqQueue = pState->pqsReady;
	} else if(pPCB->susp == PCB_FREE && pPCB->status == PCB_BLOCKED) {
		pqQueue = pState->pqsBlocked;
	} else {
		/* Attempted to remove a PCB that wasn't in a queue. */
		assert(0);
	}

	pNext = pPCB->pNext;
	pPrev = pPCB->pPrev;

	if(pNext == pPrev) {
		/* Empty a queue. */
		pqQueue->pHead = NULL;
	} else {
		pNext->pPrev = pPrev;
		pPrev->pNext = pNext;
	}

	pqQueue->nprocs -= 1;
}
