#include <stdio.h>

#include "procexecute.h"

/* Execute an image. */
int executeimage(FILE *fImage, int offset) {
	/* The current character. */
	int c;
	/* The counter for positions. */
	int counter = offset;

	/* The interrupt character. */
	char ch = '!';

	/* Go to the right offset. */
	fseek(fImage, offset, SEEK_SET);
	/* Get the character. */
	c = fgetc(fImage);
	/* Execute until an interrupt happens. */
	while (c != EOF && c - 5 != (int)ch) {
		/* Increase position. */
		counter++;

		/* Get a new character. */
		c = fgetc(fImage);
	}

	if (c == EOF) {
		return 0;
	} else {
		/* Give back the offset so that we can continue it later. */
		return counter;
	}
}
