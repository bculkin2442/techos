#include <assert.h>
#include <ctype.h>
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
#include "comlist.h"
#include "commandstate.h"
#include "commands.h"

#include "dispatchcmds.h"
#include "datecmds.h"
#include "scriptcmds.h"
#include "pcbcmds.h"
#include "filecmds.h"

#include "techos.h"

/* Initialize commands. */
void initcoms() {
	/* Doing nothing at the moment. */
}

/* Dispose of commands. */
void disposecoms() {
	/* Doing nothing at the moment. */
}

/* Add commands to a command list. */
void addcommands(struct comlist *list) {
	/* Date/time commands. */
	addcommand(list, "date",    "Display the current date/time",                &handle_date);
	addcommand(list, "datefmt", "Set the format the date is displayed/read in", &handle_datefmt);
	addcommand(list, "setdate", "Set the current date",                         &handle_setdate);
	addcommand(list, "time",    "Display the current time",                     &handle_time);

	/* Script commands. */
	addcommand(list, "script",    "Execute a script file",        &handle_script);

	/* PCB commands. */
	addcommand(list, "mkpcb",     "Create PCB and insert into queue",          &handle_mkpcb);
	addcommand(list, "sspcb",     "Suspend PCB and reinsert into queue",       &handle_sspcb);
	addcommand(list, "rspcb",     "Set PCB state to unsuspended and reinsert", &handle_rspcb);
	addcommand(list, "sppcb",     "Set PCB priority and reinserts",            &handle_sppcb);
	addcommand(list, "shpcb",     "Display PCB/queue information",             &handle_shpcb);

	/* Dispatching commands. */
	addcommand(list, "dispatch", "Dispatch all processes currently available", &handle_dispatch);

	/* File commands. */
	addcommand(list, "ls", "Show all files in the directory",  &handle_ls);
	addcommand(list, "cd", "Change the current directory",     &handle_cd);
	addcommand(list, "mkdir", "Create a new file directory",   &handle_mkdir);
	addcommand(list, "rmdir", "Delete a  directory",           &handle_rmdir);
	addcommand(list, "touch", "Create a file",                 &handle_touch);
	addcommand(list, "rm", "Delete a file",                    &handle_rm);

	/* Misc. Commands. */
	addcommand(list, "exit",    "Exit TechOS",                                    &handle_exit);
	addcommand(list, "help",    "Get help for commands, or list available ones.", &handle_help);
	addcommand(list, "version", "Display version/author information",             &handle_version);
	addcommand(list, "moo", " ~~moo~~",             &handle_moo);
}

/*
 * Handle exiting from the prompt.
 *
 * Make sure to confirm before hand.
 */
HANDLECOM(exit) {
	/* Variables for line input. */
	char  *pszLine = NULL;
	size_t lread, lsize;

	/* Command return status. */
	int ret;

	/* Init vars. */
	ret     = 0;
	pszLine = NULL;

	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: exit [-h] [--help]\n", ostate);
	}

	fprintf(ostate->output, "Are you sure you want to exit? (y/n) ");
	lread = getline(&pszLine, &lsize, stdin);

	/* Use rpmatch() to pick an action. */
	if(lread > 1) {
		int result = rpmatch(pszLine);

		switch(result) {
		case 1:
			ret = -1;
			break;
		case 0:
			ret = 0;
			break;
		case -1:
		default:
			fprintf(ostate->output, "ERROR: Unknown response '%s'\n", pszLine);
		}
	}

	/*
	 * Cleanup after ourselves.
	 */
	if(pszLine != NULL) free(pszLine);

	return ret;
}

/* Print out version/author information. */
HANDLECOM(version) {
		return checkhelpargs(argc, argv, "Usage: version [-advh] [--author] [--date] [--version] [--help]\n", ostate);

	/* Should version info be printed? */
	int printVersion = 1;
	/* Should author information be printed? */
	int printAuthor  = 0;
	/* Should date information be printed? */
	int printDate    = 0;

	/* Reinit getopt. */
	optind = 1;

	/* Process options. */
	while(1) {
		/*
		 * @TODO 10/07/17 Ben Culkin :VersionOpts
		 * 	Adjust these options to some better way of indicating
		 * 	which information we do/do not want to show. If this was
		 * 	java, I'd just go with a custom format string sorta
		 * 	thing, but that'd have to be reimplemented for C, and'd
		 * 	be kinda painful. Maybe we'd want to look into something
		 * 	with getsubopt()?
		 */

		/* The current option/long option. */
		int opt, optidx;
		/* Usage message. */
		static char *usage = "Usage: version [-advh] [--author] [--date] [--version] [--help]\n";

		static struct option longopts[] = {
			/* Info ctl. options. */
			{"author",  no_argument, 0, 'a'},
			{"date",    no_argument, 0, 'd'},
			{"version", no_argument, 0, 'v'},

			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

			/* Terminating option. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "advh", longopts, &optidx);

		/* Finish options. */
		if(opt == -1) break;

		switch(opt) {
			/* a, d and v are toggle options. */
		case 'a':
			printAuthor = 1 - printAuthor;
			break;
		case 'd':
			printDate = 1 - printDate;
			break;
		case 'v':
			printVersion = 1 - printDate;
			break;
		case 'h':
			fprintf(ostate->output, "%s", usage);
			return 0;
		default:
			fprintf(ostate->output, "ERROR: Unknown short option '%c'\n", opt);
			fprintf(ostate->output, "%s", usage);
			return 1;
		}
	}

	/* Print version info. */
	if(printVersion) fprintf(ostate->output, "\tTechOS v%d.%d\n", major_ver, minor_ver);
	if(printAuthor)  fprintf(ostate->output, "\tAuthors: Benjamin Culkin, Lucas Darnell, Jared Miller\n");
	if(printDate)    fprintf(ostate->output, "\tCompletion Date: 9/13/17\n");

	return 0;
}

HANDLECOM(help) {
	/* Usage message. */
	char *usage = "Usage: help [-h] [--help] [<command-name>]\n";

	/* Handle CLI args. */
	if(argc == 1) {
		fprintf(ostate->output, "Available commands:\n");

		/* 
		 * @TODO 10/07/17 Ben Culkin :PrintCommandState
		 * 	 printcommands needs to be rewritten to accept a
		 * 	 commandstate, so that it can handle things like command
		 * 	 categories and help-only commands/internal commands
		 * 	 that don't show up in the default help list.
		 */
		printcommands(ostate->pComstate->plCommands, ostate->output);
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
			/* File wasn't found. */
			fprintf(ostate->output, "\tINTERNAL ERROR: Manpage for command '%s' ('%s') couldn't be found.\n", argv[1], manpath);

			free(manpath);
			return 1;
		}
		if(S_ISREG(scratch.st_mode)) {
			/* Command to run. */
			char *compath;

			/* Create our command, then run it. */
			assert(asprintf(&compath, "man %s", manpath) != -1);
			if(system(compath) != 0) {
				fprintf(ostate->output, "\tINTERNAL ERROR: Could not display the manpage for '%s'\n", argv[1]);
			}

			free(compath);	
		} else {
			fprintf(ostate->output, "\tINTERNAL ERROR: Manpage for command '%s' ('%s') is not valid.\n", argv[1], manpath);

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
HANDLECOM(moo) {

	fprintf(ostate->output, "        (__)\n");
	fprintf(ostate->output, "        (oo)\n");
	fprintf(ostate->output, "  /------\\/\n");
	fprintf(ostate->output, " / |    ||\n");
	fprintf(ostate->output, "*  /\\---/\\\n");
	fprintf(ostate->output, "   ~~   ~~\n");
	fprintf(ostate->output, "....\"Have you mooed today?\"...\n");
	return 0;
}
