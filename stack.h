#include<stdio.h>
#include<stdlib.h>

#define size 100
struct stack {
int s[size];
int top;
} mst;

int stfull() 
{
   if (mst.top >= size - 1)
      return 1;
   else
      return 0;
}

void push(int item) 
{
   mst.top++;
   mst.s[mst.top] = item;
}

int stempty() 
{
   if (mst.top == -1)
      return 1;
   else
      return 0;
}

int pop() 
{
   int item;
   item = mst.s[mst.top];
   mst.top--;
   return (item);
}

