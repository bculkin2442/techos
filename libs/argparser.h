#ifndef ARGPARSER_HEADER
#define ARGPARSER_HEADER

/* 
 * Library for simple parsing of CLI arguments from user input.
 */

/*
 * The maximum number of CLI arguments parsed from a string.
 */
#define MAX_CLI_ARGS 256

struct cliargs {
	int    argc;
	char **argv;
};

/*
 * Parse a set of CLI args from a given string.
 *
 * The main thing this does that strtok doesn't, is that this properly handles
 * double/single quoted strings as grouping.
 *
 * NOTE 1: The lifetime of the array of arguments is disjoint from the provided
 * string.
 *
 * NOTE 2: The provided string is mutated.
 */
struct cliargs parseargs(char *);
#endif
