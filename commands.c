#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

/*
 * Handle exiting from the prompt.
 *
 * Make sure to confirm before hand.
 */
int handle_exit(int argc, char **argv) {
	/*
	 * Variables for line input.
	 */
	char *line = NULL;
	size_t lread;
	size_t lsize;

	/*
	 * Command return status.
	 */
	int ret = 0;

	printf("Are you sure you want to exit? (y/n) ");
	lread = getline(&line, &lsize, stdin);

	/*
	 * As long as we read at least one character, decide what to do off it.
	 */
	if(lread > 1) {
		int ch = toupper(line[0]);

		switch(ch) {
		case 'Y':
			ret = -1;
			break;
		case 'N':
			ret = 0;
			break;
		default:
			printf("? UNA '%c'\n", ch);
		}
	}

	/*
	 * Cleanup after ourselves.
	 */
	if(line != NULL)
		free(line);

	return ret;
}
