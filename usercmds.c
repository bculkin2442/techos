#define _ATFILE_SOURCE

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "osstate.h"
#include "command.h"

#include "user.h"
#include "usercmds.h"

HANDLECOM(mkusr) {
	/* Reinit getopt. */
	optind = 1;
	int showSize = 0;

	while(1) {
		/* Enum declaration for long options. */
		enum scriptopt {
			/* Help option. */
			SO_HELP = 0,
		};

		/* The current option, and the current long option. */
		int opt, optidx;

		/* Our usage message. */
		char *usage = "Usage ls [-h] [--help] [-l] <directory-name or path>";

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

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
			/* 
			 * We picked a long option, but they are handled by
			 * their short options.
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;

		}
	}

	/* The specified filename. */
        char *username;
	char *pass = NULL;
	size_t lsize;


        if(argc <= (optind)) {
         	fprintf(ostate->output, "\tERROR: Must provide the file to create as an argument\n");
         	return 1;
        }

	username = argv[optind];


	/* check if the user already exists */
	if(udblookup(ostate->pdUsers,username) == NULL)
	{
		fprintf(ostate->output, "Enter password for new user %s:\n", username);
		pass = getline(&pass, &lsize, stdin);
		/* check if a password was entered */
		if(sizeof(pass) > 0)
		{
			udbinsert(ostate->pdUsers, UTY_BASIC, username, pass);
			fprintf(ostate->output, "Sucessfully created user %s\n", username);
		}
		else
		{
			fprintf(ostate->output, "\tPassword cannot be empty, try again\n", username);			
		}
	}
	else
	{
		fprintf(ostate->output, "\tERROR: That username already exists\n", username);
		return 1;
	}

	return 0;
}

HANDLECOM(rmusr) {
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
		char *usage = "Usage cd [-h] [--help] <directory-name>";

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

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
			/* 
			 * We picked a long option, but they are handled by
			 * their short options.
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;

		}
	}

	return 0;
}

HANDLECOM(pwd) {
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
		char *usage = "Usage mkdir [-h] [--help] <directory-name>";

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

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
			/* 
			 * We picked a long option, but they are handled by
			 * their short options.
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;

		}
	}

	return 0;
}

/* Handle removing a directory. */
HANDLECOM(mkadm) {
	/* Reinit getopt. */
	optind = 1;
	/* Handle options. */
	while (1) {

		/* The current option & long option. */
		int opt, optidx;

		/* Our usage message. */
		char *pszUsage = "Usage: rmdir [-h] [--help] <directory-name>";

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

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
			/* 
			 * We picked a long option, but they are handled by
			 * their short options.
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", pszUsage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", pszUsage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", pszUsage);
			return 1;

		}
	}
	return 0;
}

HANDLECOM(rmadm) {
	/* Reinit getopt. */
	optind = 1;
	/* Handle options. */
	while (1) {

		/* The current option & long option. */
		int opt, optidx;

		char *pszUsage = "Usage: touch [-h] [--help] <file-name>";
		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

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
			/* 
			 * We picked a long option, but they are handled by
			 * their short options.
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", pszUsage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", pszUsage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", pszUsage);
			return 1;
		}
	}
		return 0;
}

