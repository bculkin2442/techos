#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libs/intern.h"

#include "users.h"

struct userdb {
	/* Contains the username to userid mapping. */
	struct interntab *ptUsers;

	/* Contains the dynamic array of users. */
	struct user **paUsers;

	/* Available/used space in user array. */
	int userspace;
	int usercount;
};

struct userdb *makeudb() {
	/* Allocate memory for the DB. */
	struct userdb  *pdDB = malloc(sizeof(struct userdb));
	/* Fail if allocation did. */
	assert(pdDB != NULL);

	/* Allocate memory for the user list. */
	pdDB->paUsers = calloc(5, sizeof(struct user));
	/* Fail if allocation did. */
	assert(pdDB->paUsers != NULL);

	/* Set up array tracking. */
	pdDB->userspace = 5;
	pdDB->usercount = 0;

	/* Allocate intern table. */
	pdDB->ptUsers = makeinterntab();

	return pdDB;
}

void killudb(struct userdb *pdDB) {
	int i;

	killinterntab(pdDB->ptUsers);

	for(i = 0; i < pdDB->usercount; i++) {
		struct user *pUser = pdDB->paUsers[i];

		free(pUser->pszName);
		free(pUser->pszPass);
	}

	free(pdDB->paUsers);

	free(pdDB);
}

struct user *udblookup(struct userdb *pdDB, char *pszUsername) {
	
}
