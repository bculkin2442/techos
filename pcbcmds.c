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

#include "pcbcmds.h"
#include "pcb.h"

HANDLECOM(mkpcb) { 
	
	int class = 1;
	
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
		char *usage = "Usage: mkpcb [name] [priority] [--class_sys|--class_app] [--help]\n";
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
	return 0;
}

HANDLECOM(blpcb) {

	return 0;
}

HANDLECOM(ubpcb) {

	return 0;
}

HANDLECOM(sspcb) {

	return 0;
}

HANDLECOM(rspcb) {

	return 0;
}

HANDLECOM(sppcb) {

	return 0;
}

HANDLECOM(shpcb) {
	return 0;
}
