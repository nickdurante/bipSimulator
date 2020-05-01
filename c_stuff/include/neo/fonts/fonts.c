#include "fonts.h"

char n_graphics_font_get_line_height(n_GFontInfo * font) {
    return font->line_height;
}

n_GGlyphInfo * n_graphics_font_get_glyph_info(n_GFontInfo * font, unsigned int codepoint) {
    char * data;
    char hash_table_size = 255, codepoint_bytes = 4, features = 0;
    switch (font->version) {
        case 1:
            data = (char *) font + __FONT_INFO_V1_LENGTH;
            break;
        case 2:
            data = (char *) font + __FONT_INFO_V2_LENGTH;
            break;
        default:
            data = (char *) font + font->fontinfo_size;
    }
    switch (font->version) {
        // switch trickery! Default first is valid.
        default:
            features = font->features;
        case 2:
            hash_table_size = font->hash_table_size;
            codepoint_bytes = font->codepoint_bytes;
        case 1:
            break;
    }

    char offset_table_item_length = codepoint_bytes +
        (features & n_GFontFeature2ByteGlyphOffset ? 2 : 4);

    n_GFontHashTableEntry * hash_data =
        (n_GFontHashTableEntry *) (data +
            (codepoint % hash_table_size) * sizeof(n_GFontHashTableEntry));

    data += (hash_table_size * sizeof(n_GFontHashTableEntry));

    if (hash_data->hash_value != (codepoint % hash_table_size))
        // There was no hash table entry with the correct hash. Fall back to tofu.
        return (n_GGlyphInfo *) (data + offset_table_item_length * font->glyph_amount + 4);

    char * offset_entry = data + hash_data->offset_table_offset;

    unsigned short iters = 0; // theoretical possibility of 255 entries in an offset
                        // table mean that we can't use a uint8 for safety
    while ((codepoint_bytes == 2
               ? *((unsigned short *) offset_entry)
               : *((unsigned int *) offset_entry)) != codepoint &&
            iters < hash_data->offset_table_size) {
        offset_entry += offset_table_item_length;
        iters++;
    }

    if ((codepoint_bytes == 2
            ? *((unsigned short *) offset_entry)
            : *((unsigned int *) offset_entry)) != codepoint)
        // We couldn't find the correct entry. Fall back to tofu.
        return (n_GGlyphInfo *) (data + offset_table_item_length * font->glyph_amount + 4);

    data += offset_table_item_length * font->glyph_amount +
        (features & n_GFontFeature2ByteGlyphOffset
            ? *((unsigned short *) (offset_entry + codepoint_bytes))
            : *((unsigned int *) (offset_entry + codepoint_bytes)));

    n_GGlyphInfo * glyph = (n_GGlyphInfo *) data;

    return glyph;
}

void n_graphics_font_draw_glyph_bounded(n_GContext * ctx, n_GGlyphInfo * glyph,
    n_GPoint p, short minx, short maxx, short miny, short maxy) {
    p.x += glyph->left_offset;
    p.y += glyph->top_offset;
    for (char y = 0; y < glyph->height; y++)
        for (char x = 0; x < glyph->width; x++)
            if (glyph->data[(y*glyph->width+x)/8] & (1 << ((y*glyph->width+x) % 8)) &&
                    p.x + x >= minx && p.x + x < maxx &&
                    p.y + y >= miny && p.y + y < maxy)
                n_graphics_set_pixel(ctx, n_GPoint(p.x + x, p.y + y), ctx->text_color);
}

void n_graphics_font_draw_glyph(n_GContext * ctx, n_GGlyphInfo * glyph, n_GPoint p) {
    n_graphics_font_draw_glyph_bounded(ctx, glyph, p, 0, __SCREEN_WIDTH, 0, __SCREEN_HEIGHT);
}
