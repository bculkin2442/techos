#ifndef TECHOS_OSSTATEH
#define TECHOS_OSSTATEH

#include <stdio.h>

#include <time.h>

#define MAX_FMT_SIZE 256

/* Default date formats. */
extern char *defin_datefmt;
extern char *deftime_datefmt;
extern char *defout_datefmt;

/* Forward-declare of command state declared elsewhere. */
struct commandstate;

/* General structure for OS-level state. */
/* @TODO Split the seperate system states into seperate structs */
struct osstate {
	/* Time/date variables. */
	/* @TODO Split into 'struct datestate'.*/
	/* Input/output/time date formats. */
	char *in_datefmt;
	char *out_datefmt;
	char *time_datefmt;
	/* Current time/date. */
	struct tm *datetime;
	
	/* Input/output streams. */
	/*
	 * @TODO rename strem; split command/user input; split error/normal output.
	 *       split into 'struct iostate'.
	 */
	/* The stream we read input from. */
	FILE *strem;
	/* The stream we output to. */
	FILE *output;

	/* Process control state. */
	struct pcbstate *pPCBstat;
	
	/* Command state. */
	struct commandstate *pComstate;
};

/* Create/delete an osstate. */
struct osstate *makeosstate();
void             killosstate(struct osstate *);
#endif
