#pragma once
//#include <pebble.h>

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                    Macros                                    |
|                                                                              |
|    General-purpose macros and system-specific things.                        |
|                                                                              |
`-----------------------------------------------------------------------------*/

#ifdef NGFX_IS_CORE
#if !defined NGFX_PREFERRED_free || \
    !defined NGFX_PREFERRED_calloc || \
    !defined NGFX_PREFERRED_malloc || \
    !defined NGFX_PREFERRED_resource_load
    #error "The macros for neographics are not defined!"
#endif
#else
    #ifdef BIP
        #define NGFX_PREFERRED_free          (vPortFree)
        #define NGFX_PREFERRED_calloc        (calloc)       // write an impl of calloc
        #define NGFX_PREFERRED_malloc        (vPortMalloc)
        #define NGFX_PREFERRED_resource_load (resource_load)// ?
    #else
        #define NGFX_PREFERRED_free          (free)
        #define NGFX_PREFERRED_calloc        (calloc)
        #define NGFX_PREFERRED_malloc        (malloc)
        #define NGFX_PREFERRED_resource_load (resource_load)
    #endif
#endif

#define __BOUND_NUM(a, b, c) ((b) <= (a) ? (a) : ((b) >= (c) ? (c) : (b)))
#ifdef PBL_BW
#define __ARGB_TO_INTERNAL(a) ((((a) & 0b111111) == 0b111111) ? 0b11111111 :\
                               (((a) & 0b111111) == 0) ? 0b00000000 : 0b01010101)
#endif

#define abssub(x, y) ((x) > (y) ? (x) - (y) : (y) - (x))

#define true 1

#define false 0

#ifdef PBL_RECT
    #ifndef __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT
        #ifdef BIP
            #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT (176 / 8)
        #else 
            #ifdef PBL_BW
                #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT (160 / 8)
            #else
                #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT (144)
            #endif
        #endif
    #endif
    #ifdef BIP
        #define __SCREEN_WIDTH 176
        #define __SCREEN_HEIGHT 176
    else
        #define __SCREEN_WIDTH 144
        #define __SCREEN_HEIGHT 168
    #endif
#else
    #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT 180
    #define __SCREEN_WIDTH 180
    #define __SCREEN_HEIGHT 180
#endif
