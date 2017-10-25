#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "osstate.h"

#include "pcb.h"
#include "pcbinternals.h"

#include "procexecute.h"

#include "dispatchcmds.h"

/* Do the actual work of dispatching processes. */
static int dodispatch(struct osstate *ostate) {
	while(candispatch(ostate->pPCBstat) > 0) {
		/* The PCB being dispatched. */
		struct pcb *pPCB;

		/* The path to the file the image comes from. */
		const char *pszImage;
		/* The file the image comes from. */
		FILE *pfImage;
		/* The name of the process being dispatched. */
		const char *pszName;

		pPCB = (struct pcb *)getdisppcb(ostate->pPCBstat);

		if(pPCB == NULL) {
			/* Something caused a dispatchable PCB to disappear. */
			fprintf(ostate->output, "WARNING: Dispatchable process went *poof*\n");
			continue;
		}

		/* Get the name of the process. */
		pszName = lookupkey(ostate->pPCBstat->ptPCBNames, pPCB->kName);

		/* Get the path to the image. */
		pszImage = lookupkey(ostate->pPCBstat->ptPCBImages, pPCB->kImage);
		/* Open the image. */
		pfImage = fopen(pszImage, "r");
		if(pfImage == NULL) {
			fprintf(ostate->output, "WARNING: Could not open image '%s' for process '%s' (id no. %d)\n",
					pszImage, pszName, pPCB->id);
			continue;
		}

		/* The process is now running. */
		pPCB->status = PCB_RUNNING;

		fprintf(ostate->output, "Executing process '%s' (id no. %d), starting at offset %d\n", pszName, pPCB->id, pPCB->offset);

		/* Dispatch the process. */
		pPCB->offset = executeimage(pfImage, pPCB->offset);

		/* The process finished. */
		if(pPCB->offset == 0) {
			/* The process finished. */
			fprintf(ostate->output, "Process '%s' (id no. %d) finished execution\n", pszName, pPCB->id);
			killpcb(pPCB);
		} else {
			/* The return from inserting the PCB. */
			enum pcberror ret;

			/* The process stopped. */
			fprintf(ostate->output, "Process '%s' (id no. %d) blocked at offset %d\n", pszName, pPCB->id, pPCB->offset);

			pPCB->status = PCB_BLOCKED;

			ret = insertpcb(ostate->pPCBstat, pPCB);

			switch(ret) {
			case PCBSUCCESS:
				break;
			case PCBINVSUSP:
				fprintf(ostate->output, "WARNING: Process '%s' (id no. %d) was dropped on the floor because of an invalid suspension status (%d)\n", pszName, pPCB->id, pPCB->susp);
				break;
			case PCBINVSTAT:
				fprintf(ostate->output, "WARNING: Process '%s' (id no. %d) was dropped on the floor because of an invalid run status (%d)\n", pszName, pPCB->id, pPCB->status);
				break;
			case PCBRUNNING:
				fprintf(ostate->output, "WARNING: Process '%s' (id no. %d) was dropped on the floor because it was improperly marked as running\n", pszName, pPCB->id);
				break;
			case PCBINQUEUE:
				fprintf(ostate->output, "WARNING: Process '%s' (id no. %d) was dropped on the floor because it was improperly inserted into a queue\n", pszName, pPCB->id);
				break;
			default:
				/* Shouldn't happen. */
				assert(0);
			}
		}

		/* Close the file. */
		fclose(pfImage);
	}

	return 0;
}

/* Handle dispatching PCBs. */
HANDLECOM(dispatch) {
	/* The number of dispatchable PCBs available. */
	int ndisp;

	ndisp = candispatch(ostate->pPCBstat);

	if(ndisp == 0) {
		fprintf(ostate->output, "No processes to dispatch\n");
		return 0;
	}

	fprintf(ostate->output, "Dispatching %d processes\n", ndisp);

	return dodispatch(ostate);
}
