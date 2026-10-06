#ifndef LOMBYTE_RNC_UI_TEXT_TEXT_REGION_H
#define LOMBYTE_RNC_UI_TEXT_TEXT_REGION_H

#include "types.h"

/* Halfword layout consumed by the resident text renderers. */
struct TextRegion {
    s16 top;
    s16 bottom;
    s16 left;
    s16 right;
    s16 anchor_x;
    s16 anchor_y;
    s16 measured_width;
    s16 rendered_height;
    s16 line_advance;
    u16 flags;
    s16 subpixel_x_sixteenths;
    s16 subpixel_y_sixteenths;
};

/* Byte alignment preserves the renderer's unaligned 24-byte descriptor copy. */
struct TextRegionBytes {
    u8 data[0x18];
};

enum TextRegionFlags {
    TEXT_REGION_CENTER_HORIZONTALLY = 1,
    TEXT_REGION_CENTER_VERTICALLY = 2,
    TEXT_REGION_MEASURE_ONLY = 4,
    TEXT_REGION_USE_SUBPIXEL_RENDERER = 8
};

#endif
