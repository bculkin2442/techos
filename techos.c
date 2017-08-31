#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "techos.h"

/*
 * The stream we read input from.
 */
FILE *strem;

/*
 * Main function. Print out a greeting, then enter the command loop.
 * After that's done, say goodbye.
 */
int main() {
	printf("Welcome to TechOS v%d.%d\n", major_ver, minor_ver);

	strem = stdin;
	comhan();

	printf("Goodbye\n");
}

/*
 * Command loop. Read & execute commands from the user.
 */
void comhan() {
	/* 
	 * The number of commands executed so far. 
	 */
	int lno = 0;

	/*
	 * Variables for line input.
	 */
	size_t lread = 0;
	size_t lsize = 0;
	char *line = NULL;

	/*
	 * Initial prompt.
	 */
	printf("TechOS(%d)>", lno);

	/*
	 * Loop until we don't read anything.
	 */
	while((lread = getline(&line, &lsize, strem)) > 0) {
		/*
		 * Actual length of read string.
		 */
		size_t llen;

		/*
		 * Variables for parsing the command name/arguments.
		 */
		char *name;
		char *saveptr;

		/*
		 * The return status of the command.
		 */
		int comres;

		/*
		 * Exit the command loop if we've read EOF.
		 */
		if(feof(strem) != 0) goto cleanup;

		lno += 1;

		/*
		 * Remove the trailing newline from the command.
		 */
		llen = strlen(line);
		if(line[llen-1] == '\n')
			line[llen-1] = '\0';

		/*
		 * Get the name from the command.
		 */
		name = strtok_r(line, " \t", &saveptr);
		if(name != NULL) {
			/*
			 * The command to execute.
			 */
			struct command com;

			/*
			 * Determine the command to execute from the name.
			 */
			com = parsecom(name);

			/*
			 * Execute the command if we have one.
			 */
			if(com.comfun != NULL) {
				char *newline;

				newline = strdup(line);

				/*
				 * Execute the command.
				 *
				 * The status should be 
				 * - zero for executing succesfully
				 * - positive for non-fatal errors
				 * - negative for fatal errors.
				 */
				comres = execcom(com, saveptr, newline);

				free(newline);
				/*
				 * Exit the loop if something failed.
				 */
				if(comres < 0) goto cleanup;
			} else {
				/*
				 * No such command exists.
				 */
				printf("! NSC '%s'\n", name);
			}
		}

		/*
		 * Reprompt.
		 */
		printf("TechOS(%d)>", lno);
	}

	/*
	 * Cleanup after ourselves.
	 */
cleanup: if(line != NULL)
		free(line);
}

/*
 * Get a command from its name.
 */
struct command parsecom(char *name) {
	int i;

	/*
	 * Go through all the commands.
	 *
	 * If one matches, return it.
	 */
	for(i = 0; i < NUM_COMMANDS; i++) {
		struct command com = commands[i];

		if(strcmp(com.name, name) == 0) {
			return com;
		}
	}

	/*
	 * INVALID_COMMAND is the last command.
	 */
	return commands[NUM_COMMANDS+2];
}

/*
 * Execute a command, plus any arguments it has.
 */
int execcom(struct command com, char *argmarker, char *argline) {
	/*
	 * Arg. array for commands.
	 */
	char *argv[MAX_ARG_COUNT];
	int   argc;

	/*
	 * Return status of commands.
	 */
	int comret;

	int i;

	/*
	 * argv[0] is always the command name.
	 */
	argv[0] = com.name;
	argc    = 1;

	/*
	 * Gather space-seperated args into an array.
	 */
	for(i = 1; i < MAX_ARG_COUNT; i++) {
		char *tok;

		tok = strtok_r(NULL, " \t", &argmarker);

		if(tok == NULL) break;

		argv[i] = tok;
		argc   += 1;
	}

	/*
	 * Execute the command.
	 */
	comret = com.comfun(argc, argv, argline);

	return comret;
}
