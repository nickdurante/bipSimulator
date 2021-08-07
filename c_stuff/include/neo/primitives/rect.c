#include "rect.h"
#include <stddef.h>

n_GPoint n_grect_center_point(n_GRect *rect) {
    short x, y;
    x = rect->origin.x + rect->size.w / 2;
    y = rect->origin.y + rect->size.h / 2;
    return n_GPoint(x, y);
}

char n_gpoint_equal(const n_GPoint *point_a, const n_GPoint *point_b) {
    if (point_a == NULL || point_b == NULL)
        return false;
    return
        point_a->x == point_b->x &&
        point_a->y == point_b->y;
}

char n_gsize_equal(const n_GSize *size_a, const n_GSize *size_b) {
    if (size_a == NULL || size_b == NULL)
        return false;
    return
        size_a->w == size_b->w &&
        size_a->h == size_b->h;
}

char n_grect_equal(const n_GRect *rect_a, const n_GRect *rect_b) {
    if (rect_a == NULL || rect_b == NULL)
        return false;
    return
        rect_a->origin.x == rect_b->origin.x &&
        rect_a->origin.y == rect_b->origin.y &&
        rect_a->size.w == rect_b->size.w &&
        rect_a->size.h == rect_b->size.h;
}

char n_grect_is_empty(const n_GRect *const rect) {
    return rect == NULL || (rect->size.w == 0 && rect->size.h == 0);
}

void n_grect_clip(n_GRect *rect_to_clip, const n_GRect *rect_clipper) {
    if (rect_to_clip == NULL || rect_clipper == NULL)
        return;
    if (rect_to_clip->origin.x < rect_clipper->origin.x) {
        rect_to_clip->size.w -= rect_clipper->origin.x - rect_to_clip->origin.x;
        rect_to_clip->origin.x = rect_clipper->origin.x;
    }
    if (rect_to_clip->origin.y < rect_clipper->origin.y) {
        rect_to_clip->size.h -= rect_clipper->origin.y - rect_to_clip->origin.y;
        rect_to_clip->origin.y = rect_clipper->origin.y;
    }

    short clipper_right = rect_clipper->origin.x + rect_clipper->size.w;
    if (rect_to_clip->origin.x + rect_to_clip->size.w > clipper_right) {
        rect_to_clip->size.w = clipper_right - rect_to_clip->origin.x;
    }
    short clipper_bottom = rect_clipper->origin.y + rect_clipper->size.h;
    if (rect_to_clip->origin.y + rect_to_clip->size.h > clipper_bottom) {
        rect_to_clip->size.h = clipper_bottom - rect_to_clip->origin.y;
    }

    if (rect_to_clip->size.w < 0) {
        rect_to_clip->size.w = 0;
    }
    if (rect_to_clip->size.h < 0) {
        rect_to_clip->size.h = 0;
    }
}

char n_grect_contains_point(const n_GRect *rect, const n_GPoint *point) {
    if (rect == NULL || point == NULL)
        return false;
    return
        point->x >= rect->origin.x &&
        point->y >= rect->origin.y &&
        point->x < rect->origin.x + rect->size.w &&
        point->y < rect->origin.y + rect->size.h;
}

n_GRect n_grect_crop(n_GRect rect, int crop_size_px) {
    return n_GRect(
        rect.origin.x + crop_size_px / 2,
        rect.origin.y + crop_size_px / 2,
        rect.size.w < crop_size_px ? 0 : rect.size.w - crop_size_px,
        rect.size.h < crop_size_px ? 0 : rect.size.h - crop_size_px
    );
}

static void n_graphics_draw_thin_rect_bounded(
        n_GContext * ctx, n_GRect rect,
        unsigned short minx, unsigned short maxx, unsigned short miny, unsigned short maxy) {
#ifdef PBL_BW
    char color = __ARGB_TO_INTERNAL(ctx->stroke_color.argb);
#else
    char color = ctx->stroke_color.argb;
#endif
    n_graphics_prv_draw_col(ctx->fbuf, rect.origin.x,
            rect.origin.y, rect.origin.y + rect.size.h - 1,
            minx, maxx, miny, maxy, color);
    n_graphics_prv_draw_row(ctx->fbuf, rect.origin.y,
            rect.origin.x, rect.origin.x + rect.size.w - 1,
            minx, maxx, miny, maxy, color);
    n_graphics_prv_draw_col(ctx->fbuf, rect.origin.x + rect.size.w - 1,
            rect.origin.y, rect.origin.y + rect.size.h - 1,
            minx, maxx, miny, maxy, color);
    n_graphics_prv_draw_row(ctx->fbuf, rect.origin.y + rect.size.h - 1,
            rect.origin.x, rect.origin.x + rect.size.w - 1,
            minx, maxx, miny, maxy, color);
}

static void n_graphics_draw_rect_bounded(
        n_GContext * ctx, n_GRect rect, unsigned short radius, n_GCornerMask mask,
        unsigned short minx, unsigned short maxx, unsigned short miny, unsigned short maxy) {
    // NB this could be changed in the future to allow for more shapes, for
    // example one with opposite corners rounded & radius equal to width.


    radius = __BOUND_NUM(0, __BOUND_NUM(0, radius, rect.size.h / 2), rect.size.w / 2);

    // NB potential for more optimization; b/w currently runs at half speed of color.

    if (mask & n_GCornerTopLeft)
        n_graphics_prv_draw_quarter_circle_bounded(ctx,
                n_GPoint(rect.origin.x + radius,
                         rect.origin.y + radius),
                radius, ctx->stroke_width, -1, -1, minx, maxx, miny, maxy);
    if (mask & n_GCornerTopRight)
        n_graphics_prv_draw_quarter_circle_bounded(ctx,
                n_GPoint(rect.origin.x + rect.size.w - radius - 1,
                         rect.origin.y + radius),
                radius, ctx->stroke_width, 1, -1, minx, maxx, miny, maxy);
    if (mask & n_GCornerBottomLeft)
        n_graphics_prv_draw_quarter_circle_bounded(ctx,
                n_GPoint(rect.origin.x + radius,
                         rect.origin.y + rect.size.h - radius - 1),
                radius, ctx->stroke_width, -1, 1, minx, maxx, miny, maxy);
    if (mask & n_GCornerBottomRight)
        n_graphics_prv_draw_quarter_circle_bounded(ctx,
                n_GPoint(rect.origin.x + rect.size.w - radius - 1,
                         rect.origin.y + rect.size.h - radius - 1),
                radius, ctx->stroke_width, 1, 1, minx, maxx, miny, maxy);

    // NB potential for optimization by disabling unnecessary stroke caps

    // Connect corners to each other clockwise.
    n_graphics_draw_line(ctx,
        n_GPoint(rect.origin.x + (mask & n_GCornerTopLeft ? radius : 0),
                 rect.origin.y),
        n_GPoint(rect.origin.x + rect.size.w - 1
                               - (mask & n_GCornerTopRight ? radius : 0),
                 rect.origin.y));
    n_graphics_draw_line(ctx,
        n_GPoint(rect.origin.x + rect.size.w - 1,
                 rect.origin.y + (mask & n_GCornerTopRight ? radius : 0)),
        n_GPoint(rect.origin.x + rect.size.w - 1,
                 rect.origin.y + rect.size.h - 1
                               - (mask & n_GCornerBottomRight ? radius : 0)));
    n_graphics_draw_line(ctx,
        n_GPoint(rect.origin.x + rect.size.w - 1
                               - (mask & n_GCornerBottomRight ? radius : 0),
                 rect.origin.y + rect.size.h - 1),
        n_GPoint(rect.origin.x + (mask & n_GCornerBottomLeft ? radius : 0),
                 rect.origin.y + rect.size.h - 1));
    n_graphics_draw_line(ctx,
        n_GPoint(rect.origin.x,
                 rect.origin.y + rect.size.h - 1
                               - (mask & n_GCornerBottomLeft ? radius : 0)),
        n_GPoint(rect.origin.x,
                 rect.origin.y + (mask & n_GCornerTopLeft ? radius : 0)));
}

void n_graphics_draw_thin_rect(n_GContext * ctx, n_GRect rect) {
    n_graphics_draw_thin_rect_bounded(ctx, n_grect_standardize(rect), 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
}

void n_graphics_draw_rect(n_GContext * ctx, n_GRect rect, unsigned short radius, n_GCornerMask mask) {
    if (!(ctx->stroke_color.argb & (0b11 << 6)))
        ;
    else if (ctx->stroke_width == 1 && (radius == 0 || mask == 0))
        n_graphics_draw_thin_rect_bounded(ctx, n_grect_standardize(rect), 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
    else
        n_graphics_draw_rect_bounded(ctx, n_grect_standardize(rect), radius, mask, 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
}

static void n_graphics_fill_rect_bounded(n_GContext * ctx, n_GRect rect, unsigned short radius, n_GCornerMask mask,
        unsigned short minx, unsigned short maxx, unsigned short miny, unsigned short maxy) {
    // NB this could be changed in the future to allow for more shapes, for
    // example one with opposite corners rounded & radius equal to width.:
    if (!(mask & 0b1111)) {
        radius = 0;
    } else {
        radius = __BOUND_NUM(0, radius, rect.size.w / 2);
        radius = __BOUND_NUM(0, radius, rect.size.h / 2);
    }

#ifdef PBL_BW
    char color = __ARGB_TO_INTERNAL(ctx->fill_color.argb);
#else
    char color = ctx->fill_color.argb;
#endif

    // TODO these should be inlined & full rows should be drawn to maximize speed.

    if (radius) {
        if (mask & n_GCornerTopLeft)
            n_graphics_prv_fill_quarter_circle_bounded(ctx,
                    n_GPoint(rect.origin.x + radius,
                             rect.origin.y + radius),
                    radius, -1, -1, minx, maxx, miny, maxy);
        if (mask & n_GCornerTopRight)
            n_graphics_prv_fill_quarter_circle_bounded(ctx,
                    n_GPoint(rect.origin.x + rect.size.w - radius - 1,
                             rect.origin.y + radius),
                    radius, 1, -1, minx, maxx, miny, maxy);
        if (mask & n_GCornerBottomLeft)
            n_graphics_prv_fill_quarter_circle_bounded(ctx,
                    n_GPoint(rect.origin.x + radius + 1,
                             rect.origin.y + rect.size.h - radius - 1),
                    radius, -1, 1, minx, maxx, miny, maxy);
        if (mask & n_GCornerBottomRight)
            n_graphics_prv_fill_quarter_circle_bounded(ctx,
                    n_GPoint(rect.origin.x + rect.size.w - radius - 1,
                             rect.origin.y + rect.size.h - radius - 1),
                    radius, 1, 1, minx, maxx, miny, maxy);

        short left_indent_top = rect.origin.x + (mask & n_GCornerTopLeft ? radius : 0),
                right_indent_top = rect.origin.x + rect.size.w - (mask & n_GCornerTopRight ? radius : 0) - 1,
                left_indent_bottom = rect.origin.x + (mask & n_GCornerBottomLeft ? radius : 0),
                right_indent_bottom = rect.origin.x + rect.size.w - (mask & n_GCornerBottomRight ? radius : 0) - 1;

        for (unsigned short r = 0; r < radius; r++) {
            n_graphics_prv_draw_row(ctx->fbuf, rect.origin.y + r,
                left_indent_top, right_indent_top,
                minx, maxx, miny, maxy, color);
            n_graphics_prv_draw_row(ctx->fbuf, rect.origin.y + rect.size.h - 1 - r,
                left_indent_bottom, right_indent_bottom,
                minx, maxx, miny, maxy, color);
        }
    }

    short right_indent = rect.origin.x + rect.size.w - 1;
    for (short r = rect.origin.y + radius; r <= rect.origin.y + rect.size.h - radius - 1; r++) {
        n_graphics_prv_draw_row(ctx->fbuf, r,
            rect.origin.x, right_indent,
            minx, maxx, miny, maxy, color);
    }
}

static void n_graphics_fill_0rad_rect_bounded(n_GContext * ctx, n_GRect rect,
        unsigned short minx, unsigned short maxx, unsigned short miny, unsigned short maxy) {
#ifdef PBL_BW
    char color = __ARGB_TO_INTERNAL(ctx->fill_color.argb);
#else
    char color = ctx->fill_color.argb;
#endif
    short right_indent = rect.origin.x + rect.size.w - 1,
            max_y = rect.origin.y + rect.size.h - 1;
    for (short r = rect.origin.y; r <= max_y; r++) {
        n_graphics_prv_draw_row(ctx->fbuf, r,
            rect.origin.x, right_indent,
            minx, maxx, miny, maxy, color);
    }
}

void n_graphics_fill_rect(n_GContext * ctx, n_GRect rect, unsigned short radius, n_GCornerMask mask) {
    if (!(ctx->fill_color.argb & (0b11 << 6)))
        ;
    else if (radius == 0 || (mask & 0b1111) == 0)
        n_graphics_fill_0rad_rect_bounded(ctx, rect, 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
    else
        n_graphics_fill_rect_bounded(ctx, rect, radius, mask, 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
}
