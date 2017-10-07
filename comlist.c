#include <assert.h>
#include <stdlib.h>

#include "libs/intern.h"
#include "command.h"
#include "comlist.h"

/*
 * Represents a list of commands.
 */
struct comlist {
	/* Dynamic array of commands. */
	struct command **commands;
	/* The available/used space in the commands array. */
	int comspace;
	int comcount;

	/* Map from command names to indexes into the commands array. */
	struct interntab *interncoms;
};

/* Allocate and initialize a command list. */
struct comlist *makecomlist() {
	/* Allocate memory for the struct. */
	struct comlist *ret = malloc(sizeof(struct comlist));
	/* Fail if allocation did. */
	assert(ret != NULL);

	/* Allocate memory for the command list. */
	ret->commands = calloc(10, sizeof(struct command));
	/* Fail if allocation did. */
	assert(ret->commands != NULL);

	/* Set the array space/available bounds. */
	ret->comspace = 10;
	ret->comcount = 0;

	/* Create the intern table for the commands. */
	ret->interncoms = makeinterntab();

	return ret;
}

/* Deinitialize and deallocate a command list. */
void killcomlist(struct comlist *list) {
	/* Free the intern table. */
	killinterntab(list->interncoms);

	/* Free the commands. */
	int i;
	for(i = 0; i < list->comcount; i++) {
		/* Get the command. */
		killcommand(list->commands[i]);
	}
	/* Free the command storage. */
	free(list->commands);

	/* Free the list itself. */
	free(list);
}

/*
 * Add a command to a command list.
 *
 * Takes 
 * - The name of the command
 * - The brief help for the command
 * - The handler for the command
 *
 * and inserts it into the list.
 */
void addcommand(struct comlist *plList, char *pszName, char *pszDesc, comfun_t pfHandler) {
	/* Intern the name. */
	internkey comkey = internstring(plList->interncoms, pszName);
	/* Fail if the request failed. */
	assert(comkey > 0);

	if(plList->comspace == plList->comcount) {
		/* Allocate more space for the list. */
		plList->comspace *= 2;
		plList->commands = realloc(plList->commands, sizeof(struct command *) * plList->comspace);
		/* Fail if request fails. */
		assert(plList->commands != NULL);
	}

	/* 
	 * Allocate a new command. We duplicate strings, so that they belong to
	 * the command. 
	 */
	struct command *pCommand = makecommand((char *)strdup(pszName), (char *)strdup(pszDesc), pfHandler);

	/* Insert it into the list. */
	plList->commands[comkey - 1]  = pCommand;
	plList->comcount             += 1;
}

/* 
 * Get a command based off of its name.
 *
 * Returns the INVALID_COMMAND if none by that name exist.
 */
struct command *getcommand(struct comlist *list, char *name) {
	/* Lookup the key we stored the command under. */
	internkey comkey = lookupstring(list->interncoms, name);
	/* Error if that's not a command. */
	if(comkey == SIINVALID) return &INVALID_COMMAND;
	/* Give the command back. */
	return list->commands[comkey - 1];
}

/*
 * Execute a function for every command in a list.
 */
void foreachcommand(struct comlist *list, void (*comitr)(struct command *)) {
	int i;
	for(i = 0; i < list->comcount; i++) comitr(list->commands[i]);
}

/* Print a command list. */
void printcommands(struct comlist *list, FILE *fle) {
	int i;
	for(i = 0; i < list->comcount; i++) {
		struct command *com = list->commands[i];

		switch(com->type) {
		case CT_NORMAL:
			fprintf(fle, "\t%s\t%s\n", com->name, com->brief);
			break;
		default:
			/* Shouldn't happen. */
			assert(0);
		}
	}
}
