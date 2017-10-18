#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main() {
	char *pszLine;
	size_t lLen, lRead;

	pszLine = NULL;
	lLen    = 0;


	do {
		printf("Enter one character at a time (x to exit): ");
		lRead = getline(&pszLine, &lLen, stdin);

		if(lRead < 1) {
			printf("Must enter at least one character.");
		} else {
			char n;

			n = pszLine[0];

			if(n == 'x') {
				break;
			} else if(n >= '0' && n <= '9') {
				push(n - 30);
			} else {
				int lhs, rhs;

				lhs = pop();
				rhs = pop();

				switch(pszLine[0]) {
				case '+':
					push(lhs + rhs);
					break;
				case '-':
					push(lhs - rhs);
					break;
				case '*':
					push(lhs * rhs);
					break;
				case '/':
					if(rhs == 0) {
						printf("\tdiv by zero\n");
						rhs = 1;
					}
					push(lhs / rhs);
					break;
				default:
					printf("\tUnknown character '%c'\n", pszLine[0]);
					break;
				}
			}
		}
	} while(lRead > 0);

	free(line);

	return 0;
}
