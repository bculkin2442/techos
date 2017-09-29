#ifndef TECHOS_OSSTATEH
#define TECHOS_OSSTATEH

#include <stdio.h>

#include <time.h>

#include "pcb.h"

#define MAX_FMT_SIZE 256

/* Default date formats. */
extern char *defin_datefmt;
extern char *deftime_datefmt;
extern char *defout_datefmt;

/* General structure for OS-level state. */
/* @TODO do we want to split the seperate system states into seperate structs? */
struct osstate {
	/* Time/date variables. */
	/* Input/output/time date formats. */
	char *in_datefmt;
	char *out_datefmt;
	char *time_datefmt;
	/* Current time/date. */
	struct tm *datetime;
	
	/* Input/output streams. */
	/*
	 * @TODO rename strem; split command/user input; split error/normal output.
	 */
	/* The stream we read input from. */
	FILE *strem;
	/* The stream we output to. */
	FILE *output;

	/* Process Control Queues. */
	struct pcbstate {
		/* Ready PCBs. */
		struct pcbqueue *qReady;
		/* Blocked PCBs. */
		struct pcbqueue *qBlocked;

		/* Suspended Ready PCBs. */
		struct pcbqueue *qsReady;
		/* Suspended Blocked PCBs. */
		struct pcbqueue *qsBlocked;
	} pcbstat;
};

/* Create/delete an osstate. */
struct osstate *makeosstate();
void             killosstate(struct osstate *);
#endif
