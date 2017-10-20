#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"

/* Parse a string from a set of arguments. */
static char *parsestr(char *pszLead, char delim, char **pSaved) {
	/* The rest of the string from the arguments. */
	char *pszRest;
	/* The string we're returning. */
	char *pszStrang;

	/* The rest of the strings contents. */
	pszRest = (char *)strtok_r(NULL, &delim, pSaved);

	/* Print the string, and bail if that fails. */
	if(asprintf(&pszStrang, "%s %s", pszLead, pszRest) == -1) assert(0);

	return pszStrang;
}

/* Parse a set of CLI arguments. */
struct cliargs parseargs(char *pszStrang) {
	/* The return value. */
	struct cliargs args;

	/* The pointer for parsing. */
	char *pSave;
	/* The currently parsed token. */
	char *pszToken;
	/* Length of current token. */
	int lToken;

	/* Initialize argument count. */
	args.argc = 0;
	/* Allocate space for the array of arguments. */
	args.argv = calloc(MAX_CLI_ARGS, sizeof(char *));
	/* Fail if we couldn't allocate memory. */
	assert(args.argv != NULL);

	/* Get the initial tokens. */
	pszToken = (char *)strtok_r(pszStrang, " ", &pSave);

	while(pszToken != NULL && args.argc < MAX_CLI_ARGS) {
		lToken = strlen(pszToken);

		/* The possible quote. */
		char posQuote;
		/* The slot the arg goes into. */
		char **ppszArg;

		posQuote = pszToken[0];
		ppszArg  = &(args.argv[args.argc]);

		if(posQuote == '\'' || posQuote == '\"') {
			if(pszToken[lToken - 1] == posQuote) {
				/* Length of single-string. */
				int stranglen;

				/* Single word string, no parsing needed */
				*ppszArg = (char *)strdup(pszToken + 1);
				stranglen = strlen(*ppszArg);

				/* Pull off the ending quote. */
				ppszArg[0][stranglen - 1] = '\0';
			} else {
				/* Parse a string. */
				*ppszArg = parsestr(pszToken + 1, posQuote, &pSave);
			}
		} else {
			/* No string, just add the argument. */
			*ppszArg = (char *)strdup(pszToken);
		}

		/* Get the next token. */
		pszToken   = (char *)strtok_r(NULL, " ", &pSave);
		args.argc += 1;
	}

	return args;
}
