# Recursive Graphics Pattern Generator

This project contains two C programs that generate recursive graphics patterns using the Cairo Graphics Library.

## Programs

### prog0.c
Generates recursive squares divided into quadrants with randomly rotated squares in each quadrant. The program divides the initial square into 4 quadrants at each recursion level, with each quadrant containing a randomly rotated square.

### bonus.c
An extension of prog0.c that recursively draws squares with alternating colors (red and green). Instead of recursing on quadrants, it recurses on each square, creating an interesting fractal pattern. Each child square is centered within its parent's quadrant and follows the parent's orientation.

## Prerequisites
- Cairo Graphics Library
- C compiler (gcc)

## Compilation
Use the provided Makefile to compile both programs:

```
make
```

## Running the Programs

### prog0
Takes two arguments: the length of a side of the square and the recursion depth.

```
./prog0
```

Then input two numbers: size and depth (e.g., "512 2")

### bonus
Takes six arguments: coordinates of the first edge (x1, y1, x2, y2), size of the drawing area, and recursion depth.

```
./bonus x1 y1 x2 y2 size depth
```

Example:
```
./bonus 50 460 460 360 512 2
```

This creates a 512x512 drawing area with the initial square defined by the edge from (50,460) to (460,360), and recursion depth of 2.

## Output
Both programs generate PNG image files:
- prog0: square.png
- bonus: bonus.png 