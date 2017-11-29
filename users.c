#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libs/intern.h"

#include "users.h"

static void killuser(void *pvUser) {
	struct user *puUser = (struct user *)pvUser;

	free(puUser->pszName);
	free(puUser->pszPass);

	free(puUser);
}

struct userdb {
	struct internlist *plUsers;
};

struct userdb *makeudb() {
	/* Allocate memory for the DB. */
	struct userdb  *pdDB = malloc(sizeof(struct userdb));
	/* Fail if allocation did. */
	assert(pdDB != NULL);

	/* Allocate memory for the user list. */
	pdDB->plUsers = makeinternlist(5, &killuser);
	/* Fail if allocation did. */
	assert(pdDB->plUsers != NULL);

	return pdDB;
}

void killudb(struct userdb *pdDB) {
	int i;

	killinternlist(pdDB->plUsers);

	free(pdDB);
}

struct user *udblookup(struct userdb *pdDB, char *pszUsername) {
	return (struct user *)getinternlist(pdDB->plUsers, pszUsername);
}

void udbinsert(struct userdb *pdDB, enum utype type, char *pszUsername, char *pszPassword) {
	struct user *puUser = malloc(sizeof(struct user));
	assert(puUser != NULL);

	puUser->type = type;

	puUser->pszName = (char *)strdup(pszUsername);
	puUser->pszPass = (char *)strdup(pszPassword);

	putinternlist(pdDB->plUsers, pszUsername, puUser);
}

void udbremove(struct userdb *pdDB, char *pszUsername) {
	deleteinternlist(pdDB->plUsers, pszUsername);
}
