#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

int handle_exit() {
	char *line = NULL;

	size_t lread;
	size_t lsize;

	int ret = 0;

	printf("Are you sure you want to exit? (y/n) ");

	lread = getline(&line, &lsize, stdin);

	if(lread > 1) {
		int ch = toupper(line[0]);

		switch(ch) {
		case 'Y':
			ret = 1;
			break;
		case 'N':
			ret = 2;
			break;
		default:
			printf("Unknown answer. Continuing\n");
		}
	}

	if(line != NULL)
		free(line);

	return ret;
}
