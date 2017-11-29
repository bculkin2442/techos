#ifndef TECHOS_USERS_H
#define TECHOS_USERS_H

/* The types of users available. */
enum utype {
	/* Basic User
	 *
	 * Have access to all normal commands.
	 */
	UTY_BASIC,
	/*
	 * Administrator
	 *
	 * All the permissions of basic users, plus the ability to
	 * create/delete/modify basic users.
	 */
	UTY_ADMIN,
	/*
	 * Root Administrator
	 *
	 * All the permissions of administrators, plus the ability to
	 * create/delete/modify administrators.
	 */
	UTY_ROOT,
};

/* Represents a TechOS user. */
struct user {
	/* The type of the user. */
	enum utype type;

	/* The name of the user. */
	char *pszName;
	/* The password of the user. */
	char *pszPass;
};

/* Represents all of the available users. */
struct userdb;

/* Create a user DB. */
struct userdb *makeudb();
/* Destroy a user DB. */
void           killudb(struct userdb *);
/* Insert a user into the database. */
/* 
 * Lookup a user in the user database.
 *
 * Returns NULL if there  is no user by that name.
 */
struct user *udblookup(struct userdb *, char *);
#endif
