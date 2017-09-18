#ifndef TECHOS_COMMANDSH
#define TECHOS_COMMANDSH

#include "osstate.h"
#include "command.h"
#include "comlist.h"

/*
 * Header for commands.
 */

/* Initialize data for commands. */
void initcoms();

/* Add all registered commands to a comlist. */
void addcommands(struct comlist *);

/* Cleanup after commands. */
void disposecoms();

/* Declare a command. */
#define DECLCOM(name) int handle_##name(int, char **, char *, struct osstate *)

/*
 * Declare all of the command handlers.
 *
 * @TODO organize these
 */
DECLCOM(exit);
DECLCOM(version);
DECLCOM(date);
DECLCOM(datefmt);
DECLCOM(setdate);
DECLCOM(help);
DECLCOM(time);

/* All the commands in the operating system. */
static struct command commands[] = {
	{"exit",    "Exit TechOS",				      &handle_exit},
	{"version", "Display version/author information",	      &handle_version},
	{"date",    "Display the current date/time",		      &handle_date},
	{"datefmt", "Set the format the date is displayed/read in",   &handle_datefmt},
	{"setdate", "Set the current date",			      &handle_setdate},
	{"time",    "Display the current time",			      &handle_time},
	{"help",    "Get help for commands, or list available ones.", &handle_help},
	{"invalid", "",						       NULL},
};

/*
 * The number of valid commands. 
 *
 * Make sure to update this whenever a new command is added.
 *
 * @TODO make this unnecessary
 */
static const int NUM_COMMANDS = 7;

#endif
