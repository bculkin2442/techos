#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include "osstate.h"
#include "command.h"
#include "filecmds.h"


HANDLECOM(ls) {
        return 1;
}
HANDLECOM(cd) {
        return 1;
}
HANDLECOM(mkdir) {
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
			break;
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
	char *dname;

	dname = argv[optind];





	return 1;
}
HANDLECOM(rmdir) {
	return 1;
}
HANDLECOM(touch) {
        return 1;
}
HANDLECOM(rm) {
        return 1;
}
