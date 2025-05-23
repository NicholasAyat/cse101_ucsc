#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "bfs.h"
#include "dijkstra.h"

// Function to read graph from file
int** readGraph(const char* filename, int* num_cities) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    // Read number of cities and edges
    int num_edges;
    fscanf(file, "%d", num_cities);
    fscanf(file, "%d", &num_edges);

    // Allocate and initialize adjacency matrix
    int** graph = (int**)malloc(*num_cities * sizeof(int*));
    for (int i = 0; i < *num_cities; i++) {
        graph[i] = (int*)calloc(*num_cities, sizeof(int));
    }

    // Read edges
    for (int i = 0; i < num_edges; i++) {
        int u, v, w;
        fscanf(file, "%d,%d,%d", &u, &v, &w);
        graph[u][v] = w;
    }

    fclose(file);
    return graph;
}

// Function to free graph memory
void freeGraph(int** graph, int num_cities) {
    for (int i = 0; i < num_cities; i++) {
        free(graph[i]);
    }
    free(graph);
}

// Function to calculate distances from source to all vertices
int* calculateDistances(int** graph, int num_cities, int src) {
    int* dist = (int*)malloc(num_cities * sizeof(int));
    int* visited = (int*)calloc(num_cities, sizeof(int));
    
    // Initialize distances
    for (int i = 0; i < num_cities; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    
    // Find shortest path for all vertices
    for (int count = 0; count < num_cities - 1; count++) {
        int min = INT_MAX, u = -1;
        
        // Find vertex with minimum distance
        for (int v = 0; v < num_cities; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        visited[u] = 1;
        
        // Update distances
        for (int v = 0; v < num_cities; v++) {
            if (!visited[v] && graph[u][v] > 0 && 
                dist[u] != INT_MAX && 
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    
    free(visited);
    return dist;
}

int main() {
    char filename[256];
    int command;
    int num_cities;
    int** graph = NULL;

    while (1) {
        // Read command
        if (scanf("%s", filename) != 1) {
            break;
        }

        // If it's a number, it's a command
        if (filename[0] >= '0' && filename[0] <= '9') {
            command = atoi(filename);
            
            switch (command) {
                case 0:  // BFS
                    if (graph) {
                        bfs(graph, num_cities, 0);
                    }
                    break;
                    
                case 1:  // Dijkstra
                    if (graph) {
                        dijkstra(graph, num_cities, 0);
                    }
                    break;
                    
                case 2:  // Bonus task
                    if (graph) {
                        // Forward distances (0 to all)
                        int* dist = calculateDistances(graph, num_cities, 0);
                        for (int i = 0; i < num_cities; i++) {
                            printf("Vertex:%d,Distance:%d\n", i, dist[i] == INT_MAX ? -1 : dist[i]);
                        }
                        free(dist);
                        
                        // Create reversed graph for backward distances
                        int** reversed = (int**)malloc(num_cities * sizeof(int*));
                        for (int i = 0; i < num_cities; i++) {
                            reversed[i] = (int*)malloc(num_cities * sizeof(int));
                            for (int j = 0; j < num_cities; j++) {
                                reversed[i][j] = graph[j][i];
                            }
                        }
                        
                        // Calculate distances from each vertex to n-1
                        for (int i = 0; i < num_cities; i++) {
                            dist = calculateDistances(graph, num_cities, i);
                            printf("Vertex:%d,Distance:%d\n", i, dist[num_cities-1] == INT_MAX ? -1 : dist[num_cities-1]);
                            free(dist);
                        }
                        
                        // Cleanup reversed graph
                        for (int i = 0; i < num_cities; i++) {
                            free(reversed[i]);
                        }
                        free(reversed);
                    }
                    break;
                    
                case 3:  // Exit
                    if (graph) {
                        freeGraph(graph, num_cities);
                    }
                    printf("Goodbye\n");
                    return 0;
            }
        } else {
            // It's a filename, read the graph
            if (graph) {
                freeGraph(graph, num_cities);
            }
            graph = readGraph(filename, &num_cities);
        }
    }

    if (graph) {
        freeGraph(graph, num_cities);
    }
    return 0;
}