# Program 4: Vocabulary BST/AVL

This program implements a vocabulary management system using a Binary Search Tree (BST). It supports inserting, deleting, finding, and traversing tokens. The project also includes a bonus implementation that uses a self-balancing AVL tree for the same operations.

## Basic Implementation (BST)

### Compilation
To compile the basic version, navigate to the `prog4` directory and use the provided Makefile:
```bash
make
```
This will generate an executable named `prog4`.

### Usage
Run the program with an input file and redirect the output for verification:
```bash
./prog4 < test1.in > my_test1.out
```
You can then compare your output with the provided `test1.out.basic` file:
```bash
diff my_test1.out test1.out.basic
```

## Bonus Implementation (AVL)

The bonus implementation uses an AVL tree to maintain a balanced vocabulary tree, resulting in faster search operations.

### Compilation
To compile the bonus version, navigate to the `prog4/bonus` directory and use its Makefile:
```bash
cd bonus
make
```
This will generate an executable named `bonus`.

### Usage
From the `prog4/bonus` directory, run the program with the test inputs located in the parent directory:
```bash
./bonus < ../test1.in > my_test1.out.bonus
```
Compare your output with the provided bonus output file:
```bash
diff my_test1.out.bonus ../test1.out.bonus
```

## Cleaning Up
To remove all compiled object files and executables from the `prog4` directory:
```bash
make clean
```
To clean the `bonus` directory, navigate to it and run:
```bash
cd bonus
make clean
``` 