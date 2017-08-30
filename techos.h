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
 * Determines which command should be executed.
 */
int parsecom(char *);
