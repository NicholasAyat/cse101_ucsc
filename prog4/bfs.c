#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bfs(int **graph, size_t num_cities, int start) {
    // Create visited array and distance array
    int *visited = (int *)calloc(num_cities, sizeof(int));
    int *distance = (int *)malloc(num_cities * sizeof(int));
    for (size_t i = 0; i < num_cities; i++) {
        distance[i] = -1;  // Initialize all distances to -1 (unreachable)
    }
    
    // Create queue for BFS
    Queue *q = createQueue(num_cities);
    
    // Mark start node as visited and set its distance to 0
    visited[start] = 1;
    distance[start] = 0;
    enqueue(q, start);
    
    while (!isEmpty(q)) {
        int current = dequeue(q);
        
        // Check all adjacent cities
        for (size_t i = 0; i < num_cities; i++) {
            if (graph[current][i] > 0 && !visited[i]) {
                visited[i] = 1;
                distance[i] = distance[current] + 1;  // Each edge has weight 1
                enqueue(q, i);
            }
        }
    }
    
    // Print the distance to the last city (N-1)
    printf("%d\n", distance[num_cities - 1]);
    
    // Cleanup
    free(visited);
    free(distance);
    freeQueue(q);
}