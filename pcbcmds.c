#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libs/intern.h"

#include "osstate.h"
#include "command.h"
#include "commands.h"

#include "pcbcmds.h"
#include "pcb.h"

void printpcb(struct osstate *ostate, struct pcb *pPCB) {
	/* The name of the PCB. */
	char *pszPCBName;
	/* The class of the PCB. */
	char *pszPCBClass = lookupstring(ostate->pPCBstat->ptPCBNames, pPCB->kName);

	fprintf(ostate->output, "PCB ID:    %d\n", pPCB->id);
	fprintf(ostate->output, "PCB Name:  %s\n", pszPCBClass);
	fprintf(ostate->output, "PCB Class: %s\n", pszPCBName);
}

HANDLECOM(mkpcb) { 
	
	int class = 1;
	
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: mkpcb [name] [priority] [-h] [--class_sys|--class_app] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			/*class options*/
			{"class_sys",  no_argument, 0, 0},
			{"class_app",  no_argument, 0, 0},
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/* Terminating option. */
			{0, 0, 0, 0}
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
					case PCB_SYSTEM:
						class = 0;
						break;
					case PCB_APPLICATION:
						class = 1;
						break;
					case 2://Help
						fprintf(ostate->output, "%s\n", usage);
						return 0;
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
	
	int priority = argv[2];
	
	if(priority < 0 || priority > 9)
	{
		fprintf("Priority entered is out of bounds.");
		return 1;
	}
	

	pcb *madePCB = makepcb(ostate->pPCBStat, argv[1], class, priority);
	insertpcb(ostate->pPCBStat, madePCB);
	
	return 0;
}

HANDLECOM(rmpcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	enum pidopt {
		/* Locate a PCB by name. */
		PID_NAME,
		/* Locate a PCB by number. */
		PID_NUM
	};
	
	enum pidopt idtype;
	
	while(1)
	{
		char *usage = "Usage: rmpcb [name] [-h] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},
			
			/*Mode options*/ 
			{"proc",  required_argument, 0, 0},
			
			/* Terminating option. */
			{0, 0, 0, 0}
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
					if(strcmp(optarg, "name")) {
						idtype = PID_NAME;
						}
					  else if(strcmp(optarg, "num")) {
						idtype = PID_NUM;
						}
				      else {
						fprintf(ostate->output, "ERROR: Invalid process ID type '%s'. Valid ID types are 'name' and 'num'\n", optarg);
						return 1;
						}
				}
				break;
					default:
						fprintf(ostate->output, "\tERROR: Invalid command-line argument\n");
						fprintf(ostate->output, "%s\n", usage);
						return 1;
				
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
	
	if(pPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name or id can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBStat, pPCB);
	free(pPCB);
	
	return 0;
}

HANDLECOM(blpcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: blpcb [name] [-h] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},
			
			/*Mode options*/
			{"proc", required_argument, 0, 0},
			
			/* Terminating option. */
			{0, 0, 0, 0}
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
	
	pcb *foundPCB = findpcbname(ostate->pPCBStat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBStat, foundPCB);
	foundPCB->status = PCB_BLOCKED;
	insertpcb(ostate->pPCBStat, foundPCB);
	
	return 0;
}

HANDLECOM(ubpcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: ubpcb [name] [-h] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/* Terminating option. */
			{0, 0, 0, 0}
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
	
	pcb *foundPCB = findpcbname(ostate->pPCBStat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBStat, foundPCB);
	foundPCB->status = PCB_READY;
	insertpcb(ostate->pPCBStat, foundPCB);
	
	return 0;
}

HANDLECOM(sspcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: sspcb [name] [-h] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/* Terminating option. */
			{0, 0, 0, 0}
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
	
	pcb *foundPCB = findpcbname(ostate->pPCBStat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBStat, foundPCB);
	foundPCB->susp = PCB_SUSPENDED;
	insertpcb(ostate->pPCBStat, foundPCB);
	
	return 0;
}

HANDLECOM(rspcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: rspcb [name] [-h] [--help]\n";
		/* The long options we take. */
		static struct option opts[] = {
			
			/* Misc. options. */
			{"help", no_argument, 0, 0},

			/* Terminating option. */
			{0, 0, 0, 0}
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
	
	pcb *foundPCB = findpcbname(ostate->pPCBStat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBStat, foundPCB);
	foundPCB->susp = PCB_FREE;
	insertpcb(ostate->pPCBStat, foundPCB);
	
	return 0;
}

HANDLECOM(sppcb) {

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
	enum showopt  mode;
	/* Chosen queue. */
	enum queueopt queue;
	/* Chosen ID method. */
	enum pidopt   idtype;

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
			SH_PID,        /* Specify the way to locate a process. */

		};

		/* The current option, and the current long option. */
		int opt, optidx;

		/* Our usage message. */
		static const char *usage = "Usage: shpcb [-h] [--help]\n";

		/* The long options we take. */
		/* 
		 * @TODO Use the 4th argument to indicate the corresponding
		 * short option.
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
		/* @TODO add short options. */
		switch(opt) {
		case 0:
			/* Handle long options. */
			switch(optidx) {
			case SH_HELP:
				fprintf(ostate->output, "%s\n", usage);
				return 0;
			case SH_MODE:
				if(strcmp(optarg, "pcb")) {
					mode = SHOW_PCB;
				} else if(strcmp(optarg, "queue")) {
					mode = SHOW_QUEUE;
				} else {
					fprintf(ostate->output, "ERROR: Invalid mode '%s'. Valid modes are 'pcb' and 'queue'\n", optarg);
					return 1;
				}
			case SH_QUEUE:
				if(strcmp(optarg, "ready")) {
					queue = QU_READY;
				} else if(strcmp(optarg, "blocked")) {
					queue = QU_BLOCKED;
				} else if(strcmp(optarg, "all")) {
					queue = QU_ALL;
				} else {
					fprintf(ostate->output, "ERROR: Invalid queue '%s'. Valid queues are 'ready', 'blocked' and 'all'\n", optarg);
					return 1;
				}
				break;
			case SH_PROC:
				if(strcmp(optarg, "name")) {
					idtype = PID_NAME;
				} else if(strcmp(optarg, "num")) {
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
		printpcb(ostate, pPCB);
		break;
	case SHOW_QUEUE:
		switch(queue) {
		case QU_READY:
			break;
		case QU_BLOCKED:
			break;
		case QU_ALL:
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
