/*
 * The major/minor version of TechOS.
 */
static const int major_ver = 1;
static const int minor_ver = 1;

/*
 * The stream we read input from.
 */
FILE *strem;

/*
 * General structure for OS-level state.
 */
struct osstate {
	/*
	 * Input/output/time date formats.
	 */
	char *in_datefmt;
	char *out_datefmt;
	char *time_datefmt;

	/*
	 * Current time/date.
	 */
	struct tm *datetime;
};

/*
 * Create/delete an osstate.
 */
struct osstate *makeosstate();
void             killosstate(struct osstate *);

/*
 * Main command handler.
 *
 * Loops reading commands and handling them until EOF or an exit command is
 * handled.
 */
void comhan();

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
int execcom(struct command, char *, char *, struct osstate *);
