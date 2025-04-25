// priority_queue.h
/*
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef int (*Comparator)(void *, void *); //Comparator is a type that refers to a function which takes two void* arguments and returns an int.

typedef struct {
    void **data; //pointer pointing to array of data pointers, each data pointer points to the acutall prioirty data sent from test.c 
    int capacity; //capcity of prioirty queue.
    int size; //current size of priority queue.
    Comparator cmp; //pointer to comparator function in test.c.
} PriorityQueue;

void pq_init(PriorityQueue *pq, Comparator cmp);
void pq_push(PriorityQueue *pq, void *item);
void *pq_pop(PriorityQueue *pq);
void print_queue(PriorityQueue *pq);
int pq_is_empty(PriorityQueue *pq);
void pq_destroy(PriorityQueue *pq);
#endif*/


// priority_queue.h
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef int (*Comparator)(void *, void *); // Comparator is a type that refers to a function which takes two void* arguments and returns an int.

/**
 * @brief Structure representing a priority queue.
 * 
 * The priority queue stores data items with priorities and maintains order based on the comparator function.
 */
typedef struct {
    void **data;     // Pointer to an array of data pointers, each pointing to the actual priority data
    int capacity;    // Maximum capacity of the priority queue
    int size;        // Current number of elements in the priority queue
    Comparator cmp;  // Comparator function to determine item priority
} PriorityQueue;

// Constructors-Destructors --------------------------

/**
 * @brief Initializes a priority queue with a given comparator function.
 * 
 * @param pq Pointer to the priority queue to initialize
 * @param cmp Comparator function used to determine priority between elements
 */
void pq_init(PriorityQueue *pq, Comparator cmp);

/**
 * @brief Destroys the priority queue and frees allocated memory.
 * 
 * @param pq Pointer to the priority queue to destroy
 */
void pq_destroy(PriorityQueue *pq);

// Manipulation functions ----------------------------

/**
 * @brief Inserts an item into the priority queue while maintaining the priority order.
 * 
 * @param pq Pointer to the priority queue
 * @param item Pointer to the data to be inserted
 */
void pq_insert(PriorityQueue *pq, void *item);

/**
 * @brief Removes and returns the highest-priority item from the priority queue.
 * 
 * @param pq Pointer to the priority queue
 * @return void* Pointer to the data of the highest-priority item, or NULL if the queue is empty
 */
void *pq_delete(PriorityQueue *pq);

// Access functions ----------------------------------

/**
 * @brief Checks whether the priority queue is empty.
 * 
 * @param pq Pointer to the priority queue
 * @return int 1 if the queue is empty, 0 otherwise
 */
int pq_is_empty(PriorityQueue *pq);

/**
 * @brief Prints the contents of the priority queue.
 * 
 * @param pq Pointer to the priority queue
 */
void print_queue(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H

