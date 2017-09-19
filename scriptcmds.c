#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include "osstate.h"
#include "command.h"
#include "scriptcmds.h"

HANDLECOM(script) {
	/* Reinit getopt. */
	optind = 1;

	while(1) {
		/* Enum declaration for long options. */
		enum scriptopt {
			/* Help option. */
			SO_HELP = 0,
		};

		/* The current option, and the current long option. */
		int opt, optidx;

		/* Our usage message. */
		char *usage = "Usage script [-h] [--help] <file-name>";

		/* The long options we take. */
		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 0},
			
			/* Terminating option. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "h", opts, &optidx);
		/* Break if we've processed everything. */
		if(opt == -1) break;

		/* Handle options. */
		switch(opt) {
		case 0:
			/* We picked a long option. */
			switch(optidx) {
			case SO_HELP:
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;

		}
	}

	/* The name of the file. */
	char *fname;
	/* The file descriptor of the file. */
	FILE *script;

	fname = argv[optind];
	
	script = fopen(fname, "r");
	if(script == NULL) {
		/* Some error with opening the file. */
		char *errmsg = strerror(errno);

		fprintf(ostate->output, "\tERROR: Couldn't open file '%s' because '%s'\n", fname, errmsg);
	} else {
		/* Enclose I/O variables in their own scope. */
		/* Variables for line input. */
		size_t  lread = 0;
		size_t  lsize = 0;
		char   *line  = NULL;
		/* Loop until we don't read anything. */
		while((lread = getline(&line, &lsize, script)) > 0) {
			/* Exit the command loop if we've read EOF. */
			if(feof(script) != 0) break;
			/* Handle the line, and exit if a command failed */
			if(handleline(ostate, line) < 0) break;
		}
		/* Cleanup ourselves. */
		if(line != NULL) free(line);
	}

	return 1;
}

HANDLECOM(scriptctl) {
	return 1;
}
