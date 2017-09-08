/*
 * Forward-declare OS state.
 */
struct osstate;

/*
 * Represents a command.
 */
struct command {
	/*
	 * The name of the command.
	 */
	const char *name;

	/*
	 * A brief description of the command.
	 */
	const char *brief;

	/*
	 * The function that implements the command.
	 */
	int (*comfun)(int, char **, char *, struct osstate *);
};

/*
 * Initialize data for commands.
 */
void initcoms();

/*
 * Cleanup after commands.
 */
void disposecoms();

/*
 * Declare a command.
 */
#define DECLCOM(name) int handle_##name(int, char **, char *, struct osstate *)

/*
 * Handle an attempt to exit.
 *
 * Prompts to make sure the user wants to exit.
 */
DECLCOM(exit);
DECLCOM(version);
DECLCOM(date);
DECLCOM(datefmt);
DECLCOM(setdate);
DECLCOM(help);
DECLCOM(time);

/*
 * All the commands in the operating system.
 */
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
 * The number of valid commands. Make sure to update this whenever a new command
 * is added.
 */
static const int NUM_COMMANDS = 7;

/*
 * An invalid command.
 */
static struct command INVALID_COMMAND = {"invalid", "", NULL};

/*
 * The max no. of arguments a command can take.
 *
 * Remember the first arg. of a command is its name.
 */
static const int MAX_ARG_COUNT = 256;
