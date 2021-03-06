#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "libs/intern.h"

#include "osstate.h"
#include "command.h"
#include "commands.h"

#include "pcb.h"
#include "pcbinternals.h"
#include "pcbcmds.h"

/*
 * @TODO 10/25/17 Ben Culkin :InsertPCBCheck
 * 	Make sure the return value of insertpcb is checked every time it is
 * 	called.
 */
/* Handle creating a PCB. */
HANDLECOM(mkpcb) { 
	/* Class of the PCB. */
	enum pcbclass clas;
	/* Priority of the PCB. */
	int priority;
	/* Name of the PCB. */
	char *pszPCBName;

	/* Current option/long option. */
	int opt, optidx;

	/* Set default options. */
	clas       = PCB_APPLICATION;
	pszPCBName = NULL;

	/* Reinit getopt. */
	optind = 1;

	/* Process options. */
	while(1) {
		/* Usage message. */
		char *usage = "Usage: mkpcb [-cnh] [-c|--class (app|application)|(system|sys)] [--help] [-n|--name <proc-name>] <image-name> <priority>\n";

		/* The long options we take. */
		static struct option opts[] = {
			/*class options*/
			{"class", required_argument, 0, 'c'},

			/* Misc. options. */
			{"help", no_argument,       0, 'h'},
			{"name", required_argument, 0, 'n'},

			/* Terminating option. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "c:n:h", opts, &optidx);
		
		/* Break if we've processed every option. */
		if(opt == -1) break;

		/* Handle options. */
		switch(opt) {
		case 0:
			/*
			 * Long-options with corrsponding short-options are
			 * handled by the corresponding short option. 
			 */
			switch(optidx) {
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'c':
			if(strcmp(optarg, "application") == 0 ||
			   strcmp(optarg, "app")         == 0) {
				clas = PCB_APPLICATION;
			} else if(strcmp(optarg, "system") == 0 ||
				  strcmp(optarg, "sys")    == 0) {
				clas = PCB_SYSTEM;
			} else {
				fprintf(ostate->output, "ERROR: Invalid PCB class '%s' (valid classes are 'app'/'application' and 'sys/system')\n", optarg);

				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		case 'n':
			/* Free an already set name. */
			if(pszPCBName != NULL) {
				free(pszPCBName);
			}
			/* 
			 * @NOTE
			 *	Do we need to duplicate this?
			 */
			pszPCBName = (char *)strdup(optarg);
			assert(pszPCBName != NULL);
			break;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}

	}

	/* Check if we have a priority as the second non-arg option. */
	if(argc >= (optind + 2)){
		/* Priority argument. */
		char *prarg;
		prarg = argv[optind + 1];

		/* Parse priority. */
		if(sscanf(prarg, "%d", &priority) < 1) {
			fprintf(ostate->output, "ERROR: '%s' is not a valid priority (must be an integer)\n", prarg);
			return 1;
		}
	} else {
		fprintf(ostate->output, "ERROR: Priority not given.\n");
		return 1;
	}

	/* Check the PCB priority is valid. */
	if(priority < PCB_MINPRIOR || priority > PCB_MAXPRIOR) {
		fprintf(ostate->output, "ERROR: Priority entered is out of bounds (must be between %d and %d)\n", PCB_MINPRIOR, PCB_MAXPRIOR);
		return 1;
	}

	/* Bind the PCB image. */
	char *pszPCBImage = argv[optind];

	/* Validate that the image referenced exists. */
	{
		struct stat scratch;

		/* Error if there isn't a normal file for the image. */
		if(stat(pszPCBImage, &scratch) == -1) {
			fprintf(ostate->output, "ERROR: No image found for process '%s'\n", pszPCBImage);
			return 1;
		} else if(!S_ISREG(scratch.st_mode)) {
			fprintf(ostate->output, "ERROR: Invalid image for process '%s'\n", pszPCBImage);
			return 1;
		}
	}

	/* Set the PCB name correctly. */
	if(pszPCBName == NULL) {
		pszPCBName = pszPCBImage;
	}

	/* Create the PCB. */
	struct pcb *madePCB = makepcb(ostate->pPCBstat, pszPCBName, pszPCBImage, clas, priority);
	if(madePCB == NULL) {
		fprintf(ostate->output, "INTERNAL ERROR: PCB named '%s' created as null.\n", pszPCBImage);
		return 1;
	}

	{
		/* Return from insertpcb. */
		enum pcberror pcbstat;

		pcbstat = insertpcb(ostate->pPCBstat, madePCB);

		switch(pcbstat) {
		case PCBSUCCESS:
			return 0;
		case PCBINVSUSP:
			fprintf(ostate->output, "INTERNAL ERROR: New PCB %d (named '%s') has invalid suspension type %d\n", madePCB->id, pszPCBImage, madePCB->susp);
			break;
		case PCBINVSTAT:
			fprintf(ostate->output, "INTERNAL ERROR: New PCB %d (named '%s') has invalid run state %d\n",
					madePCB->id, pszPCBImage, madePCB->status);
			break;
		case PCBRUNNING:
			fprintf(ostate->output, "INTERNAL ERROR: New PCB %d (named '%s') is already running\n",
					madePCB->id, pszPCBImage);
			break;
		case PCBINQUEUE:
			fprintf(ostate->output, "INTERNAL ERROR: New PCB %d (named '%s') is already in a queue\n",
					madePCB->id, pszPCBImage);
			break;
		default:
			fprintf(ostate->output, "INTERNAL ERROR: Unknown return %d from attempting to insert new PCB %d (named '%s)\n", pcbstat, madePCB->id, pszPCBImage);
		}

		return 1;
	}
}

/* Handle deleting a PCB. */
/*
 * @NOTE
 *	This command has been removed from the list of commands, but the
 *	function is sticking around in case something we want to do is remove
 *	processes that are currently executing.
 */
HANDLECOM(rmpcb) {
	/* Current option/long option. */
	int opt, optidx;

	/*The pcb to remove*/
	struct pcb *pPCB;

	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};

	/* Reinit getopt. */
	optind = 1;

	/* The default PCB id type. */
	enum pidopt idtype = PID_NAME;
	while(1) {
		/* Our usage method. */
		char *usage = "Usage: rmpcb [name] [-hp] [--help] [--proc name|num] <proc-name>|<proc-id>\n";
		/* The long options we take. */
		static struct option opts[] = {
			/*Mode options*/ 
			{"proc",  required_argument, 0, 'p'},

			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

			/* Terminating option. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "hp:", opts, &optidx);
		/* Break if we've processed every option. */
		if(opt == -1) break;

		/* Handle options. */
		switch(opt) {
		case 0:
			/*
			 * Long options with corresponding short options are
			 * handled by their short options.
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
		case 'p':
			if(strcmp(optarg, "name") == 0) {
				idtype = PID_NAME;
			} else if(strcmp(optarg, "num") == 0) {
				idtype = PID_NUM;
			} else {
				fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
				return 1;
			}
			break;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}	
	}

	/* Grab the PCB. */
	switch(idtype) {
	case PID_NAME:
		if(optind < argc) {
			char *pszPCBName = argv[optind];

			pPCB = findpcbname(ostate->pPCBstat, pszPCBName);
			if(pPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with name '%s'\n", pszPCBName);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB name as argument\n");
			return 1;
		}
		break;
	case PID_NUM:
		if(optind < argc) {
			/* PCB ID. */
			int pcbid;
			/* Tentative PCB ID. */
			char *pszPCBID;

			pszPCBID = argv[optind];
			if(sscanf(pszPCBID, "%d", &pcbid) < 1) {
				fprintf(ostate->output, "ERROR: '%s' is not a valid PCB id (must be a positive integer)\n", pszPCBID);
				return 1;
			}
			if(pcbid < 0) {
				fprintf(ostate->output, "ERROR: PCB ID %d is not valid (must be a positive integer)\n", pcbid);
				return 1;
			}

			pPCB = findpcbnum(ostate->pPCBstat, pcbid);
			if(pPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with ID %d\n", pcbid);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB ID as argument.\n");
			return 1;
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Remove and free the PCB. */
	removepcb(ostate->pPCBstat, pPCB);
	killpcb(pPCB);

	return 0;
}

HANDLECOM(sspcb) {
	/* Current option & long option. */
	int opt, optidx;

	/* Reinit getopt. */
	optind = 1;

	/* Types of locating PCBs. */
	/*
	 * @NOTE
	 * 	Should this enum get put somewhere global?
	 */
	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};

	/* Set sensible default options. */
	enum pidopt idtype = PID_NAME;

	while(1) {
		/* Our usage message. */
		char *usage = "Usage: sspcb [name] [-h] [--help] [--proc name|num] <proc-name>|<proc-id>\n";

		/* The long options we take. */
		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 'h'},

			/*Mode options*/
			{"proc", required_argument, 0 , 'p'},

			/* Terminating option. */
			{0, 0, 0, 0},
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "hp:", opts, &optidx);

		/* Break if we've processed every option. */
		if(opt == -1) break;

		/* Handle options. */
		switch(opt) {
		case 0:
			/* Handle long options. */
			switch(optidx) {
				/* 
				 * Long options are handled by their
				 * corresponding short option.
				 */
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		case 'p':
			if(strcmp(optarg, "name") == 0) {
				idtype = PID_NAME;
			} else if(strcmp(optarg, "num") == 0) {
				idtype = PID_NUM;
			} else {
				fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
				return 1;
			}
			break;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}

	}

	{
		/* The PCB we're looking for. */
		struct pcb *foundPCB;

		/* Grab the PCB. */
		switch(idtype) {
		case PID_NAME:
			if(optind < argc) {
				char *pszPCBName = argv[optind];
				foundPCB = findpcbname(ostate->pPCBstat, pszPCBName);
				if(foundPCB == NULL) {
					fprintf(ostate->output, "ERROR: No PCB with name '%s'\n", pszPCBName);
					return 1;
				}
			} else {
				fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
				return 1;
			}
			break;
		case PID_NUM:
			if(optind < argc) {
				int pcbid = atoi(argv[optind]);
				foundPCB = findpcbnum(ostate->pPCBstat, pcbid);
				if(foundPCB == NULL) {
					fprintf(ostate->output, "ERROR: No PCB with ID '%d'\n", pcbid);
					return 1;
				}
			} else {
				fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
				return 1;
			}
			break;
		default:
			/* Shouldn't happen. */
			assert(0);
		}

		removepcb(ostate->pPCBstat, foundPCB);
		foundPCB->susp = PCB_SUSPENDED;
		insertpcb(ostate->pPCBstat, foundPCB);
	}

	return 0;
}

HANDLECOM(rspcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;

	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};

	enum pidopt idtype = PID_NAME;

	while(1)
	{
		char *usage = "Usage: rspcb [name] [-h] [--help] [--proc name|num] <proc-name>|<proc-id>\n";
		/* The long options we take. */
		static struct option opts[] = {

			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/*Mode options*/
			{"proc", required_argument, 0 , 0},

			/* Terminating option. */
			{0, 0, 0, 0},
		};

		//get an option
		opt = getopt_long(argc, argv, "h", opts, &optidx);

		//break if we've processed every option
		if(opt == -1) break;

		//Handle options
		switch(opt)
		{
		case 0:
			//Long options
			switch(optidx)
			{
			case 0://Help
				fprintf(ostate->output, "%s\n", usage);
				return 0;
			case 1://SH_PROC
				if(strcmp(optarg, "name") == 0) {
					idtype = PID_NAME;
				}
				else if(strcmp(optarg, "num") == 0) {
					idtype = PID_NUM;
				}
				else {
					fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
					return 1;
				}
				break;
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
			//Short options	
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}	

	}

	struct pcb *foundPCB;

	switch(idtype) {
	case PID_NAME:
		if(optind < argc) {
			char *pszPCBName = argv[optind];
			foundPCB = findpcbname(ostate->pPCBstat, pszPCBName);
			if(foundPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with name '%s'\n", pszPCBName);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
			return 1;
		}
		break;
	case PID_NUM:
		if(optind < argc) {
			int pcbid = atoi(argv[optind]);
			foundPCB = findpcbnum(ostate->pPCBstat, pcbid);
			if(foundPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with ID '%d'\n", pcbid);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
			return 1;
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	removepcb(ostate->pPCBstat, foundPCB);
	foundPCB->susp = PCB_FREE;
	insertpcb(ostate->pPCBstat, foundPCB);

	return 0;
}

HANDLECOM(sppcb) {

	//current option and long option
	int opt, optidx;
	optind = 1;

	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};

	enum pidopt idtype = PID_NAME;

	int priorityN;

	while(1)
	{
		char *usage = "Usage: rspcb [name] [priority] [-h] [--help] [--proc name|num] <proc-name>|<proc-id>\n";
		/* The long options we take. */
		static struct option opts[] = {

			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/*Mode options*/
			{"proc", required_argument, 0 , 0},

			/* Terminating option. */
			{0, 0, 0, 0},
		};

		//get an option
		opt = getopt_long(argc, argv, "h", opts, &optidx);

		//break if we've processed every option
		if(opt == -1) break;

		//Handle options
		switch(opt)
		{
		case 0:
			//Long options
			switch(optidx)
			{
			case 0://Help
				fprintf(ostate->output, "%s\n", usage);
				return 0;
			case 1://SH_PROC
				if(strcmp(optarg, "name") == 0) {
					idtype = PID_NAME;
				}
				else if(strcmp(optarg, "num") == 0) {
					idtype = PID_NUM;
				}
				else {
					fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
					return 1;
				}
				break;
			default:
				fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
				fprintf(ostate->output, "%s\n", usage);
				return 1;
			}
			break;
			//Short options	
		case 'h':
			fprintf(ostate->output, "%s\n", usage);
			return 0;
		default:
			fprintf(ostate->output, "\tERROR: Invalid command-line argument.\n");
			fprintf(ostate->output, "%s\n", usage);
			return 1;
		}	

	}

	struct pcb *foundPCB;

	switch(idtype) {
	case PID_NAME:
		if(optind < argc) {
			char *pszPCBName = argv[optind];
			foundPCB = findpcbname(ostate->pPCBstat, pszPCBName);
			if(foundPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with name '%s'\n", pszPCBName);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
			return 1;
		}
		break;
	case PID_NUM:
		if(optind < argc) {
			int pcbid = atoi(argv[optind]);
			foundPCB = findpcbnum(ostate->pPCBstat, pcbid);
			if(foundPCB == NULL) {
				fprintf(ostate->output, "ERROR: No PCB with ID '%d'\n", pcbid);
				return 1;
			}
		} else {
			fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
			return 1;
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	if(argc >= (optind + 2)){
		priorityN = atoi(argv[optind + 1]);
	} else {
		fprintf(ostate->output, "ERROR: Priority not given\n");
		return 1;
	}

	if(priorityN < PCB_MINPRIOR || priorityN > PCB_MAXPRIOR)
	{
		fprintf(ostate->output, "Priority entered is out of bounds.\n");
		return 1;
	}

	foundPCB->priority = priorityN;

	return 0;
}

HANDLECOM(shpcb) {
	/* Mode options. */
	enum showopt { 
		/* Show a specific PCB. */
		SHOW_PCB,
		/* Show a PCB queue. */
		SHOW_QUEUE
	};
	/* Queue options. */
	enum queueopt {
		/* Show the ready queues. */
		QU_READY,
		/* Show the blocked queues. */
		QU_BLOCKED,
		/* Show all the queues. */
		QU_ALL
	};
	/* Process ID options. */
	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};

	/* Chosen mode. */
	enum showopt  mode   = SHOW_QUEUE;
	/* Chosen queue. */
	enum queueopt queue  = QU_ALL;
	/* Chosen ID method. */
	enum pidopt   idtype = PID_NAME;

	/* Reinit getopt. */
	optind = 1;

	/* Parse CLI args. */
	while(1) {
		/* Enum declaration for long options. */
		enum shpcbopt {
			/* Help option. */
			SH_HELP = 0,

			/* Mode options. */
			SH_MODE,       /* Specify the command mode. */
			SH_QUEUE,      /* Specify the queue to show. */
			SH_PROC,       /* Specify the way to locate a process. */

		};

		/* The current option, and the current long option. */
		int opt, optidx;

		/* Our usage message. */
		static const char *usage = "Usage: shpcb [-h] [--help] [--mode pcb|queue] [--queue ready|blocked|all] [--proc name|num] <proc-name>|<proc-id>\n";


		/* The long options we take. */
		/* 
		 * :OptHandling
		 */
		static struct option opts[] = {
			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/* Mode options. */
			{"mode",  required_argument, 0, 0},
			{"queue", required_argument, 0, 0},
			{"proc",  required_argument, 0, 0},

			/* Terminating argument. */
			{0, 0, 0, 0}
		};

		/* Get an option. */
		opt = getopt_long(argc, argv, "h", opts, &optidx);
		/* Break if there are no more options. */
		if(opt == -1) break;

		/* Handle options. */
		/* :OptHandling */
		switch(opt) {
		case 0:
			/* Handle long options. */
			switch(optidx) {
			case SH_HELP:
				fprintf(ostate->output, "%s\n", usage);
				return 0;
			case SH_MODE:
				if(strcmp(optarg, "pcb") == 0) {
					mode = SHOW_PCB;
				} else if(strcmp(optarg, "queue") == 0) {
					mode = SHOW_QUEUE;
				} else {
					fprintf(ostate->output, "ERROR: Invalid mode '%s'. Valid modes are 'pcb' and 'queue'\n", optarg);
					return 1;
				}
				break;
			case SH_QUEUE:
				if(strcmp(optarg, "ready") == 0) {
					queue = QU_READY;
				} else if(strcmp(optarg, "blocked") == 0) {
					queue = QU_BLOCKED;
				} else if(strcmp(optarg, "all") == 0) {
					queue = QU_ALL;
				} else {
					fprintf(ostate->output, "ERROR: Invalid queue '%s'. Valid queues are 'ready', 'blocked' and 'all'\n", optarg);
					return 1;
				}
				break;
			case SH_PROC:
				if(strcmp(optarg, "name") == 0) {
					idtype = PID_NAME;
				} else if(strcmp(optarg, "num") == 0) {
					idtype = PID_NUM;
				} else {
					fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
					return 1;
				}
				break;
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
		}
	}

	switch(mode) {
	case SHOW_PCB:
		{
			/* The PCB to show. */
			struct pcb *pPCB;

			switch(idtype) {
			case PID_NAME:
				if(optind < argc) {
					char *pszPCBName = argv[optind];
					pPCB = findpcbname(ostate->pPCBstat, pszPCBName);
					if(pPCB == NULL) {
						fprintf(ostate->output, "ERROR: No PCB with name '%s'\n", pszPCBName);
						return 1;
					}
				} else {
					fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
					return 1;
				}

				break;
			case PID_NUM:
				if(optind < argc) {
					int pcbid = atoi(argv[optind]);
					pPCB = findpcbnum(ostate->pPCBstat, pcbid);
					if(pPCB == NULL) {
						fprintf(ostate->output, "ERROR: No PCB with ID '%d'\n", pcbid);
						return 1;
					}
				} else {
					fprintf(ostate->output, "ERROR: Must specify PCB name as argument.\n");
					return 1;
				}
				break;
			default:
				/* Shouldn't happen. */
				assert(0);
			}
			printpcb(pPCB, ostate);
		}
		break;
	case SHOW_QUEUE:
		switch(queue) {
		case QU_READY:
			{
				/* No. of processes. */
				int nprocs;

				nprocs = 0                                +
					ostate->pPCBstat->pqReady->nprocs +
					ostate->pPCBstat->pqsReady->nprocs;

				fprintf(ostate->output, "No. of Ready Processes: %d\n", nprocs);
				foreachpcb(ostate->pPCBstat->pqReady,
						&printpcb, ostate);
				foreachpcb(ostate->pPCBstat->pqsReady,
						&printpcb, ostate);
			}
			break;
		case QU_BLOCKED:
			{
				/* No. of processes. */
				int nprocs;

				nprocs = 0                                  +
					ostate->pPCBstat->pqBlocked->nprocs +
					ostate->pPCBstat->pqsBlocked->nprocs;

				fprintf(ostate->output, "No. of Blocked Processes: %d\n", nprocs);
				foreachpcb(ostate->pPCBstat->pqBlocked,
						&printpcb, ostate);
				foreachpcb(ostate->pPCBstat->pqsBlocked,
						&printpcb, ostate);
			}
			break;
		case QU_ALL:
			{
				/* No. of processes. */
				int nprocs;
				nprocs = 0                                   + 
					ostate->pPCBstat->pqReady->nprocs    +
					ostate->pPCBstat->pqBlocked->nprocs  +
					ostate->pPCBstat->pqsReady->nprocs   + 
					ostate->pPCBstat->pqsBlocked->nprocs;

				fprintf(ostate->output, "No. of Processes: %d\n", nprocs);
				fprintf(ostate->output, "\nReady Processes:\n");
				foreachpcb(ostate->pPCBstat->pqReady,
						&printpcb, ostate);
				fprintf(ostate->output, "\nBlocked Processes:\n");
				foreachpcb(ostate->pPCBstat->pqBlocked,
						&printpcb, ostate);
				fprintf(ostate->output, "\nSuspended Ready Processes:\n");
				foreachpcb(ostate->pPCBstat->pqsReady,
						&printpcb, ostate);
				fprintf(ostate->output, "\nSuspended Suspended Processes:\n");
				foreachpcb(ostate->pPCBstat->pqsBlocked,
						&printpcb, ostate);
			}
			break;
		default:
			/* Shouldn't happen. */
			assert(0);
		}
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}
	return 0;
}
