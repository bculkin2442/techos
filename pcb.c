#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libs/intern.h"

#include "osstate.h"

#include "pcb.h"
#include "pcbinternals.h"

/* Allocate and initialize a PCB. */
struct pcb *makepcb(struct pcbstate *pState, char *pszPCBName, char *pszPCBImage, enum pcbclass clas, int prior) {
	/* The PCB to return. */
	struct pcb *pPCB;

	/* Make sure priorities are valid. */
	if(prior < PCB_MINPRIOR || prior > PCB_MAXPRIOR) return NULL;

	/* Allocate a PCB and fail if allocation does. */
	pPCB = malloc(sizeof(struct pcb));
	assert(pPCB != NULL);

	/* Set PCB ID. */
	pPCB->id    = pState->nProcid++;
	/* Intern the string for the PCB name. */
	pPCB->kName = internstring(pState->ptPCBNames, pszPCBName);

	/* Initialize passed in properties. */
	pPCB->clas     = clas;
	pPCB->priority = prior;

	/* Initialize PCB status. */
	pPCB->status = PCB_READY;
	pPCB->susp   = PCB_FREE;

	/* Initialize PCB queue. */
	pPCB->pNext = pPCB;
	pPCB->pPrev = pPCB;

	/* Setup process data. */
	pPCB->kImage = internstring(pState->ptPCBImages, pszPCBImage);
	pPCB->offset = 0;

	return pPCB;
}

/* Deinitialize and deallocate a PCB. */
void killpcb(struct pcb * pPCB) {
	free(pPCB);
}

/* Find something by number in a PCB queue. */
static struct pcb *queuefindpcbnum(struct pcbqueue *pqQueue, int pcbnum) {
	switch(pqQueue->type) {
	case QT_NORMAL:
		{
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
	case QT_PRIORITY:
		{
			/* Loop index for queues. */
			int i;
			/* Cast queue. */
			struct pcbqueueprior *pqrQueue;

			pqrQueue = (struct pcbqueueprior *)pqQueue;
			for(i = 0; i <= PCB_MAXPRIOR; i++) {
				/* The PCB we possibly found. */
				struct pcb *pPCB;

				/* Find in each priority level. */
				pPCB = queuefindpcbnum(pqrQueue->apqQueues[i], pcbnum);
				if(pPCB != NULL) return pPCB;
			}
			
			/* We found nothing. */
			return NULL;
		}
	default:
		/* Shouldn't happen. */
		assert(0);
	}
}

/* Find something by name in a PCB queue. */
static struct pcb *queuefindpcbname(struct pcbqueue *pqQueue, int kPCBName) {
	switch(pqQueue->type) {
	case QT_NORMAL:
		{
			/* The number of the first PCB. */
			int initPCB;
			/* The current PCB to search. */
			struct pcb *pPCB;

			/* Initialize iteration. */
			pPCB    = pqQueue->pHead;

			if(pPCB == NULL) {
				/* Empty queue. */
				return 0;
			} else {
				initPCB = pPCB->id;
			}

			do {
				/* Return the PCB if it matches. */
				if(pPCB->kName == kPCBName) return pPCB;
				/* Advance to the next PCB in the queue. */
				pPCB = pPCB->pNext;
			} while(pPCB->id != initPCB);

			return NULL;
		}
	case QT_PRIORITY:
		{
			/* Loop index for queues. */
			int i;
			/* Cast queue. */
			struct pcbqueueprior *pqrQueue;

			pqrQueue = (struct pcbqueueprior *)pqQueue;
			for(i = 0; i <= PCB_MAXPRIOR; i++) {
				/* The PCB we possibly found. */
				struct pcb *pPCB;

				/* Find in each priority level. */
				pPCB = queuefindpcbname(pqrQueue->apqQueues[i], kPCBName);
				if(pPCB != NULL) return pPCB;
			}
			
			/* We found nothing. */
			return NULL;
		}
	default:
		/* Shouldn't happen. */
		assert(0);
	}
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

	/* Only look in non-empty queues. */
	if(pState->pqReady->pHead != NULL)
		pPCB = queuefindpcbname(pState->pqReady, kPCBName);
	if(pState->pqBlocked->pHead != NULL) 
		if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqBlocked, kPCBName);
	if(pState->pqsReady->pHead != NULL) 
		if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqsReady, kPCBName);
	if(pState->pqsBlocked->pHead != NULL) 
		if(pPCB == NULL) pPCB = queuefindpcbname(pState->pqsBlocked, kPCBName);

	return pPCB;
}

/* Fill an queue if it is empty. */
static int fillqueue(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Only valid on normal queues. */
	assert(pqQueue->type == QT_NORMAL);

	if(pqQueue->pHead == NULL) {
		pqQueue->pHead = pPCB;
		return 1;
	}

	return 0;
}

/* Insert a PCB into a FIFO queue. */
static void fifoinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Only valid on normal queues. */
	assert(pqQueue->type == QT_NORMAL);

	/* The current head of the chain. */
	struct pcb *pHead;
	/* The current tail of the chain. */
	struct pcb *pTail;

	/* Fill an queue if it is empty. */
	if(fillqueue(pqQueue, pPCB)) return;

	pHead = pqQueue->pHead;
	pTail = pHead->pPrev;

	/* Chain the PCB to the back. */
	pPCB->pNext = pHead;
	pPCB->pPrev = pTail;

	/* Adjust the queue members. */
	pTail->pNext = pPCB;
	pHead->pPrev = pPCB;
}

/* Insert a PCB into a FILO queue. */
static void filoinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Only valid on normal queues. */
	assert(pqQueue->type == QT_NORMAL);

	/* Fill an queue if it is empty. */
	if(fillqueue(pqQueue, pPCB)) return;

	/* Chain the PCB to the queue. */
	fifoinsertpcb(pqQueue, pPCB);

	/* Set the new head. */
	pqQueue->pHead = pPCB;
}

/* Insert a PCB into a priority queue. */
static void priorinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	/* Cast queue. */
	struct pcbqueueprior *pqrQueue;

	/* Only valid on priority queues. */
	assert(pqQueue->type == QT_PRIORITY);

	pqrQueue = (struct pcbqueueprior *)pqQueue;

	/* Insert it into the proper queue. */
	fifoinsertpcb(pqrQueue->apqQueues[pPCB->priority], pPCB);
	pqrQueue->apqQueues[pPCB->priority]->nprocs += 1;
}

void doinsertpcb(struct pcbqueue *pqQueue, struct pcb *pPCB) {
	switch(pqQueue->type) {
	case QT_NORMAL:
		fifoinsertpcb(pqQueue, pPCB);
		break;
	case QT_PRIORITY:
		priorinsertpcb(pqQueue, pPCB);
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}
}
/* Insert a PCB into the proper queue. */
enum pcberror insertpcb(struct pcbstate *pState, struct pcb *pPCB) {
	/* The two queues to select from. */
	struct pcbqueue *pqReady;
	struct pcbqueue *pqBlocked;

	/* Don't insert a PCB that is already in a queue. */
	if(pPCB->pNext != pPCB || pPCB->pPrev != pPCB) return PCBINQUEUE;

	switch(pPCB->susp) {
	case PCB_FREE:
		pqReady   = pState->pqReady;
		pqBlocked = pState->pqBlocked;
		break;
	case PCB_SUSPENDED:
		pqReady   = pState->pqsReady;
		pqBlocked = pState->pqsBlocked;
		break;
	default:
		return PCBINVSUSP;
	}

	switch(pPCB->status) {
	case PCB_READY:
		doinsertpcb(pqReady, pPCB);
		pqReady->nprocs += 1;
		break;
	case PCB_BLOCKED:
		doinsertpcb(pqBlocked, pPCB);
		pqBlocked->nprocs += 1;
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
		pqQueue = pState->pqsReady;
	} else if(pPCB->susp == PCB_SUSPENDED && pPCB->status == PCB_BLOCKED) {
		pqQueue = pState->pqsBlocked;
	} else if(pPCB->susp == PCB_FREE && pPCB->status == PCB_READY) {
		pqQueue = pState->pqReady;
	} else if(pPCB->susp == PCB_FREE && pPCB->status == PCB_BLOCKED) {
		pqQueue = pState->pqBlocked;
	} else {
		printf("BANG! Attempted to remove a PCB not in a queue.\n");
		assert(0);
	}

	/* Save the links to restore. */
	pNext = pPCB->pNext;
	pPrev = pPCB->pPrev;

	/* Remove the PCB from the queue. */
	pPCB->pNext = pPCB;
	pPCB->pPrev = pPCB;

	/* Set the proper queue. */
	switch(pqQueue->type) {
	case QT_NORMAL:
		break;
	case QT_PRIORITY:
		pqQueue->nprocs -= 1;
		pqQueue = ((struct pcbqueueprior *)pqQueue)->apqQueues[pPCB->priority];
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	pqQueue->nprocs -= 1;

	if(pqQueue->nprocs < 1) {
		/* Empty a queue. */
		pqQueue->pHead = NULL;
	} else {
		/* Restore the queue links. */
		pNext->pPrev = pPrev;
		pPrev->pNext = pNext;

		if(pqQueue->pHead == pPCB) {
			/* Reset queue head. */
			pqQueue->pHead = pNext;
		}
	}
}

/* Print a PCB. */
void printpcb(struct pcb *pPCB, void *pvState) {
	/* The OS state. */
	struct osstate *ostate;

	/* The name of the PCB. */
	const char *pszPCBName;
	/* The class of the PCB. */
	const char *pszPCBClass;
	/* The status of the PCB. */
	const char *pszPCBStatus;
	/* The suspension status of the PCB. */
	const char *pszPCBSusp;

	/* Cast arg. */
	ostate = (struct osstate *)(pvState);

	/* Get the PCB name. */
	pszPCBName = lookupkey(ostate->pPCBstat->ptPCBNames, pPCB->kName);

	/* Get the PCB class. */
	switch(pPCB->clas) {
	case PCB_SYSTEM:
		pszPCBClass = "System";
		break;
	case PCB_APPLICATION:
		pszPCBClass = "Application";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Get the PCB status. */
	switch(pPCB->status) {
	case PCB_READY:
		pszPCBStatus = "Ready";
		break;
	case PCB_RUNNING:
		pszPCBStatus = "Running";
		break;
	case PCB_BLOCKED:
		pszPCBStatus = "Blocked";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Get the PCB suspension status. */
	switch(pPCB->susp) {
	case PCB_SUSPENDED:
		pszPCBSusp = "Yes";
		break;
	case PCB_FREE:
		pszPCBSusp = "No";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Print basic PCB information. */
	fprintf(ostate->output, "PCB ID:           %d\n", pPCB->id);
	fprintf(ostate->output, "PCB Name:         %s\n", pszPCBName);
	fprintf(ostate->output, "PCB Class:        %s\n", pszPCBClass);
	fprintf(ostate->output, "PCB Priority:     %d\n", pPCB->priority);

	/* Print PCB status information. */
	fprintf(ostate->output, "PCB Status:       %s\n", pszPCBStatus);
	fprintf(ostate->output, "Is PCB Suspended: %s\n\n", pszPCBSusp);
}

/* Check if there are any processes to dispatch. */
int candispatch(struct pcbstate *pState) {
	if(pState->pqReady->nprocs > 0 || pState->pqBlocked->nprocs > 0) {
		return pState->pqReady->nprocs + pState->pqBlocked->nprocs;
	}

	return 0;
}

/* Get a dispatchable process. */
struct pcb *getdisppcb(struct pcbstate *pState) {
	if(pState->pqReady->nprocs > 0 && pState->pqBlocked->nprocs > 0) {
		if(random() > .5) return poppcb(pState, pState->pqReady);
		else              return poppcb(pState, pState->pqBlocked);
	} else if(pState->pqReady->nprocs > 0) {
		return poppcb(pState, pState->pqReady);
	} else if(pState->pqBlocked->nprocs > 0) {
		return poppcb(pState, pState->pqBlocked);
	} else {
		return NULL;
	}
}
