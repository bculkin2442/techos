#include <assert.h>
#include <stdlib.h>

#include "libs/intern.h"

#include "pcb.h"

/* Represents a circular queue of PCBs. */
struct pcbqueue {
	/* # of processes in this queue. */
	int noprocs;

	/* The first process in this queue. */
	struct pcb *pHead;
};

/* Contains all of the PCB queues. */
struct pcbstate {
	/* PCB name table. */
	struct interntab *ptPCBNames;
	/* Next process ID. */
	int nProcid;

	/* Ready PCBs. */
	struct pcbqueue *pqReady;
	/* Blocked PCBs. */
	struct pcbqueue *pqBlocked;

	/* Suspended Ready PCBs. */
	struct pcbqueue *pqsReady;
	/* Suspended Blocked PCBs. */
	struct pcbqueue *pqsBlocked;
};

/* Allocate and initialize a PCB. */
struct pcb *makepcb(struct pcbstate *pState, char *pszPCBName, enum pcbclass clas, int prior) {
	/* The PCB to return. */
	struct pcb *pPCB;

	/* Allocate a PCB and fail if allocation does. */
	pPCB = malloc(sizeof(struct pcb));
	assert(pPCB != NULL);

	/* Intern the string for the PCB name. */
	pPCB->kName    = internstring(pState->ptPCBNames, pszPCBName);
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
static struct pcb *queuefindpcbnum(struct pcbqueue *pQueue, int pcbnum) {
	/* The number of the first PCB. */
	int initPCB;
	/* The current PCB to search. */
	struct pcb *pPCB;

	/* Initialize iteration. */
	pPCB    = pQueue->pHead;
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
static struct pcb *queuefindpcbname(struct pcbqueue *pQueue, int kPCBName) {
	/* The number of the first PCB. */
	int initPCB;
	/* The current PCB to search. */
	struct pcb *pPCB;

	/* Initialize iteration. */
	pPCB    = pQueue->pHead;
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
