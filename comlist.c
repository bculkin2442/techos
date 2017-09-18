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
		struct command *com = list->commands[i];

		/* Free its components, then it. */
		free(com->name);
		free(com->brief);
		free(com);
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
void addcommand(struct comlist *list, char *comname, char *comdesc, comfun_t handler) {
	/* Intern the name. */
	internkey comkey = internstring(list->interncoms, comname);
	/* Fail if the request failed. */
	assert(comkey > 0);

	if(list->comspace == list->comcount) {
		/* Allocate more space for the list. */
		list->comspace *= 2;
		list->commands = realloc(list->commands, sizeof(struct command *) * list->comspace);
		/* Fail if request fails. */
		assert(list->commands != NULL);
	}

	/* Allocate a new command. */
	struct command *com = malloc(sizeof(struct command));
	/* Fail if request fails. */
	assert(com != NULL);

	/* Initialize the command. */
	com->name   = (char *)strdup(comname);
	com->brief  = (char *)strdup(comdesc);
	com->comfun = handler;

	/* Insert it into the list. */
	list->commands[comkey - 1]  = com;
	list->comcount             += 1;
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

void printcommands(struct comlist *list, FILE *fle) {
	int i;
	for(i = 0; i < list->comcount; i++) {
		struct command *com = list->commands[i];

		fprintf(fle, "\t%s\t%s\n", com->name, com->brief);
	}
}
