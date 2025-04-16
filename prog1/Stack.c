#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Stack.h"
#include "List.h"
#include <assert.h>

// Constructor for the Stack ADT
StackPtr createStack(void (*dataPrinter)(void *data)) {
    StackPtr S = (StackPtr)malloc(sizeof(StackObj));
    if (S) {
        S->top = NULL;
        S->dataPrinter = dataPrinter;
        S->length = 0;
    }
    return S;
}

// Destructor for the Stack ADT
void destroyStack(StackPtr *pS) {
    if (pS && *pS) {
        NodePtr current = (*pS)->top;
        while (current != NULL) {
            NodePtr next = current->next;
            free(current);
            current = next;
        }
        free(*pS);
        *pS = NULL;
    }
}

// Returns the length of the stack
int lengthStack(StackPtr S) {
    if (S == NULL) return -1;
    return S->length;
}

// Prints the data in the stack
void printStack(StackPtr S) {
    if (S == NULL || S->dataPrinter == NULL) return;
    
    NodePtr current = S->top;
    while (current != NULL) {
        S->dataPrinter(current->data);
        current = current->next;
    }
}

// Retrieves the data at the top of the stack without removing it
void *peekStack(StackPtr S) {
    if (S == NULL || S->top == NULL) return NULL;
    return S->top->data;
}

// Pushes an entry onto the stack.
bool pushStack(StackPtr S, void *data) {
    if (S == NULL) return false;
    
    NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
    if (newNode == NULL) return false;
    
    newNode->data = data;
    newNode->next = S->top;
    S->top = newNode;
    S->length++;
    
    return true;
}

// Pops the entry from the top of the stack and returns the data from that entry.
void *popStack(StackPtr S) {
    if (S == NULL || S->top == NULL) return NULL;
    
    NodePtr temp = S->top;
    void *data = temp->data;
    S->top = temp->next;
    free(temp);
    S->length--;
    
    return data;
}
