#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "osstate.h"

#include "pcb.h"
#include "pcbinternals.h"

#include "commandstate.h"

/* Set default date formats */
char *defin_datefmt   = "%Y-%m-%d";
char *deftime_datefmt = "%r (%Z)";
char *defout_datefmt  = "%A, %d, %B, %Y";

/* Allocate/initialize OS state. */
struct osstate *makeosstate() {
	/* State to return. */
	struct osstate *ostate;

	/* Current time. */
	clock_t clocktime;

	ostate = malloc(sizeof(struct osstate));
	assert(ostate != NULL);

	/* Set up default formats for date I/O. */
	ostate->in_datefmt   = malloc(MAX_FMT_SIZE);
	ostate->out_datefmt  = malloc(MAX_FMT_SIZE);
	ostate->time_datefmt = malloc(MAX_FMT_SIZE);
	/* Fail if a memory allocation failed. */
	assert(ostate->in_datefmt != NULL);
	assert(ostate->out_datefmt != NULL);
	assert(ostate->time_datefmt != NULL);

	/* Set them to their default values. */
	sprintf(ostate->in_datefmt,   "%.256s", defin_datefmt);
	sprintf(ostate->time_datefmt, "%.256s", deftime_datefmt);
	sprintf(ostate->out_datefmt,  "%.256s", defout_datefmt);

	/* Get current date/time. */
	clocktime = time(NULL);
	ostate->datetime = localtime(&clocktime);

	/* Setup PCB state. */
	ostate->pPCBstat = makepcbstate();

	/* Setup command state. */
	ostate->pComstate = makecommandstate();

	/* Setup working directory. */
	ostate->fWorkingDir = open(".", O_PATH);

	return ostate;
}

/* Free/destroy OS state. */
void killosstate(struct osstate *ostate) {
	/* Free command state. */
	killcommandstate(ostate->pComstate);

	/* Free PCB state. */
	killpcbstate(ostate->pPCBstat);

	/* Free date/time vars. */
	free(ostate->in_datefmt);
	free(ostate->time_datefmt);
	free(ostate->out_datefmt);

	/* Free state. */
	free(ostate);
}
