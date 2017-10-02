#include <stdio.h>
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
