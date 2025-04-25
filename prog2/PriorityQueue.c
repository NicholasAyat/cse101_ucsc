// Implement all the functions from PriorityQueue.h in this file. In addition, you are allowed to create any helper functions as well if you need to do so.

#include "PriorityQueue.h"

// Local helper function to swap two Person elements
static void pq_swap(Person* a, Person* b) {
    Person temp = *a;
    *a = *b;
    *b = temp;
}

Person* getMax(Heap* heap) {
    if (heap == NULL || heap->size == 0) return NULL;
    return &heap->arr[0];
}

Person* extractMax(Heap* heap) {
    if (heap == NULL || heap->size == 0) return NULL;

    // Store the root to return
    Person* max = (Person*)malloc(sizeof(Person));
    *max = heap->arr[0];

    // Replace root with last element
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    // Maintain heap property
    heapify(heap, 0);

    return max;
}

void insert(Heap* heap, Person* person) {
    if (heap == NULL || person == NULL) return;

    // If heap is at capacity, double the capacity
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->arr = (Person*)realloc(heap->arr, heap->capacity * sizeof(Person));
    }

    // Insert at the end
    int i = heap->size;
    heap->arr[i] = *person;
    heap->size++;

    // Fix the heap property if violated
    while (i != 0 && heap->arr[parent(i)].age < heap->arr[i].age) {
        pq_swap(&heap->arr[i], &heap->arr[parent(i)]);
        i = parent(i);
    }
}

void increasePriority(Heap* heap, int index, int new_priority) {
    if (heap == NULL || index < 0 || index >= heap->size || new_priority <= heap->arr[index].age)
        return;

    heap->arr[index].age = new_priority;
    
    // Fix the heap property if violated
    while (index != 0 && heap->arr[parent(index)].age < heap->arr[index].age) {
        pq_swap(&heap->arr[index], &heap->arr[parent(index)]);
        index = parent(index);
    }
}