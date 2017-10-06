#ifndef TECHOS_PCBINTERNH
#define TECHOS_PCBINTERNH

/* Internal PCB structures. */

/* Represents a circular queue of PCBs. */
struct pcbqueue {
	/* # of processes in this queue. */
	int nprocs;

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

/* Execute an action for all of the PCBs in a queue. */
void foreachpcb(struct pcbqueue *, void (*pcbitr)(struct pcb *, void *), void *);

/* Allocate/initialize PCB queue. */
struct pcbqueue *makepcbqueue();
/* 
 * Deinitialize/deallocate PCB queue.
 *
 * NOTE: Killing a PCB queue kills all PCBs currently enqueued.
 */
void             killpcbqueue(struct pcbqueue *);
#endif
