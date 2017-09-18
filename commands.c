#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "osstate.h"
#include "command.h"
#include "comlist.h"
#include "commands.h"
#include "techos.h"

#include "datecmds.h"

/*
 * Initialize commands.
 */
void initcoms() {
	/*
	 * Doing nothing at the moment.
	 */
}

/*
 * Dispose of commands.
 */
void disposecoms() {
	/*
	 * Doing nothing at the moment.
	 */
}

void addcommands(struct comlist *list) {
	addcommand(list, "exit",    "Exit TechOS",                                    &handle_exit);
	addcommand(list, "version", "Display version/author information",             &handle_version);
	addcommand(list, "date",    "Display the current date/time",                  &handle_date);
	addcommand(list, "datefmt", "Set the format the date is displayed/read in",   &handle_datefmt);
	addcommand(list, "setdate", "Set the current date",                           &handle_setdate);
	addcommand(list, "time",    "Display the current time",                       &handle_time);
	addcommand(list, "help",    "Get help for commands, or list available ones.", &handle_help);
}

/* Check if a command that only takes the 'help' argument recieved it. */
int checkhelpargs(int argc, char **argv, char *usage, struct osstate *ostate) {
	if(argc > 2 || (strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)) {
		fprintf(ostate->output, "\tERROR: Invalid command-line arguments.\n");
	}

	fprintf(ostate->output, "%s", usage);

	return 0;
}

/*
 * Handle exiting from the prompt.
 *
 * Make sure to confirm before hand.
 */
HANDLECOM(exit) {
	/* Variables for line input. */
	char *line = NULL;
	size_t lread;
	size_t lsize;

	/* Command return status. */
	int ret = 0;

	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: exit [-h] [--help]\n", ostate);
	}

	fprintf(ostate->output, "Are you sure you want to exit? (y/n) ");
	lread = getline(&line, &lsize, stdin);

	/* As long as we read at least one character, decide what to do off it. */
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
			fprintf(ostate->output, "Unknown answer '%c'\n", ch);
		}
	}

	/*
	 * Cleanup after ourselves.
	 */
	if(line != NULL) free(line);

	return ret;
}

/* Print out version/author information. */
HANDLECOM(version) {
	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: version [-h] [--help]\n", ostate);
	}

	/*
	 * Print version info.
	 *
	 * @TODO add flags for printing only parts of this information.
	 */
	fprintf(ostate->output, "TechOS v%d.%d\n", major_ver, minor_ver);
	fprintf(ostate->output, "\tAuthors: Benjamin Culkin, Lucas Darnell, Jared Miller\n");
	fprintf(ostate->output, "\tCompletion Date: 9/13/17\n");

	return 0;
}

HANDLECOM(help) {
	/* Usage message. */
	char *usage = "Usage: help [-h] [--help] [<command-name>]\n";

	/* Handle CLI args. */
	if(argc == 1) {
		fprintf(ostate->output, "Available commands\n");

		printcommands(all_commands, ostate->output);
	} else if(argc == 2){
		/* The path to the man page. */
		char *manpath;

		/* The struct for checking if a file exists. */
		struct stat scratch;

		int ret;

		/* Handle CLI args. */
		if(argv[1][0] == '-') {
			checkhelpargs(argc, argv, usage, ostate);
			return 1;
		}

		/* Get the path we want to use. */
		assert(asprintf(&manpath, "help/%s.1", argv[1]) != -1);

		/* Check if the file exists. */
		if(stat(manpath, &scratch) == -1) {
			/* Something went wrong. */
			fprintf(ostate->output, "\tINTERNAL ERROR: Something went wrong checking for the manpage");
			free(manpath);
			return 1;
		}
		if(S_ISREG(scratch.st_mode)) {
			/* Command to run. */
			char *compath;

			/* Create our command, then run it. */
			assert(asprintf(&compath, "man %s", manpath) != -1);
			if(system(compath) != 0) {
				fprintf(ostate->output, "\tINTERNAL ERROR: Something went wrong displaying the manpage.\n");
			}

			free(compath);	
		} else {
			fprintf(ostate->output, "\tERROR: No help available for command '%s'\n", argv[1]);

			free(manpath);
			return 1;
		}

		free(manpath);
	} else {
		fprintf(ostate->output, "\tERROR: Invalid command-line arguments\n");
		fprintf(ostate->output, "\t%s", usage);
	}

	return 0;
}
