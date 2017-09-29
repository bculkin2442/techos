#ifndef TECHOS_PCBH
#define TECHOS_PCBH

#include "libs/intern.h"

#define PCB_MINPRIOR 0
#define PCB_MAXPRIOR 9

/* Represents the classification of a PCB. */
enum pcbclass {
	/* A system process. */
	PCB_SYSTEM,
	/* An application process. */
	PCB_APPLICATION,
};

/* PCB run status. */
enum pcbstatus {
	/* The process is blocked. */
	PCB_BLOCKED,
	/* The process is ready to run. */
	PCB_READY,
	/* The process is running. */
	PCB_RUNNING,
};

/* PCB Suspension status. */
enum pcbsusp {
	/* The process is suspended. */
	PCB_SUSPENDED,
	/* The process is not suspended. */
	PCB_FREE,
};

/* Represents a running process. */
struct pcb {
	/* 
	 * Process ID.
	 *
	 * Unique identifier for the processes.
	 */
	int id;
	/* 
	 * Process name.
	 *
	 * Stored as a interned string, for fast comparisons.
	 */
	internkey     kName;
	/* Process class. */
	enum pcbclass clas;
	/* Process priority. */
	int           priority;

	/* PCB status indicators. */
	/* Whether the PCB is ready, running or blocked. */
	enum pcbstatus status;
	/* Whether the PCB is suspended or not. */
	enum pcbsusp   susp;

	/* The next process in whatever queue this PCB is in. */
	struct pcb *pNext;
	/* The previous process in whatever queue this PCB is in. */
	struct pcb *pPrev;
};

/* Represents a circular queue of PCBs. */
struct pcbqueue;

/* Contains all of the PCB queues. */
struct pcbstate;

/* Allocate a new PCB. */
struct pcb *makepcb(struct pcbstate *, char *, enum pcbclass, int);
/* Deallocate a PCB. */
void        killpcb(struct pcb *);

/*
 * Find a PCB by its process number in a set of queues. 
 *
 * Returns NULL if the PCB wasn't found.
 */
struct pcb *findpcbnum(struct pcbstate *, int);
/*
 * Find a PCB by its process name in a set of queues.
 *
 * Returns NULL if the PCB wasn't found.
 *
 * WARNING: Will only return the first PCB by a given name, searching the
 * queues in the following order:
 *	1. Ready
 * 	2. Blocked
 *	3. Suspended Ready
 *	4. Suspended Blocked.
 */
struct pcb *findpcbname(struct pcbstate *, char *);

/* Insert a PCB into the proper queue. */
void insertpcb(struct pcbstate *, struct pcb *);
/* Remove a PCB from the queue it is in. */
void removepcb(struct pcbstate *, struct pcb *);
#endif