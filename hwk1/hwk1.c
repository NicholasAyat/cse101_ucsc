#include "Stack.h"
#include "List.h"
#include <stdlib.h>
#include <stdio.h>

void printNumber( void *num )
{
	int *i = num;		// "casting" void * to int *
	printf( "%d    ", *i );	// print what i is pointing to
}

void printString( void *str )
{
	char *s = str;		// "casting" void * to char *
	printf( "%s    ", s );	// print string starting from s
}

int main(int argc, char **argv){


	void *data;
	int  value;
	int  num1 = 123;
	int  num2 = 321;
	char st1[100] = "hello";
	char st2[100] = "good bye";

	// Declare a new num list

	ListPtr L = createList( printNumber );
	 
	// Perform desired operation on list
	
	appendList( L, &num2 );
	appendList( L, &num1 );

	int len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	destroyList( &L );

	printf( "num list after destroying it is: " );
	printList( L );
	printf( "-- \n" );

	L = createList( printNumber );	// why is this line needed?

	appendList( L, &num1 );
	appendList( L, &num2 );
	appendList( L, &num1 );

	len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	for( int i=0; i<len; i++ ) {
	    data = getList( L, i );
	    if (data != NULL) {
		value = *(int *)data;	// contents of data after casting to int *
		printf( "%dth element of list is %d\n", i, value );
	    }
	}

	deleteList( L, 1 );
	len = lengthList( L );
	printf( "num list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	// Declare a new list

	printf( "\nReuse L for strings\n" );

	L = createList( printString );	// don't i need to call destroyList(L) 1st?

	appendList( L, st1 );
	appendList( L, st2 );

	len = lengthList( L );
	printf( "string list has %d elements\n", len );
	printList( L );
	printf( "\n" );

	// Additional tests for deleteList()
	printf("\nTesting deleteList function:\n");
	
	// Test deleting from empty list
	ListPtr emptyList = createList(printNumber);
	printf("Trying to delete from empty list: %s\n", 
		   deleteList(emptyList, 0) == NULL ? "NULL (correct)" : "Not NULL (incorrect)");
	destroyList(&emptyList);

	// Test deleting with various indices
	ListPtr testList = createList(printNumber);
	int test1 = 10, test2 = 20, test3 = 30, test4 = 40;
	
	appendList(testList, &test1);
	appendList(testList, &test2);
	appendList(testList, &test3);
	appendList(testList, &test4);
	
	printf("\nOriginal test list: ");
	printList(testList);
	printf("\n");
	
	// Test deleting first element
	printf("Deleting first element (index 0): ");
	void *deleted = deleteList(testList, 0);
	printf("Deleted value: %d\n", *(int*)deleted);
	printf("List after deleting first: ");
	printList(testList);
	printf("\n");
	
	// Test deleting last element
	printf("Deleting last element (index %d): ", lengthList(testList) - 1);
	deleted = deleteList(testList, lengthList(testList) - 1);
	printf("Deleted value: %d\n", *(int*)deleted);
	printf("List after deleting last: ");
	printList(testList);
	printf("\n");
	
	// Test deleting middle element
	printf("Deleting middle element (index 0): ");
	deleted = deleteList(testList, 0);
	printf("Deleted value: %d\n", *(int*)deleted);
	printf("List after deleting middle: ");
	printList(testList);
	printf("\n");
	
	// Test invalid index
	printf("Trying to delete invalid index (-1): %s\n", 
		   deleteList(testList, -1) == NULL ? "NULL (correct)" : "Not NULL (incorrect)");
	printf("Trying to delete invalid index (too large): %s\n", 
		   deleteList(testList, 100) == NULL ? "NULL (correct)" : "Not NULL (incorrect)");
	
	destroyList(&testList);

	// Test Stack ADT
	printf("Stack operations:\n");
	StackPtr myStack = createStack(printNumber);
	
	int data4 = 100, data5 = 200, data6 = 300;
	
	printf("Pushing 100, 200, 300 to the stack...\n");
	pushStack(myStack, &data4);
	pushStack(myStack, &data5);
	pushStack(myStack, &data6);
	
	printf("Stack after pushing: ");
	printStack(myStack);
	printf("\n");

	printf("Length of the stack: %d\n", lengthStack(myStack));

	printf("Peeking the top of the stack: %d\n", *(int *)peekStack(myStack));

	printf("Popping the top of the stack...\n");
	popStack(myStack);
	printStack(myStack);
	printf("\n");
	
	destroyStack(&myStack);
	
	return 0;
}
