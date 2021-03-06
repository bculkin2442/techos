#ifndef TECHOS_COMMANDSH
#define TECHOS_COMMANDSH

#include "osstate.h"
#include "command.h"
#include "comlist.h"

/* Header for commands. */

/* Initialize data for commands. */
void initcoms();

/* Add all registered commands to a comlist. */
void addcommands(struct comlist *);

/* Cleanup after commands. */
void disposecoms();

/* Declare all of the command handlers. */
DECLCOM(exit);
DECLCOM(version);
DECLCOM(help);
DECLCOM(moo);

#endif
