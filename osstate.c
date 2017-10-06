#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "osstate.h"

#include "pcb.h"
#include "pcbinternals.h"

/* Set default date formats */
char *defin_datefmt   = "%Y-%m-%d";
char *deftime_datefmt = "%r (%Z)";
char *defout_datefmt  = "%A, %d, %B, %Y";

/* Allocate/initialize PCB state. */
static struct pcbstate *makepcbstate() {
	/* Allocate the state, and fail if allocation fails. */
	struct pcbstate *pState = malloc(sizeof(struct pcbstate));
	assert(pState != NULL);

	/* Setup name table. */
	pState->ptPCBNames = makeinterntab();
	/* Initialize proc. ids. */
	pState->nProcid    = 1;

	/* Setup queues. */
	pState->pqReady    = makepcbqueue();
	pState->pqBlocked  = makepcbqueue();
	pState->pqsReady   = makepcbqueue();
	pState->pqsBlocked = makepcbqueue();

	return pState;
}

/* Deinitialize/deallocate PCB state. */
static void killpcbstate(struct pcbstate *pState) {
	/* Free associated queues. */
	killpcbqueue(pState->pqReady);
	killpcbqueue(pState->pqBlocked);
	killpcbqueue(pState->pqsReady);
	killpcbqueue(pState->pqsBlocked);

	/* Free interned names. */
	killinterntab(pState->ptPCBNames);

	/* Free state. */
	free(pState);
}

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

	return ostate;
}

/* Free/destroy OS state. */
void killosstate(struct osstate *state) {
	/* Free PCB state. */
	killpcbstate(state->pPCBstat);

	/* Free date/time vars. */
	free(state->in_datefmt);
	free(state->time_datefmt);
	free(state->out_datefmt);

	/* Free state. */
	free(state);
}
