#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "commands.h"
#include "techos.h"

#define HANDLECOM(nam) int handle_##nam(int argc, char **argv, char *argl)
/*
 * Handle exiting from the prompt.
 *
 * Make sure to confirm before hand.
 */
HANDLECOM(exit) {
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

HANDLECOM(version) {
	printf("TechOS v%d.%d\n", major_ver, minor_ver);
	printf("\tAuthors: Benjamin Culkin, Lucas Darnell, Jared Miller\n");
	printf("\tCompletion Date: 8/31/17\n");

	return 0;
}

HANDLECOM(date) {
	/*
	 * Time values.
	 */
	time_t     clocktime;
	struct tm *datetime;

	/*
	 * String buffer for times.
	 */
	char outtime[255];

	/*
	 * Amount of occupied buffer.
	 */
	size_t timesize;

	clocktime = time(NULL);

	datetime = localtime(&clocktime);

	timesize  = strftime(outtime, 255, "%a, %d %b %Y %T %z", datetime);

	printf("%s\n", outtime);

	return 0;
}
