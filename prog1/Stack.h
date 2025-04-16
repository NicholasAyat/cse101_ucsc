#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "List.h"
//----------------------------------------------------
// Stack.h
// Header file for Stack ADT
// ---------------------------------------------------


typedef struct StackObj{
    NodePtr top; // Points to the top of the stack
    void (*dataPrinter)(void *data); // Function to print the data in stack
    int length; // Keeps track of the stack's length
} StackObj;

typedef struct StackObj* StackPtr;

// Constructors-Destructors --------------------------

/**
 * @brief Creates a new stack
 * 
 * @param dataPrinter Function to use to print out a piece of data in the stack
 * @return StackPtr Pointer to the newly created stack.
 */
StackPtr createStack(void (*dataPrinter)(void *data));

/**
 * @brief Frees the space taken up by the stack
 * 
 * @param pS A pointer to the StackPtr to deallocate.
 *           The pointer should be set to NULL!
 */
void destroyStack(StackPtr *pS);

// Access functions ----------------------------------

/**
 * @brief Gets the length of a stack
 * 
 * @param S The stack for which the length should be returned.
 * @return int The length of the stack, or -1 if error
 */
int lengthStack(StackPtr S);

/**
 * @brief Prints the data in the stack in the format "%s\n" for each entry
 * 
 * @param S The stack to print
 */
void printStack(StackPtr S);

/**
 * @brief Retrieves the data from the top of the stack.
 *
 * @param S The stack to retrieve the data from.
 * @return void* The data that is on top of the stack. Returns NULL if the stack is empty.
 */
void *peekStack(StackPtr S);

// Manipulation functions ----------------------------

/**
 * @brief Pushes an entry onto the stack.
 * 
 * @param S The stack to push onto
 * @param data The data to add as new entry on the stack
 * @return bool True if the push was successful, False otherwise
 */
bool pushStack(StackPtr S, void *data);

/**
 * @brief Pops the entry from the top of the stack and returns the data from that entry.
 * 
 * @param S The stack to pop the entry from.
 * @return void* The data that was stored in the top entry. Returns NULL if the stack is empty.
 */
void *popStack(StackPtr S);

#endif // STACK_H

