#pragma once
//#include <pebble.h>
#include "../types.h"
#include "../macros.h"
#include "../common.h"
#include "../context.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                   Fonts                                      |
|                                                                              |
|                         Abstracts away font magicks.                         |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup fonts
 *  Get glyph information.
 *  @{
 */

/*!
 * Stores basic information about a font.
 */
typedef struct n_GFontInfo {
    char version;
    char line_height;
    unsigned short glyph_amount;
    unsigned short wildcard_codepoint;
    // v2+
    char hash_table_size;
    char codepoint_bytes;
    // v3+
    char fontinfo_size;
    char features;
} __attribute__((__packed__)) n_GFontInfo;

/*!
 * An entry in the hash table, which stores a hash (computed by: codepoint
 * modulo hash table length) and gives the offset table offset for that value.
 */
typedef struct n_GFontHashTableEntry {
    char hash_value;
    char offset_table_size;
    unsigned short offset_table_offset;
} __attribute__((__packed__)) n_GFontHashTableEntry;

/*!
 * Length of the n_GFontInfo for font version 1
 */
#define __FONT_INFO_V1_LENGTH 6
/*!
 * Length of the n_GFontInfo for font version 2
 */
#define __FONT_INFO_V2_LENGTH 8

/*!
 * Bitmasks which determine whether certain features are
 * available in a font.
 */
typedef enum {
    n_GFontFeature2ByteGlyphOffset = 0b1,
    n_GFontFeatureRLE4Encoding = 0b10,
} n_GFontFeatures;

/*!
 * Metadata and data of a glyph.
 */
typedef struct n_GGlyphInfo {
    char width;
    char height;
    signed char left_offset;
    signed char top_offset;
    signed char advance;
    char data[];
} __attribute__((__packed__)) n_GGlyphInfo;

#ifdef REBBLEOS
/*!
 * Inside RebbleOS, fonts are actually files. 
 */
typedef struct file * n_GFont;
#else
/*!
 * Alias for user-facing API.
 */
typedef n_GFontInfo * n_GFont;
#endif

/*!
 * Get the line height of a given font.
 */
char n_graphics_font_get_line_height(n_GFont font);

/*!
 * Get glyph information for a charcode from a font (tofu if not available)
 */
n_GGlyphInfo * n_graphics_font_get_glyph_info(n_GFont font, unsigned int charcode);

/*!
 * Draw a glyph.
 */
void n_graphics_font_draw_glyph(n_GContext * ctx, n_GGlyphInfo * glyph, n_GPoint p);

/*! @}
 */
