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

	/* Make sure enough arguments are provided. */
	if(argc <= (optind)) {
		fprintf(ostate->output, "\tERROR: Must provide the directory name as an argument\n");
		return 1;
	}

	{
		/* The name of the file. */
		char *pszDirname;

		pszDirname = argv[optind];

		/* check the status */
		if(mkdirat(ostate->fWorkingDir, pszDirname, 0777) != 0) {
			switch(errno) {
			case EACCES:
			case EPERM:
				fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
				break;
			case EEXIST:
				fprintf(ostate->output, "\tERROR: A directory or file with that name already exists\n");
				break;
			case ENAMETOOLONG:
				fprintf(ostate->output, "\tERROR: '%s' is too long for a directory name\n", pszDirname);
				break;
			case ENOSPC:
				fprintf(ostate->output, "\tERROR: No diskspace remaining\n");
				break;
			default:
				{
					char *pszError = strerror(errno);
					fprintf(ostate->output, "\tERROR: Unknown error creating directory '%s'\n", pszDirname);
					fprintf(ostate->output, "\tCause: %s\n", pszError);
				}
				break;
			}

			return 1;
		}

		fprintf(ostate->output, "Successfully created directory '%s'\n", pszDirname);


	}

	return 0;
}

/* Handle removing a directory. */
HANDLECOM(rmdir) {
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

	if(argc <= (optind)) {
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
		if(fstatat(ostate->fWorkingDir, pszDirname, &buf, 0) != 0) {
			fprintf(ostate->output, "\tERROR: Could not access '%s'\n", pszDirname);
			return 1;
		}

		/* Check that it is a directory. */
		if(!S_ISDIR(buf.st_mode)) {
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

			fDir = openat(ostate->fWorkingDir, pszDirname, 0);
			if(fDir == -1) {
				fprintf(ostate->output, "\tERROR: Could not check if '%s' was empty\n", pszDirname);
				return 1;
			}

			sDir = (DIR *)fdopendir(fDir);
			if(sDir == NULL) {
				fprintf(ostate->output, "\tERROR: Could not check if '%s' was empty\n", pszDirname);
				return 1;
			}

			pdEnt = readdir(sDir);
			while(pdEnt != NULL) {
				count += 1;

				pdEnt = readdir(sDir);
			}

			if(count > 2) {
				fprintf(ostate->output, "\tERROR: Directory '%s' is not empty\n", pszDirname);

				closedir(sDir);

				return 1;
				/* fDir is closed by the closedir. */
			} else {
				closedir(sDir);
				/* fDir is closed by the closedir. */
			}
		}
	}

	if(unlinkat(ostate->fWorkingDir, pszDirname, AT_REMOVEDIR) != 0) {
		/* Error removing directory. */
		switch(errno) {
		case EACCES:
		case EPERM:
		case EFAULT:
			fprintf(ostate->output, "\tERROR: You don't have permission to delete '%s'\n", pszDirname);
			break;
		case EROFS:
			fprintf(ostate->output, "\tERROR: Filesystem is read-only. Can't delete directory '%s'\n", pszDirname);
			break;
		default:
			{
				char *pszError = strerror(errno);
				fprintf(ostate->output, "\tERROR: Unknown error removing directory '%s'\n", pszDirname);
				fprintf(ostate->output, "\tCause: %s\n", pszError);
			}
			break;
		}

		return 1;
	}

	fprintf(ostate->output, "Successfully removed directory '%s'\n", pszDirname);
	return 0;
}

HANDLECOM(touch) {
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

	{
		/* The specified filename. */
		char *pszFilename;
		/* The file being opened. */
		int dFile;

		if(argc <= (optind)) {
			fprintf(ostate->output, "\tERROR: Must provide the file to create as an argument\n");
			return 1;
		}

		pszFilename = argv[optind];

		/* Open the file, give an error if opening fails. */
		dFile = openat(ostate->fWorkingDir, pszFilename, O_RDONLY | O_CREAT | O_EXCL, 0777);
		if(dFile == -1) {
			switch(errno) {
			case EACCES:
			case EPERM:
			case EFAULT:
				fprintf(ostate->output, "\tERROR: You don't have permission to do that\n");
				break;
			case EDQUOT:
				fprintf(ostate->output, "\tERROR: You have exhausted your diskspace quota\n");
				break;
			case EINTR:
				fprintf(ostate->output, "\tERROR: Attempt to create file '%s' was interrupted\n", pszFilename);
				break;
			case EMFILE:
			case ENFILE:
				fprintf(ostate->output, "\tERROR: Too many files open to attempt to create file '%s'\n", pszFilename);
				break;
			case ENAMETOOLONG:
				fprintf(ostate->output, "\tERROR: '%s' is too long for a file name\n", pszFilename);
				break;
			case ENOSPC:
				fprintf(ostate->output, "\tERROR: The disk is full\n");
				break;
			case EROFS:
				fprintf(ostate->output, "\tERROR: Filesystem is read-only. Can't create file '%s'\n", pszFilename);
				break;
			case EEXIST:
				fprintf(ostate->output, "\tERROR: File '%s' already exists\n", pszFilename);
				break;	
			default:
				{
					char *pszError = strerror(errno);
					fprintf(ostate->output, "\tERROR: Unknown error creating file '%s'\n", pszFilename);
					fprintf(ostate->output, "\tCause: %s\n", pszError);
				}
				break;
			}

			return 1;
		}

		/* Don't leak the file handle. */
		close(dFile);

		fprintf(ostate->output, "Successfully created file '%s'\n", pszFilename);
		return 0;
	}
}

HANDLECOM(rm) {
	/* Reinit getopt. */
	optind = 1;
	/* Handle options. */
	while (1) {

		/* The current option & long option. */
		int opt, optidx;

		/* Our usage message. */
		char *pszUsage = "Usage: rm [-h] [--help] <file-name>";

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
			return 1;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
			fprintf(ostate->output, "%s\n", pszUsage);
			return 1;

		}
	}

	{
		/* The specified filename. */
		char *pszFilename;

		if(argc <= (optind)) {
			fprintf(ostate->output, "\tERROR: Must provide the file to delete as an argument\n");
			return 1;
		}

		pszFilename = argv[optind];

		/* Ensure the file exists. */
		{
			/* Scratch struct. */
			struct stat buf;

			if(fstatat(ostate->fWorkingDir, pszFilename, &buf, 0) != 0) {
				fprintf(ostate->output, "\tERROR: Could not check if file '%s' exists\n", pszFilename);
				return 1;
			}

			if(!S_ISREG(buf.st_mode)) {
				fprintf(ostate->output, "\tERROR: '%s' is not a regular file\n", pszFilename);
				return 1;
			}
		}

		{
			char *pszLine;

			size_t llen, lread;

			fprintf(ostate->output, "Are you sure you want to delete this file? (y/n) ");

			llen = getline(&pszLine, &llen, ostate->strem);

			switch(rpmatch(pszLine)) {
			case 0:
				/* Don't delete the file. */
				return 0;
			case 1:
				/* Delete the file. */
				break;
			case -1:
			default:
				/* Incorrect response. */
				fprintf(ostate->output, "\tERROR: Unrecognized response\n");
				return 1;
			}
		}

		if(unlinkat(ostate->fWorkingDir, pszFilename, 0) == -1) {
			switch(errno) {
			case EACCES:
			case EFAULT:
			case EPERM:
				fprintf(ostate->output, "\tERROR: You do not have permission to delete '%s'\n", pszFilename);
				break;
			case EROFS:
				fprintf(ostate->output, "\tERROR: Filesystem is read-only, can't delete file '%s'\n", pszFilename);
				break;
			default:
				{
					char *pszError = strerror(errno);
					fprintf(ostate->output, "\tERROR: Unknown error attempting to delete file '%s'\n", pszFilename);
					fprintf(ostate->output, "\tCause: %s\n", pszError);
				}
				break;
			}

			return 1;
		}

		fprintf(ostate->output, "Successfully deleted file '%s'\n", pszFilename);
		return 0;
	}
}
