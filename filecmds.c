<<<<<<< HEAD
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

			/* Terminating option. */
			{0, 0, 0, 0}
		}

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

	/*make sure enough arguments are provided*/
	if(argc <= (optind + 1)) {
                fprintf(ostate->output, "\tERROR: Must provide the directory name as an argument\n");
                return 1;
        }

        struct stat st;

        /* check the status */
        if(mkdirat(ostate->fWorkingDir, dname, 0777) != 0)
	{

		switch(errno)
		{
			case EACCES:
			fprintf(ostate->output, "\tERROR: Cannot write to given path\n");
			break;
			case EEXIST:
			fprintf(ostate->output, "\tERROR: Pathname already exists\n");
			break;
			case EDQUOT:
			case EFAULT:
			case ELOOP:
			case EMLINK:
			fprintf(ostate->output, "\tERROR: Unknown error\n");
			break;
			case ENAMETOOLONG:
			fprintf(ostate->output, "\tERROR: Pathname too long\n");
			break;
			case ENOENT:
			fprintf(ostate->output, "\tERROR: A directory component in pathname does not exist\n");
			break;
			case ENOMEM:
			fprintf(ostate->output, "\tERROR: Insufficient kernel memory\n");
			break;
			case ENOSPC:
			fprintf(ostate->output, "\tERROR: No room for new directory\n");
			break;
			case ENOTDIR:
			fprintf(ostate->output, "\tERROR: A  component used as a directory in pathname is not, in fact, a directory\n");
			break;
			case EPERM:
			fprintf(ostate->output, "\tERROR: Filesystem does not support creating directories\n");
			break;
			default:
			break;
		}
	}


	return 1;
}

/* Handle removing a directory. */
HANDLECOM(rmdir) {
	/* Handle options. */
	{
		/* Reinit getopt. */
		optind = 1;

		/* The current option & long option. */
		int opt, optidx;

		/* Our usage message. */
		char *pszUsage = "Usage: rmdir [-h] [--help] <directory-name>";

		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

			/* Terminating option. */
			{0, 0, 0, 0}
		}

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
			return 1;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;

		}
	}

	if(argc <= (optind + 1)) {
		fprintf(ostate->output, "\tERROR: Must provide the directory to remove as an argument\n");
		return 1;
	}

	/* The specified directory. */
	char *pszDirname;
	
	pszDirname = argv[optind];

	/* Verify that we're trying to delete a directory. */
	{
		/* Scratch struct. */
		struct stat buf;

		/* Stat the thing. */
		if(fstatat(ostate->fWorkingDir, pszDirname, &buf) != 0) {
			fprintf(ostate->output, "\tERROR: Could not access '%s'\n", pszDirname);
			return 1;
		}

		/* Check that it is a directory. */
		if(!S_ISDIR(buf)) {
			fprintf(ostate->output, "\tERROR: '%s' is not a directory\n", pszDirname);
			return 1;
		}

		/* Ensure the directory is empty. */
		{
			/* FD to directory. */
			int fDir;
			/* Directory stream. */
			DIR *sDir;
			/* Current entry into the stream. */
			struct dirent *pdEnt;
			/* The number of directory entries read so far. */
			int count;

			/* Init count. */
			count = 0;

			fDir = openat(ostate->fWorkingDir, pszDirname);
			if(fDir == -1) {
				fprintf(ostate->output, "\tERROR: Could not check if '%s' was empty\n", pszDirname);
				return 1;
			}

			sDir = fdopendir(fDir);
			if(sDir == NULL) {
				fprintf(ostate->output, "\tERROR: Could not check if '%s' was empty\n", pszDirname);
				return 1;
			}

			pdEnt = readdir(sDir);
			while(pdEnt != NULL && count <= 2) {
				count += 1;
			}

			if(count > 2) {
				fprintf(ostate->output, "\tERROR: Directory '%d' is not empty\n", pszDirname);

				closedir(sDir);
				/* fDir is closed by the closedir. */
			}

			closedir(sDir);
			/* fDir is closed by the closedir. */
		}
	}

	if(unlinkat(ostate->fWorkingDir, pszDirname, AT_REMOVEDIR) != 0) {
		/* Error removing directory. */
		switch(errno) {
		/* @TODO 11/07/17 Ben Culkin :RmdirErrors
		 * 	Give some more specific errors.
		 */
		default:
			fprintf(ostate->output, "\tERROR: Unknown error %d removing directory '%s'\n", errno, pszDirname);
			return 1;
		}
	}

	fprintf(ostate->output, "Successfully removed directory '%s'\n", pszDirname);
	return 0;
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
