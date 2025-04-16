# List and Stack ADT Implementation

This project implements a List ADT and Stack ADT in C, supporting generic data types through void pointers.

## Components

- `List.h/List.c`: Implementation of the List ADT
- `Stack.h/Stack.c`: Implementation of the Stack ADT
- `hwk1.c`: Test program demonstrating both ADTs
- `Makefile`: Compilation instructions

## Features

### List ADT
- Generic data storage using void pointers
- Dynamic node-based implementation
- Operations: create, destroy, append, delete, get, print, length

### Stack ADT
- LIFO (Last-In-First-Out) data structure
- Generic data storage using void pointers
- Operations: push, pop, peek, print, length

## Building and Running

```bash
# Compile the project
make

# Run the program
./hwk1

# Clean compiled files
make clean
```

## Output
The program demonstrates both ADTs by:
1. Testing List operations with integers and strings
2. Testing Stack operations with integers
3. Performing comprehensive tests of the delete operation 