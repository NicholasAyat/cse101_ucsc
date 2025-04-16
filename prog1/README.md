# Text Formatter

A command-line text formatting utility that processes input text according to specified alignment and width parameters.

## Features

- Left, right, and center text alignment
- Configurable line width
- Undo functionality for format operations
- Handles multiple spaces, tabs, and punctuation
- Memory-efficient using List and Stack ADTs

## Usage

1. Compile the program:
```bash
make
```

2. Run the program:
```bash
./prog1 < commands.in
```

## Commands

- `open <filename>` - Opens a text file for formatting
- `format <L|R|C> <width>` - Formats text with specified alignment and width
  - L: Left align
  - R: Right align
  - C: Center align
  - width: Maximum characters per line
- `undo` - Removes the most recently created formatted file
- `exit` - Terminates the program

## Example

Input commands (commands.in):
```
open text.in
format L 70
format R 50
format C 45
exit
```

This will:
1. Open text.in
2. Create text.L70 (left-aligned, 70 chars width)
3. Create text.R50 (right-aligned, 50 chars width)
4. Create text.C45 (center-aligned, 45 chars width)
5. Exit the program

## Files

- formatter.c: Main program implementation
- List.h/c: List ADT implementation
- Stack.h/c: Stack ADT implementation
- Makefile: Build configuration
- commands.in: Sample input commands
- text.in: Sample text file 