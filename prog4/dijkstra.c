#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Helper function to find vertex with minimum distance value
static int minDistance(int *dist, int *visited, int num_cities) {
    int min = INT_MAX, min_index = -1;
    
    for (int v = 0; v < num_cities; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }
    
    return min_index;
}

// Function to perform Dijkstra's algorithm on a graph represented as an adjacency matrix
void dijkstra(int **graph, int num_cities, int src) {
    int *dist = (int *)malloc(num_cities * sizeof(int));
    int *visited = (int *)calloc(num_cities, sizeof(int));
    
    // Initialize distances
    for (int i = 0; i < num_cities; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    
    // Find shortest path for all vertices
    for (int count = 0; count < num_cities - 1; count++) {
        int u = minDistance(dist, visited, num_cities);
        
        // If no more reachable vertices, break
        if (u == -1) break;
        
        visited[u] = 1;
        
        // Update distances of adjacent vertices
        for (int v = 0; v < num_cities; v++) {
            if (!visited[v] && graph[u][v] > 0 && 
                dist[u] != INT_MAX && 
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    
    // Print the distance to the last city (N-1)
    if (dist[num_cities - 1] == INT_MAX) {
        printf("-1\n");
    } else {
        printf("%d\n", dist[num_cities - 1]);
    }
    
    // Cleanup
    free(dist);
    free(visited);
}
