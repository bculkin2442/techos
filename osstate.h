#ifndef TECHOS_OSSTATEH
#define TECHOS_OSSTATEH

/* Default date formats. */
extern char *defin_datefmt;
extern char *deftime_datefmt;
extern char *defout_datefmt;

/* General structure for OS-level state. */
struct osstate {
	/* Input/output/time date formats. */
	char *in_datefmt;
	char *out_datefmt;
	char *time_datefmt;

	/* Current time/date. */
	struct tm *datetime;
	
	/* The stream we read input from. */
	FILE *strem;
};

/* Create/delete an osstate. */
struct osstate *makeosstate();
void             killosstate(struct osstate *);

#endif
