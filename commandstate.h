#ifndef TECHOS_COMMANDSTATEH
#define TECHOS_COMMANDSTATEH

struct commandstate {
	struct comlist *plCommands;
};

/* Allocate/initialize command state. */
struct commandstate *makecommandstate();
/* Deinitialize/deallocate command state. */
void                killcommandstate(struct commandstate *);
#endif
