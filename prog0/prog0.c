#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

// Structure to represent a point
typedef struct {
    double x;
    double y;
} Point;

// Function to draw a rotated square within given bounds
void draw_rotated_square(cairo_t *cr, double x, double y, double size) {
    double r = (double)rand() / RAND_MAX * size; // Random offset between 0 and size
    
    // Calculate the four points of the rotated square
    Point points[4];
    points[0] = (Point){x + r, y};
    points[1] = (Point){x + size, y + r};
    points[2] = (Point){x + size - r, y + size};
    points[3] = (Point){x, y + size - r};

    // Draw the filled square
    cairo_move_to(cr, points[0].x, points[0].y);
    for (int i = 1; i < 4; i++) {
        cairo_line_to(cr, points[i].x, points[i].y);
    }
    cairo_close_path(cr);
    cairo_fill(cr);
}

// Recursive function to draw squares
void draw_recursive_squares(cairo_t *cr, double x, double y, double size, int depth) {
    // Base case check
    if (size < 8) return; // Minimum square size check
    
    if (depth == 0) {
        // Only draw square when we've reached the target depth
        draw_rotated_square(cr, x, y, size);
    } else {
        // Otherwise just recurse into quadrants
        double half = size / 2;
        draw_recursive_squares(cr, x, y, half, depth - 1);           // Top left
        draw_recursive_squares(cr, x + half, y, half, depth - 1);    // Top right
        draw_recursive_squares(cr, x, y + half, half, depth - 1);    // Bottom left
        draw_recursive_squares(cr, x + half, y + half, half, depth - 1); // Bottom right
    }
}

int main(int argc, char *argv[]) {
    int size, depth;
    
    // Read input from stdin
    if (scanf("%d %d", &size, &depth) != 2) {
        fprintf(stderr, "Error: Please provide size and depth\n");
        return 1;
    }
    
    // Validate input
    if (size < 8) {
        fprintf(stderr, "Error: Size must be at least 8 pixels\n");
        return 1;
    }
    
    // Check if recursion depth is too high for the given size
    double smallest_square = size / pow(2, depth);
    if (smallest_square < 8) {
        fprintf(stderr, "Warning: Increase length or decrease level\n");
        return 1;
    }

    // Create the surface and context
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, size, size);
    cairo_t *cr = cairo_create(surface);
    
    // Set black background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    
    // Set white for squares
    cairo_set_source_rgb(cr, 1, 1, 1);
    
    // Start the recursive drawing
    draw_recursive_squares(cr, 0, 0, size, depth);
    
    // Save to PNG and cleanup
    cairo_surface_write_to_png(surface, "square.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    
    return 0;
} 