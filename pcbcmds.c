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
	
	//current option and long option
	int opt, optidx;
	optind = 1;
	
	while(1)
	{
	
		char *usage = "Usage: mkpcb [] [--help]\n";
		
	}
	
	int priority = argv[2];
	
	if(priority < 0 || priority > 9)
	{
		fprintf("Priority entered is out of bounds.");
		return 1;
	}
	

	makepcb(ostate->pPCBStat, argv[1], classEnum, priority);
	
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
