#pragma once
#include "../all.h"

/**
 * Draws a drop shadow for a portal.
 *
 * Renders a multi-layer soft shadow effect behind the portal to create
 * depth and visual separation from the background.
 *
 * @param cr The Cairo context to draw on.
 * @param portal The portal to draw the shadow for.
 */
void draw_portal_shadow(cairo_t *cr, Portal *portal);
