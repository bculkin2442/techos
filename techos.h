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
struct command parsecom(char *);

/*
 * Command executor.
 *
 * Gathers the CLI args for a command, and then executes it.
 */
int execcom(struct command, char *);
