#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "osstate.h"

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
	assert(ostate->in_datefmt != NULL);
	ostate->out_datefmt  = malloc(MAX_FMT_SIZE);
	assert(ostate->out_datefmt != NULL);
	ostate->time_datefmt = malloc(MAX_FMT_SIZE);
	assert(ostate->time_datefmt != NULL);

	sprintf(ostate->in_datefmt,   "%.256s", defin_datefmt);
	sprintf(ostate->time_datefmt, "%.256s", deftime_datefmt);
	sprintf(ostate->out_datefmt,  "%.256s", defout_datefmt);

	/* Get current date/time. */
	clocktime = time(NULL);
	ostate->datetime = localtime(&clocktime);

	return ostate;
}

/* Free/destroy OS state. */
void killosstate(struct osstate *state) {
	free(state->in_datefmt);
	free(state->time_datefmt);
	free(state->out_datefmt);

	free(state);
}
