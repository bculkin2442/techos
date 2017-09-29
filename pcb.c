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

	pPCB->status = PCB_READY;
	pPCB->susp   = PCB_FREE;

	pPCB->pNext = pPCB;
	pPCB->pPrev = pPCB;

	return pPCB;
}

/* Deinitialize and deallocate a PCB. */
void killpcb(struct pcb * pPCB) {
	free(pPCB);
}
