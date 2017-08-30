enum commands {
	COM_UNKNOWN,
	COM_EXIT   ,
};

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
int handle_exit();
