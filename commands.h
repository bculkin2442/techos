struct command {
	const char *name;

	int (*comfun)(int, char **);
};

/*
 * Handle an attempt to exit.
 *
 * Prompts to make sure the user wants to exit.
 *
 * Returns 1 for a confirmed exit, 0 to continue running commands.
 */
int handle_exit(int, char **);

static struct command commands[] = {
	{"exit", &handle_exit},
	{"invalid", NULL},
};

/*
 * The number of valid commands.
 */
static const int NUM_COMMANDS = 1;

/*
 * The max no. of arguments a command can take.
 *
 * Remember the first arg. of a command is its name.
 */
static const int MAX_ARG_COUNT = 256;
