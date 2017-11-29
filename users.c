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

/* Load user database from a file. */
void ramfileintodb(struct userdb *pdDB, char *pszFilename) {
	FILE *pfFile;

	char *pszLine;
	size_t  llen;
	size_t lread;

	llen = 0;

	pfFile = fopen(pszFilename, "r");
	if(pfFile == NULL) {
		return;
	}

	while((lread = getline(&pszLine, &llen, pfFile)) != -1) {
		if(feof(pfFile)) break;
		
		char *pszName, *pszPass, *pszType, *pSave;
		enum utype type;

		/* Read username. */
		pszName = (char *)strtok_r(pszName, ":", &pSave);
		/* Skip improperly formatted records. */
		if(pszName == NULL) continue;

		/* Read password. */
		pszPass = (char *)strtok_r(NULL, ":", &pSave);
		/* Skip improperly formatted records. */
		if(pszPass == NULL) continue;

		/* Read user type. */
		pszType = (char *)strtok_r(NULL, ":", &pSave);
		/* Skip improperly formatted records. */
		if(pszType == NULL) continue;

		/* Parse user type. */
		if(strncmp("rim", pszType, 3) == 0) {
			type = UTY_BASIC;
		} else if(strncmp("spoke", pszType, 5) == 0) {
			type = UTY_ADMIN;
		} else if(strncmp("hub", pszType, 3) == 0) {
			type = UTY_ROOT;
		} else {
			/* Invalid user type. */
			continue;
		}

		udbinsert(pdDB, type, pszName, pszPass);
	}

	fclose(pfFile);
}

void offloaduser(char *pszUsername, void *pvUser, void *pvData) {
	struct user *puUser;

	FILE *pfOutput;

	char *pszKind;

	puUser   = (struct user *)pvUser;
	pfOutput = (FILE *)pvData;

	switch(puUser->type) {
	case UTY_BASIC:
		pszKind = "rim";
		break;
	case UTY_ADMIN:
		pszKind = "spoke";
		break;
	case UTY_ROOT:
		pszKind = "hub";
		break;
	default:
		/* Shouldn't happen. */
		assert(0);
	}

	fprintf(pfOutput, "%s:%s:%s\n", puUser->pszName, puUser->pszPass, pszKind);
}

/* Save user database to a file. */
void ramdbintofile(struct userdb *pdDB, char *pszFilename) {
	FILE *pfFile = fopen(pszFilename, "w");
	if(pfFile == NULL) return;

	foreachinternlist(pdDB->plUsers, &offloaduser, pfFile);

	fclose(pfFile);
}
