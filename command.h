#ifndef TECHOS_COMMANDH
#define TECHOS_COMMANDH

#include "osstate.h"

/* Type of a command handler. */
typedef int (*comfun_t)(int, char **, char *, struct osstate *);

/* Represents a command. */
struct command {
	/* The name of the command. */
	char *name;

	/* A brief description of the command. */
	char *brief;

	/* The function that implements the command. */
	comfun_t comfun;
};

/* An invalid command. */
static struct command INVALID_COMMAND = {"invalid", "", NULL};
#endif
