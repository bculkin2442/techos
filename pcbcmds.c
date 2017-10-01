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

#include "pcb.h"
#include "pcbinternals.h"
#include "pcbcmds.h"

/* Print a PCB. */
void printpcb(struct pcb *pPCB, void *pvState) {
	/* The OS state. */
	struct osstate *ostate;

	/* The name of the PCB. */
	const char *pszPCBName;
	/* The class of the PCB. */
	const char *pszPCBClass;
	/* The status of the PCB. */
	const char *pszPCBStatus;
	/* The suspension status of the PCB. */
	const char *pszPCBSusp;
	
	/* Cast arg. */
	ostate = (struct osstate *)(pvState);

	/* Get the PCB name. */
	pszPCBName = lookupkey(ostate->pPCBstat->ptPCBNames, pPCB->kName);

	/* Get the PCB class. */
	switch(pPCB->clas) {
	case PCB_SYSTEM:
		pszPCBClass = "System";
		break;
	case PCB_APPLICATION:
		pszPCBClass = "Application";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}
	
	/* Get the PCB status. */
	switch(pPCB->status) {
	case PCB_READY:
		pszPCBStatus = "Ready";
		break;
	case PCB_RUNNING:
		pszPCBStatus = "Running";
		break;
	case PCB_BLOCKED:
		pszPCBStatus = "Blocked";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Get the PCB suspension status. */
	switch(pPCB->susp) {
	case PCB_SUSPENDED:
		pszPCBSusp = "Yes";
		break;
	case PCB_FREE:
		pszPCBSusp = "No";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	/* Print basic PCB information. */
	fprintf(ostate->output, "PCB ID:           %d\n", pPCB->id);
	fprintf(ostate->output, "PCB Name:         %s\n", pszPCBClass);
	fprintf(ostate->output, "PCB Class:        %s\n", pszPCBName);
	fprintf(ostate->output, "PCB Priority:     %d\n", pPCB->priority);

	/* Print PCB status information. */
	fprintf(ostate->output, "PCB Status:       %s\n", pszPCBStatus);
	fprintf(ostate->output, "Is PCB Suspended: %s\n", pszPCBSusp);
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
	
	int priority = atoi(argv[2]);
	
	if(priority < 0 || priority > 9)
	{
		fprintf(ostate->output, "Priority entered is out of bounds.");
		return 1;
	}
	

	struct pcb *madePCB = makepcb(ostate->pPCBstat, argv[1], class, priority);

	insertpcb(ostate->pPCBstat, madePCB);
	
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
	
	enum pidopt idtype = PID_NAME;
	
	while(1)
	{
		char *usage = "Usage: rmpcb [name] [-h] [--help] [--proc]<name|num>\n";
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
	
	
	struct pcb *foundPCB = findpcbname(ostate->pPCBstat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBstat, foundPCB);
	free(foundPCB);
	
	return 0;
}

HANDLECOM(blpcb) {
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: blpcb [name] [-h] [--help] [--proc]<name|num>\n";
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
	
	struct pcb *foundPCB = findpcbname(ostate->pPCBstat, argv[1]);
	
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
	
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBstat, foundPCB);
	foundPCB->status = PCB_BLOCKED;
	insertpcb(ostate->pPCBstat, foundPCB);
	
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
	
	struct pcb *foundPCB = findpcbname(ostate->pPCBstat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBstat, foundPCB);
	foundPCB->status = PCB_READY;
	insertpcb(ostate->pPCBstat, foundPCB);
	
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
	
	struct pcb *foundPCB = findpcbname(ostate->pPCBstat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBstat, foundPCB);
	foundPCB->susp = PCB_SUSPENDED;
	insertpcb(ostate->pPCBstat, foundPCB);
	
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
	
	struct pcb *foundPCB = findpcbname(ostate->pPCBstat, argv[1]);
	if(foundPCB == NULL){
		fprintf(ostate->output, "\tERROR: PCB name can not be found\n");
		return 1;
	}
	
	removepcb(ostate->pPCBstat, foundPCB);
	foundPCB->susp = PCB_FREE;
	insertpcb(ostate->pPCBstat, foundPCB);
	
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

				nprocs = 0                                 +
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

				nprocs = 0                                   +
					 ostate->pPCBstat->pqBlocked->nprocs +
					 ostate->pPCBstat->pqsBlocked->nprocs;

				fprintf(ostate->output, "No. of Ready Processes: %d\n", nprocs);
				foreachpcb(ostate->pPCBstat->pqReady,
						&printpcb, ostate);
				foreachpcb(ostate->pPCBstat->pqsReady,
						&printpcb, ostate);
			}
			break;
		case QU_ALL:
			{
				/* No. of processes. */
				int nprocs;
				nprocs = 0                                    + 
					 ostate->pPCBstat->pqReady->nprocs    +
					 ostate->pPCBstat->pqsReady->nprocs   + 
					 ostate->pPCBstat->pqBlocked->nprocs  +
					 ostate->pPCBstat->pqsBlocked->nprocs +

				fprintf(ostate->output, "No. of Ready Processes: %d\n", nprocs);
				foreachpcb(ostate->pPCBstat->pqReady,
						&printpcb, ostate);
				foreachpcb(ostate->pPCBstat->pqBlocked,
						&printpcb, ostate);
				foreachpcb(ostate->pPCBstat->pqReady,
						&printpcb, ostate);
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
