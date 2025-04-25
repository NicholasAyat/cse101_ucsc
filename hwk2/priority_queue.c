// priority_queue.c
#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"

#define INITIAL_CAPACITY 16 

// Helper functions to calculate indices
int parent(int i) { return (i - 1) / 2; }
int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }

void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue *pq, int idx) {
    while (idx > 0) {
        int p = parent(idx);
        if (pq->cmp(pq->data[idx], pq->data[p]) < 0) {
            swap(&pq->data[idx], &pq->data[p]);
            idx = p;
        } else {
            break;
        }
    }
}

void heapify_down(PriorityQueue *pq, int idx) {
    int largest = idx;
    int l = left(idx);
    int r = right(idx);

    if (l < pq->size && pq->cmp(pq->data[l], pq->data[largest]) < 0)
        largest = l;

    if (r < pq->size && pq->cmp(pq->data[r], pq->data[largest]) < 0)
        largest = r;

    if (largest != idx) {
        swap(&pq->data[idx], &pq->data[largest]);
        heapify_down(pq, largest);
    }
}

void pq_init(PriorityQueue *pq, Comparator cmp) {
    pq->capacity = INITIAL_CAPACITY;
    pq->size = 0;
    pq->cmp = cmp;
    pq->data = malloc(sizeof(void*) * pq->capacity);
}

void pq_insert(PriorityQueue *pq, void *node) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->data = realloc(pq->data, sizeof(void*) * pq->capacity);
    }
    
    pq->data[pq->size] = node;
    heapify_up(pq, pq->size);
    pq->size++;
}

void *pq_delete(PriorityQueue *pq) {
    if (pq_is_empty(pq)) {
        return NULL;
    }

    void *root = pq->data[0];
    pq->size--;
    
    if (pq->size > 0) {
        pq->data[0] = pq->data[pq->size];
        heapify_down(pq, 0);
    }

    return root;
}

void print_queue(PriorityQueue *pq) {
    for (int i = 0; i < pq->size; i++) {
        printf("%d", *(int*)pq->data[i]);
        if (i < pq->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int pq_is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

void pq_destroy(PriorityQueue *pq) {
    free(pq->data);
    pq->data = NULL;
    pq->size = 0;
    pq->capacity = 0;
}

