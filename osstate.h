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
/* @TODO 10/07/17 Ben Culkin :OSStateRestructure
 * 	Split the seperate system states into seperate structs. This is so that
 * 	various systems can be made aware of only the state parts they need to,
 * 	as well as allowing various instances to be swapped around using
 * 	commands so as to handle 'user profiles' or something along those lines.
 */
struct osstate {
	/* :OSStateRestructure
	 * 	This will go into a 'struct datestate' or something along those
	 * 	lines.
	 */
	/* Time/date variables. */
	/* Input/output/time date formats. */
	char *in_datefmt;
	char *out_datefmt;
	char *time_datefmt;
	/* Current time/date. */
	struct tm *datetime;
	
	/* :OSStateRestructure
	 * 	This will go into a 'struct iostate' or something like that.
	 */
	/* Input/output streams. */
	/*
	 * @TODO 10/07/17 Ben Culkin :IORefactor
	 * 	Do the following things to cleanup the way I/O is handled:
	 * 		- Rename strem to something else
	 * 		- Split command/user input in someway. Not sure of the
	 * 			best way to go about this; having two seperate
	 * 			file handles that may refer to the same thing
	 * 			might be the best solution.
	 * 		- Split error/normal output the same way command/user
	 * 			input is split.
	 * 		- Put I/O into functions. Create various functions to do
	 * 			things like print 'ERROR' and 'INTERNAL ERROR'
	 * 			messages with a consistent format. Also,
	 * 			consider doing something similiar with input,
	 * 			but none come to mind for it.
	 */
	/* The stream we read input from. */
	FILE *strem;
	/* The stream we output to. */
	FILE *output;

	/* Process control state. */
	struct pcbstate *pPCBstat;
	
	/* Command state. */
	struct commandstate *pComstate;

	/* Base working directory for the OS. */
	int fWorkingDir;

	/* Currently logged in user. */
	struct user *puCurrent;
};

/* Create/delete an osstate. */
struct osstate *makeosstate();
void             killosstate(struct osstate *);
#endif
