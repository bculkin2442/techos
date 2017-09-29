#ifndef TECHOS_PCBH
#define TECHOS_PCBH

#include "libs/intern.h"

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
	internkey     name;
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
	struct pcb *next;
	/* The previous process in whatever queue this PCB is in. */
	struct pcb *prev;
};

/* Represents a circular queue of PCBs. */
struct pcbqueue {
	/* # of processes in this queue. */
	int noprocs;

	/* The first process in this queue. */
	struct pcb *head;
};
#endif
