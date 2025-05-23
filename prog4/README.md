# Program 4: Graph Traversal Algorithms

This program implements two fundamental graph traversal algorithms: Breadth-First Search (BFS) and Dijkstra's algorithm, applied to a network of cities connected by railways.

## Features

- **BFS Implementation**: Finds shortest path in terms of number of cities traversed (uniform travel time)
- **Dijkstra's Algorithm**: Finds shortest path in terms of actual travel time
- **Bonus Task**: Displays shortest distances from vertex 0 to all vertices and from each vertex to vertex n-1

## Input Format

The program reads input from stdin in the following format:
1. First line: filename containing city connections
2. Subsequent lines: commands (0, 1, 2, or 3)

City connection files (e.g., city1.in) contain:
- First line: number of cities (N)
- Second line: number of connections
- Following lines: connections in format "u,v,w" (from city, to city, travel time)

## Commands

- `0`: Run BFS algorithm
- `1`: Run Dijkstra's algorithm
- `2`: Run bonus task
- `3`: Exit program

## Compilation

```bash
make clean
make
```

## Usage

```bash
./prog4 < test1.in
```

## Test Files

- `test1.in`, `test2.in`, `test3.in`: Test input files
- `city1.in`, `city2.in`, `city3.in`: City connection files
- `test1.out`, `test2.out`, `test3.out`: Expected output files

## Implementation Details

- Uses provided queue implementation for BFS
- Implements Dijkstra's algorithm using array-based approach
- Handles memory management properly
- Includes error handling for file operations and memory allocation 