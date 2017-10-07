#include <assert.h>
#include <stdlib.h>

#include "comlist.h"
#include "commandstate.h"

struct commandstate *makecommandstate() {
	/* Allocate command state, and fail if it does. */
	struct commandstate *pComstate = malloc(sizeof(struct commandstate));

	pComstate->plCommands = makecomlist();

	return pComstate;
}

void killcommandstate(struct commandstate *pComstate) {
	killcomlist(pComstate->plCommands);

	free(pComstate);
}
