#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libs/argparser.h"

#include "osstate.h"

#include "commands.h"
#include "comlist.h"
#include "commandstate.h"

#include "techos.h"

/* The major/minor version of TechOS. */
const int major_ver = 5;
const int minor_ver = 0;

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
	/* Add all the commands to the command list. */
	addcommands(ostate->pComstate->plCommands);

	fprintf(ostate->output, "Welcome to TechOS v%d.%d\n", major_ver, minor_ver);

	/* user login */
	loginuser(ostate);

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
	newline = (char *)strdup(line);

	/* Remove the trailing newline from the command. */
	llen = strlen(line);
	if(line[llen-1] == '\n')
		line[llen-1] = '\0';

	/* Parse CLI args. */
	args = parseargs(line);
	if(args.argv[0] != NULL) {
		/* The command to execute. */
		struct command *com;

		/* Determine the command to execute from the name. */
		/*
		 * @TODO 10/0/7/17 Ben Culkin :GetCommand
		 * 	May want to change this from using the general
		 * 	'getcommand' function (and that should probably be named
		 * 	something like getcommandfromlist to make what it takes
		 * 	clear), to using a local version so that it can handle
		 * 	things like aliases and other varying types of commands
		 * 	that aren't normal ones stored in the single list.
		 */
		com = getcommand(ostate->pComstate->plCommands, args.argv[0]);

		/* Execute the command if we have one. */
		if(com->comfun != NULL) {
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

/* Execute a command, plus any arguments it has. */
int execcom(struct command *com, struct cliargs args, char *argline, struct osstate *ostate) {
	/* Return status of commands. */
	int comret;
	/* Execute the command. */
	switch(com->type) {
	case CT_NORMAL:
		comret = com->comfun(args.argc, args.argv, argline, ostate);
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	return comret;
}

void loginuser(struct osstate *ostate) {
	char *pszUsername, *pszPassword;
	size_t llen, lread;

	pszUsername = NULL;
	pszPassword = NULL;

	llen = 0;

	while(1) {
		struct user *puUser;

		fprintf(ostate->output, "Username: ");
		lread = getline(&pszUsername, &llen, ostate->strem);

		while(lread <= 0) {
			fprintf(ostate->output, "ERROR: Must specify a username.\n");

			fprintf(ostate->output, "Username: ");
			lread = getline(&pszUsername, &llen, ostate->strem);
		}

		puUser = udblookup(ostate->pdUsers, pszUsername);
		if(puUser == NULL) {
			fprintf(ostate->output, "\tERROR: No user named '%s' exists\n", pszUsername);
			continue;
		}

		fprintf(ostate->output, "Password: ");
		lread = getline(&pszPassword, &llen, ostate->strem);

		while(lread <= 0) {
			fprintf(ostate->output, "ERROR: Must specify a password.\n");

			fprintf(ostate->output, "Password: ");
			lread = getline(&pszPassword, &llen, ostate->strem);
		}

		if(strcmp(puUser->pszPass, pszPassword) == 0) {
			fprintf(ostate->output, "Welcome '%s'!\n", pszUsername);

			ostate->puCurrent = puUser;
			break;
		}

		fprintf(ostate->output, "\tERROR: Incorrect password\n");
	}

	if(pszUsername != NULL)
		free(pszUsername);

	if(pszPassword != NULL)
		free(pszPassword);
}
