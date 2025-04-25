# hwk2 - Priority Queue Implementation

A dynamic, array-based priority queue implementation using a binary max-heap data structure.

## Files
- `priority_queue.h`: Header file containing type definitions and function declarations
- `priority_queue.c`: Implementation of the priority queue operations
- `test.c`: Test cases demonstrating functionality
- `makefile`: Compilation instructions
- `test.out`: Expected output format

## Functions
### Main API Functions:
- `pq_init()`: Initialize priority queue
- `pq_insert()`: Insert element
- `pq_delete()`: Remove highest priority element
- `pq_is_empty()`: Check if queue is empty
- `pq_destroy()`: Free queue memory
- `print_queue()`: Display queue contents

### Helper Functions:
- `parent()`: Get parent index
- `left()`: Get left child index
- `right()`: Get right child index
- `swap()`: Swap two elements
- `heapify_up()`: Maintain heap property upward
- `heapify_down()`: Maintain heap property downward

## Build & Run
```bash
make        # Compile the program
./hwk2      # Run the tests
make clean  # Clean build files
```

## Implementation Details
- Uses binary max-heap structure
- Dynamic array-based storage
- Generic data storage using void pointers
- Custom comparator function support
- O(log n) insert and delete operations 