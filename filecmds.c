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

	return 1;
}

/* Handle removing a directory. */
HANDLECOM(rmdir) {
	/* Handle options. *////////////////////////////////////////////////////////////////////////////////////////////////
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

	if(argc >= (optind + 1)) {
		fprintf(ostate->output, "\tERROR: Must provide the directory to remove as an argument\n");
		return 1;
	}
	/*END HANDLE OPTIONS*////////////////////////////////////////////////////////////////////////////////////////////////
	
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

HANDLECOM(touch) {
	/* Handle options. *////////////////////////////////////////////////////////////////////////////////////////////////
	{
		/* Reinit getopt. */
		optind = 1;

		/* The current option & long option. */
		int opt, optidx;

		/* Our usage message. */
		char *pszUsage = "Usage: touch [-h] [--help] <directory-name>";

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

	if(argc >= (optind + 1)) {
		fprintf(ostate->output, "\tERROR: Must provide the directory to remove as an argument\n");
		return 1;
	}
	/*END HANDLE OPTIONS*////////////////////////////////////////////////////////////////////////////////////////////////
	//get the path
	char *path;
	path = argv[optind];
	
	if(openat(ostate->fWorkingDir, path, O_CREAT) != 0)
	{
		switch(errno)
		{
			case EACCES:
				printf("The requested access to the file is not allowed, or search permission is denied for one of the directories in the path prefix of pathname, or the file did not exist yet and write access to the parent directory is not allowed.");
			case EDQUOT:
				printf("Where O_CREAT is specified, the file does not exist, and the user's quota of disk blocks or inodes on the file system has been exhausted.");
			case EEXIST:
				printf("pathname already exists and O_CREAT and O_EXCL were used.");
			case EFAULT:
				printf("pathname points outside your accessible address space.");
			case EFBIG:
				printf("See EOVERFLOW.");
			case EINTR:
				printf("While blocked waiting to complete an open of a slow device");
			case EISDIR:
				printf("pathname refers to a directory and the access requested involved writing");
			case ELOOP:
				printf("Too many symbolic links were encountered in resolving pathname, or O_NOFOLLOW was specified but pathname was a symbolic link.");
			case EMFILE:
				printf("The process already has the maximum number of files open.");
			case ENAMETOOLONG:
				printf("pathname was too long.");
			case ENFILE:
				printf("The system limit on the total number of open files has been reached.");
			case ENODEV:
				printf("pathname refers to a device special file and no corresponding device exists.");
			case ENOENT:
				printf("O_CREAT is not set and the named file does not exist. Or, a directory component in pathname does not exist or is a dangling symbolic link.");
			case ENONEM:
				printf("Insufficient kernel memory was available.");
			case ENOSPC:
				printf("pathname was to be created but the device containing pathname has no room for the new file.");
			case ENOTDIR:
				printf("A component used as a directory in pathname is not, in fact, a directory, or O_DIRECTORY was specified and pathname was not a directory.");
			case ENXIO:
				printf("O_NONBLOCK | O_WRONLY is set, the named file is a FIFO and no process has the file open for reading. Or, the file is a device special file and no corresponding device exists.");
			case EOVERFLOW:
				printf("pathname refers to a regular file that is too large to be opened. The usual scenario here is that an application compiled on a 32-bit platform without -D_FILE_OFFSET_BITS=64 tried to open a file whose size exceeds (2<<31)-1 bits; see also O_LARGEFILE above. This is the error specified by POSIX.1-2001; in kernels before 2.6.24, Linux gave the error EFBIG for this case.");
			case EPERM:
				printf("The O_NOATIME flag was specified, but the effective user ID of the caller did not match the owner of the file and the caller was not privileged (CAP_FOWNER).");
			case EROFS:
				printf("pathname refers to a file on a read-only file system and write access was requested.");
			case ETXTBSY:
				printf("pathname refers to an executable image which is currently being executed and write access was requested.");
			case EWOULDBLOCK:
				printf("The O_NONBLOCK flag was specified, and an incompatible lease was held on the file");
			
		}
		
	}
	
        return 1;
}

HANDLECOM(rm) {
        return 1;
}
