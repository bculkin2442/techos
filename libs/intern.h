#ifndef INTERN_HEADER
#define INTERN_HEADER

/*
 * Handle interning of string constants for quick equality checking.
 */

/* An intern key is a reference to a string in an intern table. */
typedef int internkey;

/* The invalid intern key. */
static internkey SIINVALID = -1;
/* The code that indicates the intern table is full. */
static internkey SITABFULL = -2;

/*
 * A interntab is a hash table that contains interned strings.
 *
 * Used to be called boss, because someone has to manage a bunch of interns.
 * However, this is a serious program, and we should have meaningful, not funny
 * names.
 */
struct interntab;

/* Create/destroy a intern table. */
struct interntab *makeinterntab();
void              killinterntab(struct interntab *);

/*
 * Intern a string into a intern table.
 *
 * Returns the 'intern key' for the string, or a negative error code if
 * something failed.
 *
 * Anything 0 or less is never a valid intern key.
 */
internkey internstring(struct interntab *, const char *);

/*
 * Lookup the intern key for a given string.
 *
 * Returns the key if the string was found, or SIINVALID if the given string isn't
 * interned.
 */
internkey lookupstring(struct interntab *, const char *);

/*
 * Lookup the string for a given intern key.
 *
 * Returns the string if the key was found, or NULL if the given key is invalid.
 */
const char *lookupkey(struct interntab *, internkey);
#endif
