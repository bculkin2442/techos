#define MAX_STACK_SIZE 100

struct stack {
	int s[MAX_STACK_SIZE];

	int top;
} mst;

int stfull() {
	if (mst.top >= MAX_STACK_SIZE - 1) return 1;
	else                     return 0;
}

void push(int item) {
	mst.top += 1;

	mst.s[mst.top] = item;
}

int stempty() {
	if (mst.top == -1) return 1;
	else               return 0;
}

int pop() {
	int item;

	item = mst.s[mst.top];

	mst.top -= 1;

	return item;
}
