#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "techos.h"

const int major_ver = 1;
const int minor_ver = 0;

int main() {
	printf("Welcome to TechOS v%d.%d\n", major_ver, minor_ver);

	comhan();

	printf("Goodbye\n");
}

void comhan() {
	int lno = 0;

	size_t lread = 0;
	size_t lsize = 0;
	
	char *line = NULL;

	printf("TechOS>");

	while((lread = getdelim(&line, &lsize, '\n', stdin)) > 0) {
		int com;

		size_t llen = 0;

		if(feof(stdin) != 0) goto cleanup;

		lno += 1;

		llen = strlen(line);
		if(line[llen-1] == '\n')
			line[llen-1] = '\0';

		com = parsecom(line);

		switch(com) {
		case 1:
			if(handle_exit() == 1) goto cleanup;

			break;
		default:
			printf("Unknown command.\n");
		}

		printf("Read line '%s'\n", line);
		printf("TechOS>");
	}

cleanup: if(line != NULL)
		free(line);
}

int parsecom(char *line) {
	char *newline;

	char *name;
	char *saveptr;

	int com;

	newline = strdup(line);

	name = strtok_r(newline, " \t", &saveptr);

	if(name != NULL) {
		if(strcmp("exit", name) == 0)
			com = 1;
		else
			com = -1;
	}

	free(newline);

	return com;
}
