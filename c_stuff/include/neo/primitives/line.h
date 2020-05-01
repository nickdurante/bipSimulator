#pragma once
//#include <pebble.h>
#include "../macros.h"
#include "../common.h"
#include "circle.h"

/*! \addtogroup Line
 *  Draw lines of arbitrary thickness.
 *  @{
 */

/*!
 * Draws a 1px-thick line within given bounds.
 */
void n_graphics_prv_draw_1px_line_bounded(
    n_GContext * ctx, n_GPoint from, n_GPoint to,
    short minx, short maxx, short miny, short maxy);
/*!
 * Draws a thick line (width >= 2).
 */
void n_graphics_prv_draw_thick_line_bounded(
    n_GContext * ctx, n_GPoint from, n_GPoint to, char width,
    short minx, short maxx, short miny, short maxy);
/*!
 * Draws a line.
 */
void n_graphics_draw_line(n_GContext * ctx, n_GPoint from, n_GPoint to);

/*! @}
 */
