#ifndef TECHOS_COMMANDSH
#define TECHOS_COMMANDSH

#include "osstate.h"
#include "command.h"
#include "comlist.h"

#include "datecmds.h"

/*
 * Header for commands.
 */

/* Initialize data for commands. */
void initcoms();

/* Add all registered commands to a comlist. */
void addcommands(struct comlist *);

/* Cleanup after commands. */
void disposecoms();

/*
 * Declare all of the command handlers.
 *
 * @TODO organize these
 */
DECLCOM(exit);
DECLCOM(version);
DECLCOM(help);

#endif
