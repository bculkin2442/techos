#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "intern.h"

/* The number of hash buckets we use. */
#define BUCKET_COUNT 10

/* Convert a string to a hashcode. */
unsigned long hashstring(const char *val) {
	/* This is the DJB2K33 algorithm. It generally works quite well. */
	unsigned long hash = 5381;
	int c;

	c    = *val;
	val += 1;

	while(c) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		c    = *val;
		val += 1;
	}

	return hash;
}

unsigned long hashkey(const int val) {
	/* @TODO check if there is a better hash func. to use. */
	return val;
}

struct bucket {
	char      *val;
	internkey  key;

	struct bucket *next;
	struct bucket *prev;
};

struct interntab {
	/* The next intern key to use. */
	internkey nextkey;
	
	/* The table of strings */
	struct bucket strings[BUCKET_COUNT];
	/* The table of keys */
	struct bucket keys[BUCKET_COUNT];
};

/* Allocate/initialize an intern table. */
struct interntab *makeinterntab() {
	struct interntab *tab;
	int i;

	/* Create a new intern table, and set its key value. */
	tab = malloc(sizeof(struct interntab));
	assert(tab != NULL);
	tab->nextkey = 1;

	/* Initialize the tables. */
	for(i = 0; i < BUCKET_COUNT; i++) {
		tab->strings[i].val = NULL;
		tab->strings[i].key = SIINVALID;

		tab->keys[i].val = NULL;
		tab->keys[i].key = SIINVALID;
	}

	return tab;
}

void addbucket(struct bucket *bucket) {
	/* The new bucket. */
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
		/* Chain sbucket. */

		/* The next bucket in the list. */
		struct bucket *nextbucket;
		nextbucket = bucket->next;

		bucket->next     = nbucket;
		nextbucket->prev = nbucket;

		nbucket->next = nextbucket;
		nbucket->prev = bucket;
	}
}

/* Intern a string. */
internkey internstring(struct interntab * table, const char *string) {
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

	shash = hashstring(string) % BUCKET_COUNT;
	khash = hashkey(key)       % BUCKET_COUNT;

	sbucket = &(table->strings[shash]);
	kbucket = &(table->keys[khash]);

	if(sbucket->key != SIINVALID) {
		/* Non-empty bucket. */
		sbucket = sbucket->prev;
		kbucket = kbucket->prev;
	}

	sbucket->val = string;
	sbucket->key = key;
	addbucket(sbucket);

	kbucket->val = string;
	kbucket->key = key;
	addbucket(kbucket);

	return key;
}

internkey lookupstring(struct interntab *table, const char *string) {
	/* The string hash. */
	int shash;
	/* The string bucket. */
	struct bucket *sbucket;
	/* The first key in a bucket. */
	int fkey;

	/* Get the bucket for this string. */
	shash   = hashstring(string) % BUCKET_COUNT;
	sbucket = &(table->strings[shash]);
	fkey    = sbucket->key;

	/* Bail out early if the bucket is empty. */
	if(sbucket->key == SIINVALID) return SIINVALID;

	do {
		/* Check if this is the right bucket. */
		if(strcmp(string, sbucket->val) == 0) return sbucket->key;

		/* Move to next bucket. */
		sbucket = sbucket->next;
		
		/* Bail out if we've looped or hit an empty bucket. */
	} while(sbucket->key != fkey && sbucket->key != SIINVALID);

	/*
	 * We didn't find anything. */
	return SIINVALID;
}

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
	kbucket = &(table->keys[khash]);
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
