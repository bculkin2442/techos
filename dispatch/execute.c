#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
	FILE *inp;
	int c;
	int offset = atoi(argv[2]);
	int counter = offset;
	char ch = '!';

	inp = fopen(argv[1], "r");
	assert(inp != NULL);

	fseek(inp, offset, SEEK_SET);
	c = fgetc(inp);
	while (c != EOF && c - 5 != (int)ch)
	{
		//printf("Read %c\n", c - 5);
		counter++;
		c = fgetc(inp);
	}
	if (c == EOF)
		return 0;
	else
		return counter;
}