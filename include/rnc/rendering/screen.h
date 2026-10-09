#ifndef LOMBYTE_RNC_RENDERING_SCREEN_H
#define LOMBYTE_RNC_RENDERING_SCREEN_H

#include "types.h"
#include "sda.h"

/*
 * Screen extent at D_0013E500, filled by init_view_context from the display
 * size. left..bottom are GS primitive coordinates (12.4 fixed point, the
 * screen centred on 0x800); the HUD draw calls add left/top to their
 * screen-space x/y (draw_hud_rect, append_screen_rect_packet, ...).
 */
struct Screen {
    s32 width;                /* 0x0: display width (D_00151780 +0x150) */
    s32 height;               /* 0x4: display height (D_00151780 +0x152) */
    s32 half_width;           /* 0x8: width >> 1 */
    s32 half_height;          /* 0xC: height >> 1 */
    s32 left;                 /* 0x10: (0x800 - half_width) << 4 */
    s32 top;                  /* 0x14: (0x800 - half_height) << 4 */
    s32 right;                /* 0x18: (0x800 + half_width) << 4 */
    s32 bottom;               /* 0x1C: (0x800 + half_height) << 4 */
};

/* gcc 2.95 has no _Static_assert: a negative array size fails the build. */
#define SCREEN_OFFSET_CHECK(field, off) \
    typedef char screen_offset_check_##field[ \
        ((unsigned long)&((struct Screen *)0)->field == (off)) ? 1 : -1]
SCREEN_OFFSET_CHECK(left, 0x10);
SCREEN_OFFSET_CHECK(bottom, 0x1C);
#undef SCREEN_OFFSET_CHECK

extern struct Screen screen_extent __asm__("D_0013E500") NOT_SDA;

#endif /* LOMBYTE_RNC_RENDERING_SCREEN_H */
