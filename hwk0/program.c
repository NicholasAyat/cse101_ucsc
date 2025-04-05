#include <cairo.h>
#include <stdio.h>

#define SIZE 512

int main() {
    cairo_surface_t *surface;
    cairo_t *cr;

    // Create a new surface
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
    cr = cairo_create(surface);

    // Set background to red (entire square)
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  // Red
    cairo_rectangle(cr, 0, 0, SIZE, SIZE);
    cairo_fill(cr);

    // Draw green diamond by connecting midpoints
    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);  // Green
    cairo_move_to(cr, SIZE/2, 0);        // Top midpoint
    cairo_line_to(cr, SIZE, SIZE/2);     // Right midpoint
    cairo_line_to(cr, SIZE/2, SIZE);     // Bottom midpoint
    cairo_line_to(cr, 0, SIZE/2);        // Left midpoint
    cairo_close_path(cr);
    cairo_fill(cr);

    // Save the image
    cairo_surface_write_to_png(surface, "output.png");

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
} 