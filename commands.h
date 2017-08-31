struct command {
	const char *name;

	int (*comfun)(int, char **, char *);
};

/*
 * Initialize data for commands.
 */
void initcoms();

/*
 * Cleanup after commands.
 */
void disposecoms();

#define DECLCOM(name) int handle_##name(int, char **, char *)
/*
 * Handle an attempt to exit.
 *
 * Prompts to make sure the user wants to exit.
 */
DECLCOM(exit);
DECLCOM(version);
DECLCOM(date);
DECLCOM(datefmt);

static struct command commands[] = {
	{"exit",    &handle_exit},
	{"version", &handle_version},
	{"date",    &handle_date},
	{"datefmt", &handle_datefmt},
	{"invalid", NULL},
};

/*
 * The number of valid commands.
 */
static const int NUM_COMMANDS = 4;

/*
 * The max no. of arguments a command can take.
 *
 * Remember the first arg. of a command is its name.
 */
static const int MAX_ARG_COUNT = 256;
