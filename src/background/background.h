#pragma once
#include "../all.h"

/**
 * Draws the background to the given Cairo context.
 * Handles both solid color and image modes based on configuration.
 *
 * @param cr The Cairo context to draw to.
 */
void draw_background(cairo_t *cr);
