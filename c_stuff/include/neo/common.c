#include "common.h"

static void n_graphics_prv_setbit(char * byte, char pos, char val) {
    *byte ^= (-val ^ *byte) & (1 << pos);
}

void n_graphics_set_pixel(n_GContext * ctx, n_GPoint p, n_GColor color) {
#ifdef PBL_BW
    n_graphics_prv_setbit(
        &ctx->fbuf[p.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + p.x / 8],
        p.x % 8, (color.argb & 0b111111));
#else
    ctx->fbuf[p.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + p.x] = color.argb;
#endif
}

void n_graphics_draw_pixel(n_GContext * ctx, n_GPoint p) {
    n_graphics_set_pixel(ctx, p, ctx->stroke_color);
}

// NB (top < bottom) leads to undefined behavior
void n_graphics_prv_draw_col(char * fb,
        short x, short top, short bottom,
        short minx, short maxx, short miny, short maxy,
        char fill) {
    if (x < minx || x >= maxx || top >= maxy || bottom < miny || bottom < top) {
        return;
    }

    unsigned short begin = __BOUND_NUM(miny, top, maxy - 1),
             end   = __BOUND_NUM(miny, bottom, maxy - 1);

    for (unsigned short y = begin; y <= end; y++) {
#ifdef PBL_BW
        n_graphics_prv_setbit(&fb[y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + x / 8],
            x % 8, (fill >> ((y + x) % 8)) & 1);
#else
        fb[y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + x] = fill;
#endif
    }
}

void n_graphics_prv_draw_row(char * fb,
        short y, short left, short right,
        short minx, short maxx, short miny, short maxy,
        char fill) {
    char * row;
    if (y >= miny && y < maxy && right >= minx && left < maxx) {
        row = fb + (y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT);
    } else {
        return;
    }

    unsigned short begin = __BOUND_NUM(minx, left, maxx - 1),
             end   = __BOUND_NUM(minx, right, maxx - 1);

#ifdef PBL_BW
    unsigned short begin_byte = begin / 8,
             end_byte   = end / 8;
#else
    unsigned short begin_byte = begin,
             end_byte   = end;
#endif


#ifdef PBL_BW
    if (y & 1)
        fill = fill >> 1 | fill << 7;
    /*\ Brace yourselves.
    |*| Here's what's going on:
    |*| - We're on b/w, which means, 8 pixels horizontally are represented by
    |*|   1 byte of memory.
    |*| - For maximum write speed, we're grouping two rows of equal width
    |*|   (because we're drawing circles/ellipses) and rendering them
    |*|   concurrently.
    |*| - The quickest way to do that is to check:
    |*|   - Does the full row fit within exectly one bit storage space?
    |*|     - If so, iterate over pixels in row and set bits accordingly.
    |*|     - Otherwise, iterate over pixels in first and last byte of the row
    |*|       and memset everything in between.
    \*/
    if (begin_byte == end_byte) {
        for (short i = begin; i <= end; i++) {
            n_graphics_prv_setbit(&row[end_byte], i - end_byte * 8, (fill >> (i%8)) & 1);
        }
    } else {
        for (short i = begin; i <= begin_byte * 8 + 8; i++) {
            n_graphics_prv_setbit(&row[begin_byte], i - begin_byte * 8, (fill >> (i%8)) & 1);
        }
        if (end_byte - begin_byte > 1) {
            memset(row + begin_byte + 1, fill, end_byte - begin_byte - 1);
        }
        for (short i = end; i >= end_byte * 8; i--) {
            n_graphics_prv_setbit(&row[end_byte], i - end_byte * 8, (fill >> (i%8)) & 1);
        }
    }
#else
    memset(row + begin_byte, fill, end_byte - begin_byte + 1);
#endif
}
