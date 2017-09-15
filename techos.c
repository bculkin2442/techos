#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libs/argparser.h"

#include "osstate.h"
#include "commands.h"
#include "techos.h"

/*
 * Main function. 
 *
 * Print out a greeting, then enter the command loop.
 *
 * After that's done, say goodbye.
 */
int main() {
	/* OS state. */
	struct osstate *ostate;

	/* Initialize commands. */
	initcoms();
	/* Init. OS state. */
	ostate         = makeosstate();
	ostate->strem  = stdin;
	ostate->output = stdout;

	fprintf(ostate->output, "Welcome to TechOS v%d.%d\n", major_ver, minor_ver);

	/* Handle commands. */
	comhan(ostate);

	fprintf(ostate->output, "Goodbye\n");

	/* Cleanup after ourselves. */
	killosstate(ostate);
	disposecoms();
}

/* Command loop. Read & execute commands from the user. */
void comhan(struct osstate *ostate) {
	/* The number of commands executed so far. */
	int lno = 0;

	/* Variables for line input. */
	size_t  lread = 0;
	size_t  lsize = 0;
	char   *line  = NULL;

	/* Initial prompt. */
	fprintf(ostate->output, "TechOS(%d)>", lno);

	/* Loop until we don't read anything. */
	while((lread = getline(&line, &lsize, ostate->strem)) > 0) {
		/* Exit the command loop if we've read EOF. */
		if(feof(ostate->strem) != 0) goto cleanup;

		lno += 1;

		/* Handle the line, and exit if a command failed */
		if(handleline(ostate, line) < 0) break;

		/* Reprompt. */
		fprintf(ostate->output, "TechOS(%d)>", lno);
	}

	/* Cleanup after ourselves. */
cleanup: if(line != NULL)
		free(line);
}

/* Handle executing a line of input. */
int handleline(struct osstate *ostate, char *line) {
	/* Actual length of read string. */
	size_t llen;

	/* The command line arguments. */
	struct cliargs args;

	/* A copy of the line. */
	char *newline;
	newline = strdup(line);

	/* Remove the trailing newline from the command. */
	llen = strlen(line);
	if(line[llen-1] == '\n')
		line[llen-1] = '\0';

	args = parseargs(line);
	if(args.argv[0] != NULL) {
		/* The command to execute. */
		struct command com;

		/* Determine the command to execute from the name. */
		com = parsecom(args.argv[0], ostate);

		/* Execute the command if we have one. */
		if(com.comfun != NULL) {
			/* The return status of the command. */
			int comres;

			/*
			 * Execute the command.
			 *
			 * The status should be one of: 
			 * - zero for executing succesfully
			 * - positive for non-fatal errors
			 * - negative for fatal errors.
			 */
			comres = execcom(com, args, newline, ostate);

			free(newline);

			return comres;
		} else {
			/* No such command exists. */
			fprintf(ostate->output, "\tERROR: No such command named '%s'\n", args.argv[0]);

			free(newline);

			return 1;
		}
	} else {
		fprintf(ostate->output, "\tERROR: Couldn't find command name in line '%s'\n", line);

		free(newline);

		return 1;
	}
}
/* Get a command from its name. */
struct command parsecom(char *name, struct osstate *ostate) {
	int i;

	/*
	 * Go through all the commands.
	 *
	 * If one matches, return it.
	 *
	 * @TODO replace this
	 */
	for(i = 0; i < NUM_COMMANDS; i++) {
		struct command com = commands[i];

		if(strcmp(com.name, name) == 0) {
			return com;
		}
	}

	/* INVALID_COMMAND is the last command. */
	return INVALID_COMMAND;
}

/* Execute a command, plus any arguments it has. */
int execcom(struct command com, struct cliargs args, char *argline, struct osstate *ostate) {
	/* Return status of commands. */
	int comret;

	/* Execute the command. */
	comret = com.comfun(args.argc, args.argv, argline, ostate);

	return comret;
}
