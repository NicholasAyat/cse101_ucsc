// Implement all the functions from Heap.h in this file. In addition, you are allowed to create any helper functions as well if you need to do so.

#include "Heap.h"

// Helper function to swap two Person elements
void swap(Person* a, Person* b) {
    Person temp = *a;
    *a = *b;
    *b = temp;
}

Heap* initializeHeap(Person* arr, int capacity, int size) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->arr = arr;
    h->capacity = capacity;
    h->size = size;

    // Build heap (rearrange array)
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(h, i);

    return h;
}

void destructHeap(Heap** h) {
    if (h == NULL || *h == NULL) return;
    
    // Free each person's dynamically allocated fields
    for (int i = 0; i < (*h)->size; i++) {
        free((*h)->arr[i].name);
        free((*h)->arr[i].type);
        free((*h)->arr[i].destinations);
    }
    
    // Free the array itself
    free((*h)->arr);
    
    // Free the heap structure
    free(*h);
    *h = NULL;
}

void heapify(Heap* h, int i) {
    if (h == NULL) return;
    
    int largest = i;
    int l = left(i);
    int r = right(i);

    // Compare with left child
    if (l < h->size && h->arr[l].age > h->arr[largest].age)
        largest = l;

    // Compare with right child
    if (r < h->size && h->arr[r].age > h->arr[largest].age)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(&h->arr[i], &h->arr[largest]);
        heapify(h, largest);
    }
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return (2 * i + 1);
}

int right(int i) {
    return (2 * i + 2);
}

void printHeap(Heap* h) {
    if (h == NULL) return;
    
    for (int i = 0; i < h->size; i++) {
        printf("%s(%d) ", h->arr[i].name, h->arr[i].age);
    }
    printf("\n");
}

int len(Heap* h) {
    return h ? h->size : 0;
}

int size(Heap* h) {
    return h ? h->capacity : 0;
}