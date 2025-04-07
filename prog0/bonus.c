#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

// Define colors
#define BLUE 0.0, 0.0, 1.0
#define GREEN 0.0, 1.0, 0.0
#define RED 1.0, 0.0, 0.0

// Structure to represent a point
typedef struct {
    double x;
    double y;
} Point;

// Function to calculate vector between two points
Point vector_between(Point p1, Point p2) {
    return (Point){p2.x - p1.x, p2.y - p1.y};
}

// Function to normalize a vector
Point normalize_vector(Point v) {
    double length = sqrt(v.x * v.x + v.y * v.y);
    return (Point){v.x / length, v.y / length};
}

// Function to scale a vector
Point scale_vector(Point v, double scale) {
    return (Point){v.x * scale, v.y * scale};
}

// Function to rotate a vector by angle (in radians)
Point rotate_vector(Point v, double angle) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return (Point){
        v.x * cos_a - v.y * sin_a,
        v.x * sin_a + v.y * cos_a
    };
}

// Function to add two points
Point add_points(Point p1, Point p2) {
    return (Point){p1.x + p2.x, p1.y + p2.y};
}

// Function to subtract points
Point subtract_points(Point p1, Point p2) {
    return (Point){p1.x - p2.x, p1.y - p2.y};
}

// Function to calculate midpoint between two points
Point midpoint(Point p1, Point p2) {
    return (Point){(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
}

// Function to calculate square corners given an edge
void calculate_square_corners(Point p1, Point p2, Point corners[4]) {
    Point edge = vector_between(p1, p2);
    double edge_length = sqrt(edge.x * edge.x + edge.y * edge.y);
    Point perpendicular = (Point){-edge.y, edge.x}; // Rotate 90 degrees CCW
    perpendicular = scale_vector(perpendicular, edge_length / sqrt(edge.x * edge.x + edge.y * edge.y));
    
    corners[0] = p1;
    corners[1] = p2;
    corners[2] = add_points(p2, perpendicular);
    corners[3] = add_points(p1, perpendicular);
}

// Function to draw a square with given corners
void draw_square(cairo_t *cr, Point corners[4]) {
    cairo_move_to(cr, corners[0].x, corners[0].y);
    for (int i = 1; i < 4; i++) {
        cairo_line_to(cr, corners[i].x, corners[i].y);
    }
    cairo_close_path(cr);
    cairo_fill(cr);
}

// Function to get quadrant information
void get_quadrant_info(Point corners[4], int quadrant, Point *center, double *size) {
    // Calculate edge vectors
    Point edge1 = vector_between(corners[0], corners[1]);
    Point edge2 = vector_between(corners[0], corners[3]);
    
    // Calculate the edge length
    double edge_length = sqrt(edge1.x * edge1.x + edge1.y * edge1.y);
    
    // Calculate center of parent square
    Point square_center = midpoint(corners[0], corners[2]);
    
    // Calculate midpoints for each quadrant (scaled relative to parent edges)
    double quadrant_offset = 0.25; // 1/4 distance from center
    
    // Determine direction for this quadrant
    double x_dir = (quadrant % 2 == 0) ? -1.0 : 1.0;  // 0,2: negative, 1,3: positive
    double y_dir = (quadrant < 2) ? -1.0 : 1.0;       // 0,1: negative, 2,3: positive
    
    // Calculate offset using quadrant and parent edge vectors
    Point offset = add_points(
        scale_vector(edge1, x_dir * quadrant_offset),
        scale_vector(edge2, y_dir * quadrant_offset)
    );
    
    *center = add_points(square_center, offset);
    
    // Size is exactly half of parent's edge length
    *size = edge_length / 2.0;
}

// Function to draw the fractal pattern recursively
void draw_fractal(cairo_t *cr, Point p1, Point p2, int depth) {
    // Calculate the corners of the current square
    Point corners[4];
    calculate_square_corners(p1, p2, corners);
    
    // Set color based on depth
    if (depth % 2 == 0) {
        cairo_set_source_rgb(cr, GREEN);
    } else {
        cairo_set_source_rgb(cr, RED);
    }
    
    // Draw the current square
    draw_square(cr, corners);
    
    // Base case
    if (depth <= 0) return;
    
    // Draw squares in each quadrant
    for (int i = 0; i < 4; i++) {
        Point center;
        double size;
        get_quadrant_info(corners, i, &center, &size);
        
        // Calculate the angle of the parent square's edge
        Point edge1 = vector_between(corners[0], corners[1]);
        double parent_angle = atan2(edge1.y, edge1.x);
        
        // Generate a random value for rotation (0 to 1)
        double r = (double)rand() / RAND_MAX;
        
        // Create a square with the same orientation as the parent, but with random offsets
        // First create basic square points
        Point offset_corners[4];
        
        // Apply random offsets similar to prog0.c approach, but pre-rotation
        offset_corners[0] = (Point){-size/2 + r * size, -size/2};
        offset_corners[1] = (Point){size/2, -size/2 + r * size};
        offset_corners[2] = (Point){size/2 - r * size, size/2};
        offset_corners[3] = (Point){-size/2, size/2 - r * size};
        
        // Rotate these corners to match parent orientation and translate to quadrant center
        Point child_corners[4];
        for (int j = 0; j < 4; j++) {
            Point rotated = rotate_vector(offset_corners[j], parent_angle);
            child_corners[j] = add_points(center, rotated);
        }
        
        // Recurse with the first edge of the new square
        draw_fractal(cr, child_corners[0], child_corners[1], depth - 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s x1 y1 x2 y2 size depth\n", argv[0]);
        return 1;
    }
    
    // Parse command line arguments
    int size = atoi(argv[5]);
    int depth = atoi(argv[6]);
    
    // Create the surface and context
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, size, size);
    cairo_t *cr = cairo_create(surface);
    
    // Set blue background
    cairo_set_source_rgb(cr, BLUE);
    cairo_paint(cr);
    
    // Convert input coordinates to match the canvas
    Point p1 = {atof(argv[1]), size - atof(argv[2])};
    Point p2 = {atof(argv[3]), size - atof(argv[4])};
    
    // Draw the fractal pattern
    draw_fractal(cr, p1, p2, depth);
    
    // Save to PNG and cleanup
    cairo_surface_write_to_png(surface, "bonus.png");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    
    return 0;
} 