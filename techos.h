#ifndef TECHOS_HEADER
#define TECHOS_HEADER

#include "libs/argparser.h"

#include "osstate.h"
#include "command.h"

/*
 * Main TechOS header.
 */

/* List of all commands. */
extern struct comlist *all_commands;

/* The major/minor version of TechOS. */
extern const int major_ver;
extern const int minor_ver;

/*
 * Main command handler.
 *
 * Loops reading commands and handling them until EOF or an exit command is
 * handled.
 */
void comhan(struct osstate *);

/*
 * Executes a command from a line of input.
 *
 * The reason this is seperated is so that things like file input and aliases
 * (if we want them) can be more easily implemented.
 */
int handleline(struct osstate *, char *);

/*
 * Command executor.
 *
 * Gathers the CLI args for a command, and then executes it.
 */
int execcom(struct command *, struct cliargs, char *, struct osstate *);

/* Log a user in. */
void loginuser(struct osstate *);
#endif
