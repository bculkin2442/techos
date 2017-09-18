#ifndef TECHOS_COMLISTH
#define TECHOS_COMLISTH

#include "command.h"

/*
 * Represents a list of commands.
 */
struct comlist;

/* Create/initialize a comlist. */
struct comlist *makecomlist();
/* Deinitialize/destroy a comlist. */
void            killcomlist(struct comlist *);

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
void addcommand(struct comlist *, char *, char *, comfun_t);

/* 
 * Get a command based off of its name.
 *
 * Returns the INVALID_COMMAND if none by that name exist.
 */
struct command *getcommand(struct comlist *, char *);

/*
 * Execute a function for every command in a list.
 */
void foreachcommand(struct comlist *, void (*comitr)(struct command *));
#endif
