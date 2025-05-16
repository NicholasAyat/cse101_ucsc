# Dictionary and Hash Table Implementation

This assignment implements a Dictionary ADT using a hash table with separate chaining for collision resolution. The implementation builds a vocabulary from a text corpus and assigns unique IDs to words.

## Components

- `Dictionary.c/h`: Dictionary ADT implementation using hash table
- `HashTable.c/h`: Hash function implementation
- `List.c/h`: List ADT for collision resolution
- `hwk3.c`: Main program that builds vocabulary and processes input

## Features

- Dictionary operations: insert, delete, find
- Hash table with separate chaining
- Memory management with proper cleanup
- Input/output matching specified format

## How to Run

1. Compile the program:
```bash
make clean
make
```

2. Run with test input:
```bash
./hwk3 corpus.txt < test.in
```

## Expected Output

The program will:
1. Display vocabulary with word IDs
2. Process input numbers and show corresponding words
3. Test dictionary operations (insert/delete)
4. Show final dictionary state

Output format matches test.out exactly. 