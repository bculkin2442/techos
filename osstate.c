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

	/* Set up default formats for date I/O. */
	ostate->in_datefmt   = malloc(256);
	ostate->out_datefmt  = malloc(256);
	ostate->time_datefmt = malloc(256);

	sprintf(ostate->in_datefmt,   "%s", defin_datefmt);
	sprintf(ostate->time_datefmt, "%s", deftime_datefmt);
	sprintf(ostate->out_datefmt,  "%s", defout_datefmt);

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
