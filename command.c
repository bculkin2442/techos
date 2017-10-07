#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osstate.h"
#include "command.h"

/* Check if a command that only takes the 'help' argument recieved it. */
int checkhelpargs(int argc, char **argv, char *usage, struct osstate *ostate) {
	if(argc > 2 || (strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)) {
		fprintf(ostate->output, "\tERROR: Invalid command-line arguments.\n");
	}

	fprintf(ostate->output, "%s", usage);

	return 0;
}

/* Allocate/initialize a command. */
struct command *makecommand(char *pszName, char *pszBrief, comfun_t pfHandler) {
	/* Allocate a command and fail if it does. */
	struct command *pCommand = malloc(sizeof(struct command));
	assert(pCommand != NULL);

	/* Initialize command. */
	pCommand->type   = CT_NORMAL;
	pCommand->name   = pszName;
	pCommand->brief  = pszBrief;
	pCommand->comfun = pfHandler;

	return pCommand;
}

/* Deinitialize/deallocate a command. */
void killcommand(struct command *pCommand) {
	free(pCommand->name);
	free(pCommand->brief);
	free(pCommand);
}
