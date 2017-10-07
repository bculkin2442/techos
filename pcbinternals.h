#ifndef TECHOS_PCBINTERNH
#define TECHOS_PCBINTERNH

/* Internal PCB structures. */

/*
 * Represents the types of PCB queue that exist.
 */
enum queuetype {
	/* 
	 * A normal, FIFO PCB queue.
	 *
	 * Represented by a struct pcbqueue.
	 */
	QT_NORMAL,
	/*
	 * A priority PCB queue.
	 *
	 * Represented by a struct pcbqueueprior.
	 */
	QT_PRIORITY
};

/* Represents a circular queue of PCBs. */
struct pcbqueue {
	/*
	 * Represents the type of queue this is.
	 */
	enum queuetype type;

	/* # of processes in this queue. */
	int nprocs;

	/* The first process in this queue. */
	struct pcb *pHead;
};

/* A priority queue of PCBs. */
struct pcbqueueprior {
	/*
	 * Represents the type of queue this is.
	 */
	enum queuetype type;

	/* # of processes in this queue. */
	int nprocs;

	/* 
	 * The list of queues for each priority.
	 */
	struct pcbqueue **apqQueues; 	
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

/* Allocate/initialize normal (QT_NORMAL) PCB queue. */
struct pcbqueue *makepcbqueue();
/* Allocate/initialize PCB queue of a specific type. */
struct pcbqueue *maketypedpcbqueue(enum queuetype);

/* 
 * Deinitialize/deallocate PCB queue.
 *
 * NOTE: Killing a PCB queue kills all PCBs currently enqueued.
 */
void killpcbqueue(struct pcbqueue *);

/* Allocate/initialize a PCB state. */
struct pcbstate *makepcbstate();
/* Deinitialize/deallocate a PCB state. */
void             killpcbstate(struct pcbstate *);
#endif
