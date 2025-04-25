#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

// Comparison function for integers
int compare_ints(void *a, void *b) {
    int x = *(int *)a;
    int y = *(int *)b;
    return y - x; // Min-heap: lower number = higher priority
}

void test_basic_operations() {
    printf("\n=== Testing Basic Operations ===\n");
    PriorityQueue pq;
    pq_init(&pq, compare_ints);

    int *a = malloc(sizeof(int)); *a = 10;
    int *b = malloc(sizeof(int)); *b = 30;
    int *c = malloc(sizeof(int)); *c = 20;
    int *d = malloc(sizeof(int)); *d = 50;
    int *e = malloc(sizeof(int)); *e = 60;

    pq_insert(&pq, a);
    pq_insert(&pq, b);
    pq_insert(&pq, c);
    pq_insert(&pq, d);
    pq_insert(&pq, e);
    print_queue(&pq);

    printf("Priority Queue Output:\n");
    while (!pq_is_empty(&pq)) {
        int *val = (int *)pq_delete(&pq);
        printf("Popped: %d\n", *val);
        free(val);
        print_queue(&pq);
    }

    pq_destroy(&pq);
}

void test_empty_queue() {
    printf("\n=== Testing Empty Queue Operations ===\n");
    PriorityQueue pq;
    pq_init(&pq, compare_ints);
    
    printf("Is empty? %s\n", pq_is_empty(&pq) ? "yes" : "no");
    void *result = pq_delete(&pq);
    printf("Delete from empty queue returns: %s\n", result == NULL ? "NULL" : "not NULL");
    
    pq_destroy(&pq);
}

void test_dynamic_resizing() {
    printf("\n=== Testing Dynamic Resizing ===\n");
    PriorityQueue pq;
    pq_init(&pq, compare_ints);
    
    printf("Inserting 20 elements to test resizing...\n");
    for (int i = 0; i < 20; i++) {
        int *val = malloc(sizeof(int));
        *val = i * 5;
        pq_insert(&pq, val);
    }
    
    print_queue(&pq);
    
    while (!pq_is_empty(&pq)) {
        int *val = (int *)pq_delete(&pq);
        free(val);
    }
    
    pq_destroy(&pq);
}

int main() {
    test_basic_operations();
    test_empty_queue();
    test_dynamic_resizing();
    return 0;
}
