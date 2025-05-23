# Binary Search Tree Implementation

A C program that implements a Binary Search Tree (BST) with support for various operations including insertion, deletion, search, and tree traversals.

## Features

- Build BST from initial array of elements
- Insert new nodes (duplicates are ignored)
- Delete nodes (using inorder successor for nodes with two children)
- Search for values with depth reporting
- Tree traversals (inorder, preorder, postorder)

## Usage

1. Compile the program:
```bash
make
```

2. Run the program:
```bash
./hwk4 < input_file
```

## Input Format

- First line: Number of initial elements (n)
- Second line: n space-separated integers to build initial BST
- Subsequent lines: Commands (one per line)
  - `insert x` - Insert value x
  - `delete x` - Delete value x
  - `find x` - Search for value x
  - `inorder` - Print inorder traversal
  - `preorder` - Print preorder traversal
  - `postorder` - Print postorder traversal
  - `exit` - Terminate program

## Example

Input (test1.in):
```
5
10 5 15 3 8
find 8
find 20
inorder
preorder
postorder
delete 5
inorder
exit
```

Output:
```
Found (Depth: 2)
Not found
3 5 8 10 15 
10 5 3 8 15 
3 8 5 15 10 
3 8 10 15 
``` 