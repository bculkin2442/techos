#ifndef TECHOS_HEADER
#define TECHOS_HEADER

#include "libs/argparser.h"

#include "osstate.h"
#include "commands.h"

/*
 * Main TechOS header.
 */

/* The major/minor version of TechOS. */
static const int major_ver = 1;
static const int minor_ver = 2;

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
 * Command parser.
 *
 * Determines which command should be executed based off of a name.
 */
struct command parsecom(char *, struct osstate *);

/*
 * Command executor.
 *
 * Gathers the CLI args for a command, and then executes it.
 */
int execcom(struct command, struct cliargs, char *, struct osstate *);
#endif
