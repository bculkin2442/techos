#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "commands.h"
#include "techos.h"

/*
 * Define a command handler.
 */
#define HANDLECOM(nam) int handle_##nam(int argc, char **argv, char *argl)

/*
 * Input/output/time date formats.
 */
static char *in_datefmt;
static char *out_datefmt;
static char *time_datefmt;

/*
 * Initialize commands.
 */
void initcoms() {
	in_datefmt   = malloc(256);
	out_datefmt  = malloc(256);
	time_datefmt = malloc(256);

	sprintf(in_datefmt,   "%s", "%Y-%m-%d");
	sprintf(time_datefmt, "%s", "%r (%Z)");
	sprintf(out_datefmt,  "%s", "%A, %d %B %Y");
}

/*
 * Dispose of commands.
 */
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

	/*
	 * Handle CLI args.
	 */
	if(argc > 1) {
		if(argc > 2 || strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
			printf("ERROR: Invalid command-line arguments.\n");
		printf("Usage: exit [-h] [--help]\n");

		return 0;
	}

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
	/*
	 * Handle CLI args.
	 */
	if(argc > 1) {
		if(argc > 2 || strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
			printf("ERROR: Invalid command-line arguments.\n");
		printf("Usage: version [-h] [--help]\n");

		return 0;
	}
		
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
	 * Handle CLI args.
	 */
	if(argc > 1) {
		if(argc > 2 || strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
			printf("ERROR: Invalid command-line arguments.\n");
		printf("Usage: date [-h] [--help]\n");

		return 0;
	}
	/*
	 * Get the time and stringize it in the proper format.
	 */
	clocktime = time(NULL);
	datetime  = localtime(&clocktime);
	timesize  = strftime(outtime, 255, out_datefmt, datetime);

	printf("%s\n", outtime);

	return 0;
}
/*
 * Display current time.
 */
HANDLECOM(time) {
	
	if(argc > 1) {
                if(argc > 2 || strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
                        printf("ERROR: Invalid command-line arguments.\n");
                printf("Usage: time [-h] [--help]\n");

                return 0;
        }

}



/*
 * Configure the date format.
 */
HANDLECOM(datefmt) {
	/*
	 * The current option, and the current long option
	 */
	int opt, optidx;
	
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
	 * Reinit getopt.
	 */
	optind = 1;

	/*
	 * Parse CLI args.
	 *
	 * -s turns on mode setting.
	 * -d turns on mode displaying.
	 * -i selects the input format.
	 * -o selects the output format.
	 */
	while(1) {
		char *usage = "Usage: datefmt [-stdioh] [--help]\n";

		static struct option opts[] = {
			{"help", no_argument, 0, 0},
			{0,     0,           0, 0}
		};

		opt = getopt_long(argc, argv, "stdioh", opts, &optidx);
		if(opt == -1) break;

		switch(opt) {
		case 0:
			/*
			 * We picked a long option.
			 */
			switch(optidx) {
			case 0:
				printf("%s\n", usage);
				return 0;
			default:
				printf("ERROR: Invalid command-line argument\n");
				printf("%s\n", usage);
				return 1;
			}
			break;
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
		case 't':
			fmt = 2;
			break;
		case 'h':
			printf("%s\n", usage);
			return 0;
		default:
			printf("ERROR: Invalid command-line argument.\n");
			printf("%s\n", usage);
			return 1;
		}
	}

	if(set == 0) {
		/*
		 * Display the format.
		 */
		switch(fmt) {
		case 0:
			printf("%s\n", in_datefmt);
			break;
		case 1:
			printf("%s\n", out_datefmt);
			break;
		case 2:
			printf("%s\n", time_datefmt);
			break;
		default:
			printf("INTERNAL ERROR: Invalid format setting.\n");
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

		if(lread < 1) {
			printf("ERROR: No input available\n");
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
		switch(fmt) {
		case 0:
			sprintf(in_datefmt,  "%s", line);
			break;
		case 1:
			sprintf(out_datefmt, "%s", line);
			break;
		case 2:
			sprintf(time_datefmt, "%s", line);
			break;
		default:
			printf("INTERNAL ERROR: Invalid format setting.\n");
			free(line);
			return 1;
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
	time_t clocktime;

	/*
	 * Handle CLI args.
	 */
	if(argc > 1) {
		if(argc > 2 || strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
			printf("ERROR: Invalid command-line arguments.\n");
		printf("Usage: setdate [-h] [--help]\n");

		return 0;
	}
	/*
	 * Get the current time.
	 */
	clocktime = time(NULL);
	datetime  = localtime(&clocktime);

	/*
	 * Prompt/read the new date.
	 */
	printf("Enter the new date: ");
	lread = getline(&line, &lsize, strem);

	if(lread < 1) {
		printf("ERROR: No input provided.\n");
		return 1;
	}

	/*
	 * Trim trailing newline.
	 */
	llen = strlen(line);
	if(line[llen-1] == '\n')
		line[llen-1] = '\0';

	/*
	 * Parse the input according to the format.
	 */
	leftovers = strptime(line, in_datefmt, datetime);
	if(leftovers == NULL) {
		printf("\tERROR: Input doesn't match format '%s'\n", in_datefmt);
		return 1;
	}

	/*
	 * Sanitize/set the time.
	 */
	clocktime = mktime(datetime);

	/*
	 * Cleanup.
	 */
	free(line);

	return 0;
}

HANDLECOM(help) {
	char *usage = "Usage: help [-h] [--help] [<command-name>]";

	/*
	 * Handle CLI args.
	 */
	if(argc == 1) {
		int i;

		printf("Available commands\n");
		for(i = 0; i < NUM_COMMANDS; i++) {
			struct command com;

			com = commands[i];

			printf("\t%s\t%s\n", com.name, com.brief);
		}
	} else if(argc == 2){
		char *manpath;

		struct stat scratch;

		/*
		 * Handle CLI args.
		 */
		if(argv[1][0] == '-') {
			if(strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
				printf("ERROR: Invalid command-line arguments.\n");
		
			printf("%s\n", usage);
			return 1;
		}

		/*
		 * Get the path we want to use.
		 */
		asprintf(&manpath, "help/%s.1", argv[1]);

		/*
		 * Check if the file exists.
		 */
		stat(manpath, &scratch);
		if(S_ISREG(scratch.st_mode)) {
			char *compath;

			/*
			 * Create our command, then run it.
			 */
			asprintf(&compath, "man %s", manpath);
			system(compath);

			free(compath);	
		} else {
			printf("ERROR: No help available for command '%s'\n", argv[1]);

			free(manpath);
			return 1;
		}

		free(manpath);
	} else {
		printf("ERROR: Invalid command-line arguments\n");
		printf("%s\n", usage);
	}

	return 0;
}
