#ifndef TECHOS_COMMANDH
#define TECHOS_COMMANDH

#include "osstate.h"

/* Type of a command handler. */
typedef int (*comfun_t)(int, char **, char *, struct osstate *);

/* 
 * The various types of commands that are possible.
 */
enum commandtype {
	/* Represents a normal command, represented by a 'struct command'. */
	CT_NORMAL,
};

/* Represents a command. */
struct command {
	/*
	 * The type of the command. Should be CT_NORMAL if this is an actual
	 * command.
	 */
	enum commandtype type;

	/* The name of the command. */
	char *name;

	/* A brief description of the command. */
	char *brief;

	/* The function that implements the command. */
	comfun_t comfun;
};

/* An invalid command. */
static struct command INVALID_COMMAND = {CT_NORMAL, "invalid", "", NULL};

/* Declare a command. */
#define DECLCOM(name) int handle_##name(int, char **, char *, struct osstate *)

/* Define a command handler. */
#define HANDLECOM(nam) int handle_##nam(int argc, char **argv, char *argl, struct osstate *ostate)

/* Check if a command that only takes the 'help' argument recieved it. */
int checkhelpargs(int, char **, char *, struct osstate *);

/* Allocate/initialize a normal (CT_NORMAL) command. */
struct command *makecommand(char *, char *, comfun_t);
/* Deinitialize/deallocate a command. */
void            killcommand(struct command *);

#endif
