#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"

/* Parse a string from a set of arguments. */
char *parsestr(char *lead, char delim, char **args) {
	/* The rest of the string from the arguments. */
	char *rest;
	/* The string we're returning. */
	char *strang;

	/* The rest of the strings contents. */
	rest = (char *)strtok_r(NULL, &delim, args);

	/* Print the string, and bail if that fails. */
	if(asprintf(&strang, "%s %s", lead, rest) == -1) assert(0);

	return strang;
}

/* Parse a set of CLI arguments. */
struct cliargs parseargs(char *strang) {
	/* The return value. */
	struct cliargs args;

	/* The pointer for parsing. */
	char *pSave;
	/* The currently parsed token. */
	char *token;

	/* Initialize argument count. */
	args.argc = 0;
	/* Allocate space for the array of arguments. */
	args.argv = calloc(MAX_CLI_ARGS, sizeof(char *));
	/* Fail if we couldn't allocate memory. */
	assert(args.argv != NULL);

	/* Get the initial tokens. */
	token     = (char *)strtok_r(strang, " ", &pSave);

	while(token != NULL && args.argc < MAX_CLI_ARGS) {
		if(token[0] == '\'' || token[0] == '\"') {
			/* Parse a string. */
			args.argv[args.argc] = parsestr(token + 1, token[0], &pSave);
		} else {
			/* No string, just add the argument. */
			args.argv[args.argc] = (char *)strdup(token);
		}

		/* Get the next token. */
		token      = (char *)strtok_r(NULL, " ", &pSave);
		args.argc += 1;
	}

	return args;
}
