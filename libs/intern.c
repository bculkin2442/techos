#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Debugging. */
#include <stdio.h>

#include "intern.h"

/* 
 * The number of hash buckets we use.
 *
 * This should be a prime number.
 */
#define BUCKET_COUNT 13

/* Convert a string to a hashcode. */
static unsigned long hashstring(const char *val) {
	/* This is the DJB2K33 algorithm. It generally works quite well. */
	unsigned long hash = 5381;
	int c;

	/* Get the current string character. */
	c    = *val;
	val += 1;

	/* While we're not at the end of the string. */
	while(c) {
		/* hash * 33 + c */
		hash = ((hash << 5) + hash) + c; 

		/* Advance to the next character. */
		c    = *val;
		val += 1;
	}

	return hash;
}

/* Convert an integer to a hashcode. */
static unsigned long hashkey(const int val) {
	/* @TODO check if there is a better hash func. to use. */
	return val;
}

/* A bucket in the intern table */
struct bucket {
	/* The string in this bucket. */
	char      *val;
	/* The intern key for this string. */
	internkey  key;

	/*
	 * Next/prev form a circular list of buckets.
	 */

	/* The next bucket in the chain. */
	struct bucket *next;
	/* The previous bucket in the chain. */
	struct bucket *prev;
};

/* Definition of an intern table. */
struct interntab {
	/* The next intern key to use. */
	internkey nextkey;
	
	/* The table of strings */
	struct bucket *strings[BUCKET_COUNT];
	/* The table of keys */
	struct bucket *keys[BUCKET_COUNT];
};

/* Allocate/initialize an intern table. */
struct interntab *makeinterntab() {
	/* The table. */
	struct interntab *tab;
	/* The counter for entry initialization */
	int i;

	/* Create a new intern table, and set its key value. */
	tab = malloc(sizeof(struct interntab));
	assert(tab != NULL);
	tab->nextkey = 1;

	/* Initialize the tables. */
	for(i = 0; i < BUCKET_COUNT; i++) {
		/* Initialize string table. */
		tab->strings[i]       = malloc(sizeof(struct bucket));
		tab->strings[i]->val  = NULL;
		tab->strings[i]->key  = SIINVALID;
		tab->strings[i]->next = NULL;
		tab->strings[i]->prev = NULL;

		/* Initialize key table. */
		tab->keys[i]       = malloc(sizeof(struct bucket));
		tab->keys[i]->val  = NULL;
		tab->keys[i]->key  = SIINVALID;
		tab->keys[i]->next = NULL;
		tab->keys[i]->prev = NULL;
	}

	return tab;
}

void killinterntab(struct interntab *table) {
	int i;
	/* Free strings table. */
	for(i = 0; i < BUCKET_COUNT; i++) {
		/* Get the initial bucket. */
		struct bucket *buck;
		buck = table->strings[i];

		/* Terminate the chain. */
		buck->prev->next = NULL;

		while(buck != NULL && buck->next != NULL) {
			/* Free each bucket in the chain. */
			struct bucket *tmp;

			tmp  = buck;
			buck = buck->next;

			/* Free the string we own. */
			if(tmp->val != NULL) free(tmp->val);
			free(tmp);
		}
	}

	/* Free keys table. */
	for(i = 0; i < BUCKET_COUNT; i++) {
		/* Get the initial bucket. */
		struct bucket *buck;
		buck = table->strings[i];

		/* Terminate the chain. */
		buck->prev->next = NULL;

		while(buck != NULL && buck->next != NULL) {
			/* Free each bucket in the chain. */
			struct bucket *tmp;

			tmp  = buck;
			buck = buck->next;

			/* Free the bucket. */
			free(tmp);
		}
	}

	free(table);
}

/* Add a bucket to a bucket chain. */
static void addbucket(struct bucket *bucket) {
	/* Allocate a new bucket. */
	struct bucket *nbucket;
	nbucket = malloc(sizeof(struct bucket));
	assert(nbucket != NULL);

	if(bucket->next == NULL) {
		/* Solo bucket. */
		bucket->next  = nbucket;
		bucket->prev  = nbucket;

		nbucket->next = bucket;
		nbucket->prev = bucket;
	} else {
		/* Chain bucket. */

		/* The next bucket in the list. */
		struct bucket *nextbucket;
		nextbucket = bucket->next;

		/* Insert our bucket into the chain. */
		bucket->next     = nbucket;
		nextbucket->prev = nbucket;
		nbucket->next = nextbucket;
		nbucket->prev = bucket;
	}
}

/* Intern a string. */
internkey internstring(struct interntab *table, const char *string) {
	/* The intern key we use. */
	internkey key;
	/* The key/string hashes. */
	int khash, shash;
	/* The string/key buckets. */
	struct bucket *sbucket, *kbucket;

	/* Don't duplicate an already interned string. */
	key = lookupstring(table, string);
	if(key != SIINVALID) return key;

	/* Get the next valid intern key. */
	key = table->nextkey;
	table->nextkey += 1;
	/* Fail if we ran out of space for strings. */
	if(key < 0) return SITABFULL;
	/* Make sure we never make the invalid key valid. */
	assert(key != SIINVALID);

	/* Create a copy of the string we own. */
	char *strang = (char *)strdup(string);

	/* Get both hashes. */
	shash = hashstring(strang) % BUCKET_COUNT;
	khash = hashkey(key)       % BUCKET_COUNT;

	/* Get the buckets for each hash. */
	sbucket = table->strings[shash];
	kbucket = table->keys[khash];

	if(sbucket->key != SIINVALID) {
		/*
		 * Non-empty bucket. 
		 *
		 * Insert into a new one.
		 */
		sbucket = sbucket->prev;
	}
	if(kbucket->key != SIINVALID) {
		/*
		 * Non-empty bucket. 
		 *
		 * Insert into a new one.
		 */
		kbucket = kbucket->prev;
	}

	/* Insert into the right bucket in both tables. */
	sbucket->val = strang;
	kbucket->val = strang;

	sbucket->key = key;
	kbucket->key = key;

	/* Add a new empty bucket to both tables. */
	addbucket(sbucket);
	addbucket(kbucket);

	return key;
}

/* Lookup a interned key for a string. */
internkey lookupstring(struct interntab *table, const char *string) {
	/* The string hash. */
	int shash;
	/* The string bucket. */
	struct bucket *sbucket;
	/* The first key in a bucket. */
	int fkey;

	/* Get the bucket for this string. */
	shash   = hashstring(string) % BUCKET_COUNT;
	sbucket = table->strings[shash];
	fkey    = sbucket->key;

	/* Bail out early if the bucket is empty. */
	if(sbucket->key == SIINVALID) return SIINVALID;

	do {
		/* Only check buckets with valid values. */
		if(sbucket->val != NULL) {
			/* Check if this is the right bucket. */
			if(strcmp(string, sbucket->val) == 0) return sbucket->key;
		}

		/* Move to next bucket. */
		sbucket = sbucket->next;
		
		/* Bail out if we've looped or hit an empty bucket. */
	} while(sbucket->key != fkey && sbucket->key != SIINVALID);

	/* We didn't find anything. */
	return SIINVALID;
}

/* Lookup the string for an interned key. */
const char *lookupkey(struct interntab *table, const internkey key) {
	/* The key hash. */
	int khash;
	/* The key bucket. */
	struct bucket *kbucket;
	/* The first key in a bucket. */
	int fkey;

	/* This key can't possibly be in the table. */
	if(table->nextkey < key) return NULL;

	/* Get the bucket for this key. */
	khash   = hashkey(key) % BUCKET_COUNT;
	kbucket = table->keys[khash];
	fkey    = kbucket->key;

	/* Bail out early if the bucket is empty. */
	if(kbucket->key == SIINVALID) return NULL;

	do {
		/* Check if this is the right bucket. */
		if(key == kbucket->key) return kbucket->val;

		/* Move to next bucket. */
		kbucket = kbucket->next;
		
		/* Bail out if we've looped or hit an empty bucket. */
	} while(kbucket->key != fkey && kbucket->key != SIINVALID);

	/*
	 * We didn't find anything. */
	return NULL;
}
