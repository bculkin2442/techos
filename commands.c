#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "commands.h"
#include "techos.h"

#define HANDLECOM(nam) int handle_##nam(int argc, char **argv, char *argl)

/*
 * Input/output date formats.
 */
static char *in_datefmt;
static char *out_datefmt;

void initcoms() {
	in_datefmt  = malloc(256);
	out_datefmt = malloc(256);

	sprintf(in_datefmt,  "%s", "%Y-%m-%d %H:%M:%S");
	sprintf(out_datefmt, "%s", "%a, %d %b %Y %T %z");
}

void disposecoms() {
	free(out_datefmt);
	free(in_datefmt);
}
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
			printf("Unknown answer '%c'\n", ch);
		}
	}

	/*
	 * Cleanup after ourselves.
	 */
	if(line != NULL)
		free(line);

	return ret;
}

/*
 * Print out version/author information.
 */
HANDLECOM(version) {
	printf("TechOS v%d.%d\n", major_ver, minor_ver);
	printf("\tAuthors: Benjamin Culkin, Lucas Darnell, Jared Miller\n");
	printf("\tCompletion Date: 8/31/17\n");

	return 0;
}

/*
 * Print out localtime in the current date format.
 */
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

	/*
	 * Get the time and stringize it in the proper format.
	 */
	clocktime = time(NULL);
	datetime = localtime(&clocktime);
	timesize  = strftime(outtime, 255, out_datefmt, datetime);

	printf("%s\n", outtime);

	return 0;
}

/*
 * Configure the date format.
 */
HANDLECOM(datefmt) {
	/*
	 * The current option.
	 */
	int opt;
	
	/*
	 * Whether to set or display the format.
	 */
	int set;

	/*
	 * The format to display/modify.
	 *
	 * 0 is the input format, 1 is the output format.
	 */
	int fmt;

	set = 0;
	fmt = 0;

	/*
	 * Parse CLI args.
	 *
	 * -s turns on mode setting.
	 * -d turns on mode displaying.
	 * -i selects the input format.
	 * -o selects the output format.
	 */
	while((opt = getopt(argc, argv, "sdio")) != -1) {
		switch(opt) {
		case 's':
			set = 1;
			break;
		case 'd':
			set = 0;
			break;
		case 'i':
			fmt = 0;
			break;
		case 'o':
			fmt = 1;
			break;
		default:
			printf("Usage: datefmt [-sdio]\n");
			return 1;
		}
	}

	if(set == 0) {
		/*
		 * Display the format.
		 */
		if(fmt == 0) {
			printf("%s\n", in_datefmt);
		} else {
			printf("%s\n", out_datefmt);
		}
	} else {
		/*
		 * Variables for format input.
		 */
		char *line;
		size_t lsize, lread, llen;

		/*
		 * Prompt/read the new format.
		 */
		printf("Enter the new format: ");
		lread = getline(&line, &lsize, strem);

		if(lread > 1) {
			printf("! IOE\n");
			return 1;
		}

		/*
		 * Trim trailing newline.
		 */
		llen = strlen(line);
		if(line[llen-1] == '\n')
			line[llen-1] = '\0';

		/*
		 * Set the format.
		 */
		if(fmt == 0) {
			sprintf(in_datefmt,  "%s", line);
		} else {
			sprintf(out_datefmt, "%s", line);
		}

		/*
		 * Cleanup.
		 */
		free(line);

		printf("Format set\n");
	}

	return 0;
}

HANDLECOM(setdate) {
	/*
	 * Variables for date input.
	 */
	char *line;
	size_t lsize, lread, llen;

	/*
	 * The time from the line, and any left-over bits.
	 */
	struct tm *datetime;
	char      *leftovers;

	/*
	 * The official time.
	 */
	time_t *clocktime;

	/*
	 * Get the current time.
	 */
	datetime = localtime(time(NULL));

	/*
	 * Prompt/read the new date.
	 */
	printf("Enter the new date: ");
	lread = getline(&line, &lsize, strem);

	if(lread > 1) {
		printf("ERROR: No input provided.\n");
		return 1;
	}

	/*
	 * Trim trailing newline.
	 */
	llen = strlen(line);
	if(line[llen-1] == '\n')
		line[llen-1] = '\0';

	leftovers = strptime(line, in_datefmt, datetime);

	if(leftovers == NULL) {
		printf("\tERROR: Input doesn't match format '%s'", in_datefmt);
		return 1;
	}

	clocktime = mktime(datetime);

	stime(clocktime);

	free(line);

	return 0;
}
