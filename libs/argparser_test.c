#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libs/argparser.h"

int main() {
	char   *line;
	size_t  llen, lread;

	printf("Enter a line to parse: ");
	while((lread = getline(&line, &llen, stdin)) > 0) {
		struct cliargs args;

		if(feof(stdin)) break;

		args = parseargs(line);

		printf("Got %d args\n", args.argc);

		int i;
		for(i = 0; i < args.argc; i++) {
			printf("\tArg %d: '%s'\n", i, args.argv[i]);
		}

		printf("Enter a line to parse: ");
	}

	if(line != NULL) free(line);

	return 0;
}
