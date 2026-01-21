#pragma once
#include "../all.h"

/**
 * Draws a rounded rectangle path on a Cairo context.
 *
 * @param cr The Cairo context to draw on.
 * @param x The x-coordinate of the top-left corner.
 * @param y The y-coordinate of the top-left corner.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param radius The corner radius.
 */
void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius);
