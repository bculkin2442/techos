#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "osstate.h"
#include "command.h"
#include "commands.h"

#include "datecmds.h"

/* Print out localtime in the current date format. */
HANDLECOM(date) {
	/* Time values. */
	time_t	   clocktime;
	struct tm *datetime;

	/* String buffer for times. */
	char outtime[255];

	/* Amount of occupied buffer. */
	size_t timesize;

	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: date [-h] [--help]\n", ostate);
	}

	/* Get the time and stringize it in the proper format. */
	timesize  = strftime(outtime, 255, ostate->out_datefmt, ostate->datetime);
	/* Error if the format was too long. */
	if(timesize == 0) {
		fprintf(ostate->output, "ERROR: Output for format '%s' is too long. It must be shorter than 255 characters when filled out\n", ostate->out_datefmt);
		return 1;
	}

	fprintf(ostate->output, "%s\n", outtime);

	return 0;
}

/* Display current time. */
HANDLECOM(time) {
	/* Time values. */
	time_t	   clocktime;
	struct tm *datetime;

	/* String buffer for times. */
	char outtime[255];

	/* Amount of occupied buffer. */
	size_t timesize;

	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: time [-h] [--help]\n", ostate);
	}

	/* Update the time in our time struct. */
	clocktime = time(NULL);
	datetime  = localtime(&clocktime);

	ostate->datetime->tm_sec = datetime->tm_sec;
	ostate->datetime->tm_min = datetime->tm_min;
	ostate->datetime->tm_hour = datetime->tm_hour;

	/* Stringize the time in the proper format. */
	timesize  = strftime(outtime, 255, ostate->time_datefmt, ostate->datetime);
	/* Error if the format was too long. */
	if(timesize == 0) {
		fprintf(ostate->output, "ERROR: Output for format '%s' is too long. It must be shorter than 255 characters when filled out\n", ostate->out_datefmt);
		return 1;
	}

	fprintf(ostate->output, "%s\n", outtime);

	return 0;
}


/* Configure the date format. */
HANDLECOM(datefmt) {
	/* Enum declarations for modes. */
	enum setmode { SM_SET, SM_DISPLAY,         };
	enum fmtmode { FM_IN,  FM_OUT,     FM_TIME };

	/* Whether to set or display the format. */
	enum setmode set;
	/* The format to display/modify. */
	enum fmtmode fmt;
	/* The format provided by the user. */
	char *pszFormat;

	/* Set options to sensible defaults. */
	set       = SM_DISPLAY;
	fmt       = FM_IN;
	pszFormat = NULL;

	/* Reinit getopt. */
	optind = 1;

	/* Parse CLI args. */
	while(1) {
		/* The current option, and the current long option */
		int opt, optidx;

		/* Our usage message. */
		static const char *usage = "Usage: datefmt [-stdioh] [--help] [--set|--display] [--time|--in|--out] [-f|--format <format>]\n";

		/* The long options we take. */
		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},
			{"format", no_argument, 0, 'f'},

			/* Mode options. */
			{"set",     no_argument, 0, 's'},
			{"display", no_argument, 0, 'd'},

			/* Format picking options. */
			{"time", no_argument, 0, 't'},
			{"in",   no_argument, 0, 'i'},
			{"out",  no_argument, 0, 'o'},

			/* Terminating option. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "f:stdioh", opts, &optidx);
		/* Break if we've processed every option. */
		if(opt == -1) break;

		/* Handle options. */
		switch(opt) {
		case 0:
			/* We picked a long option. */
			switch(optidx) {
				/* 
				 * Long options are handled by corresponding
				 * short options.
				 */
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'f':
			/* Free previous format. */
			if(pszFormat != NULL) {
				free(pszFormat);
			}
			/*
			 * @NOTE
			 * 	Is this duplication necessary?
			 */
			pszFormat = (char *)strdup(optarg);
			break;
		case 's':
			set = SM_SET;
			break;
		case 't':
			fmt = FM_TIME;
			break;
		case 'd':
			set = SM_DISPLAY;
			break;
		case 'i':
			fmt = FM_IN;
			break;
		case 'o':
			fmt = FM_OUT;
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}
	}

	if(set == SM_DISPLAY) {
		/* Display the format. */
		switch(fmt) {
		case FM_IN:
			fprintf(ostate->output, "%s\n", ostate->in_datefmt);
			break;
		case FM_OUT:
			fprintf(ostate->output, "%s\n", ostate->out_datefmt);
			break;
		case FM_TIME:
			fprintf(ostate->output, "%s\n", ostate->time_datefmt);
			break;
		default:
			/* Shouldn't happen. */
			assert(0);
		}
	} else {
		/* Read a format only if we need to. */
		if(pszFormat == NULL) {
			/* Variables for format input. */
			size_t lsize, lread, llen;

			/* Prompt/read the new format. */
			fprintf(ostate->output, "Enter the new format: ");
			lread = getline(&pszFormat, &lsize, ostate->strem);

			if(lread < 1) {
				fprintf(ostate->output, "\tERROR: No input available\n");
				return 1;
			}

			/* Trim trailing newline. */
			llen = strlen(pszFormat);
			if(pszFormat[llen-1] == '\n')
				pszFormat[llen-1] = '\0';

			/* Warn if truncation is going to occur. */
			if(llen >= 256) fprintf(ostate->output, "WARNING: Truncating format '%1$s' to '%1$.256s'\n", pszFormat);
		}

		/* Set the format. */
		switch(fmt) {
		case FM_IN:
			sprintf(ostate->in_datefmt,  "%.256s", pszFormat);
			break;
		case FM_OUT:
			sprintf(ostate->out_datefmt, "%.256s", pszFormat);
			break;
		case FM_TIME:
			sprintf(ostate->time_datefmt, "%.256s", pszFormat);
			break;
		default:
			/* Shouldn't happen. */
			assert(0);
		}

		/* Cleanup after ourselves. */
		free(pszFormat);

		fprintf(ostate->output, "Format set\n");
	}

	return 0;
}

HANDLECOM(setdate) {
	/*
	 * @TODO 10/25/17 Ben Culkin :SetdateCLI
	 *	Convert this to use getopt for arg-handling and add the
	 *	following options:
	 * 	- Specify the date as a CLI param
	 * 	- Specify a custom format to use
	 */
	/* Variables for date input. */
	char *line;
	size_t lsize, lread, llen;

	/* The time from the line, and any left-over bits. */
	struct tm *datetime;
	char	  *leftovers;

	/* The official time. */
	time_t clocktime;

	/* Handle CLI args. */
	if(argc > 1) {
		return checkhelpargs(argc, argv, "Usage: setdate [-h] [--help]\n", ostate);
	}

	/* Get the current time. */
	datetime  = ostate->datetime;

	/* Prompt/read the new date. */
	fprintf(ostate->output, "Enter the new date: ");
	lread = getline(&line, &lsize, ostate->strem);

	if(lread < 1) {
		fprintf(ostate->output, "\tERROR: No input provided.\n");
		return 1;
	}

	/* Trim trailing newline. */
	llen = strlen(line);
	if(line[llen-1] == '\n')
		line[llen-1] = '\0';

	/* Parse the input according to the format. */
	leftovers = (char *)strptime(line, ostate->in_datefmt, datetime);
	if(leftovers == NULL) {
		/* The format didn't match correctly. */
		fprintf(ostate->output, "\tERROR: Input '%s' doesn't match format '%s'\n", line, ostate->in_datefmt);
		free(line);
		return 1;
	} else if(*leftovers != '\0') {
		/* There were trailing characters in the input. */
		fprintf(ostate->output, "\tWARNING: Trailing input '%s' unused by format\n", leftovers);
	}

	/* Sanitize/set the time. */
	clocktime        = mktime(datetime);
	ostate->datetime = localtime(&clocktime);
	if(ostate->datetime == NULL) {
		fprintf(ostate->output, "\tFATAL ERROR: Date/time value overflowed.");
		free(line);
		return -1;
	}

	/* Cleanup. */
	free(line);
	return 0;
}
