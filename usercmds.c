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

#include "users.h"
#include "usercmds.h"

/* Handle creating a user. */
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
		char *usage = "Usage mkusr [-h] [--help] [-l] <username>";

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

       	if(argc <= (optind)) {
       		fprintf(ostate->output, "\tERROR: Must provide the name of the user to create as an argument\n");
       		return 1;
       	}

	{
		char *pszUsername;

		pszUsername = argv[optind];

		if(ostate->puCurrent->type == UTY_BASIC) {
			fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
			return 1;
		}

		pszUsername = argv[optind];

		/* check if the user already exists */
		if(udblookup(ostate->pdUsers, pszUsername) == NULL) {
			char *pszPassword;
			size_t llen, lread;

			pszPassword = NULL;
			llen        = 0;

			fprintf(ostate->output, "Enter password for user '%s':\n", pszUsername);
			lread = getline(&pszPassword, &llen, stdin);

			/* Make sure a password is entered. */
			while(lread <= 0) {
				fprintf(ostate->output, "\tPassword cannot be empty, try again\n");			

				fprintf(ostate->output, "Enter password for user '%s':\n", pszUsername);
				lread = getline(&pszPassword, &llen, stdin);
			}

			/* Trim trailing newline. */
			llen = strlen(pszPassword);
			if(pszPassword[llen-1] == '\n') {
				pszPassword[llen-1] = '\0';
			}

			udbinsert(ostate->pdUsers, UTY_BASIC, pszUsername, pszUsername);

			fprintf(ostate->output, "Sucessfully created user '%s'\n", pszUsername);

			free(pszPassword);
		} else {
			fprintf(ostate->output, "\tERROR: A user who goes by '%s' already exists\n", pszUsername);
			return 1;
		}
	}

	return 0;
}

/* Handle deleting a user. */
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
		char *usage = "Usage rmusr [-h] [--help] <username>";

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

	if(argc <= (optind)) {
		fprintf(ostate->output, "\tERROR: Must provide the name of the user to delete as an argument\n");
		return 1;
	}

	{
		char *pszUsername;

		struct user *puUser;

		pszUsername = argv[optind];

		puUser = udblookup(ostate->pdUsers, pszUsername);

		if(puUser == NULL) {
			fprintf(ostate->output, "\tERROR: No user named '%s' exists\n", pszUsername);
			return 1;
		}

		if(ostate->puCurrent->type == UTY_BASIC) {
			fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
			return 1;
		}

		switch(puUser->type) {
		case UTY_BASIC:
			udbremove(ostate->pdUsers, pszUsername);
			break;
		case UTY_ADMIN:
			if(ostate->puCurrent->type == UTY_ADMIN) {
				fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
				return 1;
			}
			udbremove(ostate->pdUsers, pszUsername);
			break;
		case UTY_ROOT:
			fprintf(ostate->output, "\tERROR: Doing that would cause the user tree to collapse\n");
			return 1;
		}

		fprintf(ostate->output, "\tSuccessfully consigned user '%s' to oblivion\n", pszUsername);
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
		char *usage = "Usage pwd [-h] [--help] <username>";

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

	if(argc <= (optind)) {
		fprintf(ostate->output, "\tERROR: Must provide the name of the user to create as an argument\n");
		return 1;
	}

	{
		char *pszUsername;
		struct user *puUser;

		pszUsername = argv[optind];

		if(strcmp(ostate->puCurrent->pszName, pszUsername) == 0) {
			puUser = ostate->puCurrent;
		} else {
			struct user *puLookup;

			puLookup = udblookup(ostate->pdUsers, pszUsername);

			if(puLookup == NULL) {
				fprintf(ostate->output, "\tERROR: No user named '%s'\n", pszUsername);
				return 1;
			}

			if(ostate->puCurrent->type != UTY_ROOT && puLookup->type == UTY_ADMIN) {
				fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
				return 1;
			}

			puUser = puLookup;
		}

		{
			char *pszPassword;
			size_t llen, lread;

			pszPassword = NULL;
			llen        = 0;

			fprintf(ostate->output, "Enter password for user '%s':\n", pszUsername);
			lread = getline(&pszPassword, &llen, stdin);

			/* Make sure a password is entered. */
			while(lread <= 0) {
				fprintf(ostate->output, "\tPassword cannot be empty, try again\n");			

				fprintf(ostate->output, "Enter password for user '%s':\n", pszUsername);
				lread = getline(&pszPassword, &llen, stdin);
			}

			/* Trim trailing newline. */
			llen = strlen(pszPassword);
			if(pszPassword[llen-1] == '\n') {
				pszPassword[llen-1] = '\0';
			}

			free(puUser->pszPass);

			puUser->pszPass = pszPassword;
		}
	}

	return 0;
}

/* Handle adding or removing administrator privileges for a user. */
HANDLECOM(toggleadm) {
	/* Reinit getopt. */
	optind = 1;
	/* Handle options. */
	while (1) {

		/* The current option & long option. */
		int opt, optidx;

		/* Our usage message. */
		char *pszUsage = "Usage: toggleadm [-h] [--help] <username>";

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

	if(argc <= (optind)) {
		fprintf(ostate->output, "\tERROR: Must provide the name of the user to delete as an argument\n");
		return 1;
	}

	{
		char *pszUsername;

		struct user *puUser;

		pszUsername = argv[optind];

		puUser = udblookup(ostate->pdUsers, pszUsername);

		if(puUser == NULL) {
			fprintf(ostate->output, "\tERROR: No user named '%s' exists\n", pszUsername);
			return 1;
		}

		if(ostate->puCurrent->type != UTY_ROOT) {
			fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
			return 1;
		}

		switch(puUser->type) {
		case UTY_BASIC:
			puUser->type = UTY_ADMIN;
			break;
		case UTY_ADMIN:
			puUser->type = UTY_BASIC;
			break;
		case UTY_ROOT:
			fprintf(ostate->output, "\tERROR: Root is already all-powerful\n");
			return 1;
		}

		if(puUser->type == UTY_ADMIN)
			fprintf(ostate->output, "\tSuccessfully toggled administration status for user '%s': ON\n", pszUsername);
		else if(puUser->type == UTY_BASIC)
			fprintf(ostate->output, "\tSuccessfully toggled administration status for user '%s': OFF\n", pszUsername);
	}

	return 0;
}
