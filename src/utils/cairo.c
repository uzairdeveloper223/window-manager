#include "../all.h"

void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius)
{
    // Draw top edge and top-right corner.
    cairo_move_to(cr, x + radius, y);
    cairo_line_to(cr, x + width - radius, y);
    cairo_arc(cr, x + width - radius, y + radius, radius, -PI / 2, 0);

    // Draw right edge and bottom-right corner.
    cairo_line_to(cr, x + width, y + height - radius);
    cairo_arc(cr, x + width - radius, y + height - radius, radius, 0, PI / 2);

    // Draw bottom edge and bottom-left corner.
    cairo_line_to(cr, x + radius, y + height);
    cairo_arc(cr, x + radius, y + height - radius, radius, PI / 2, PI);

    // Draw left edge and top-left corner.
    cairo_line_to(cr, x, y + radius);
    cairo_arc(cr, x + radius, y + radius, radius, PI, 3 * PI / 2);

    cairo_close_path(cr);
}
