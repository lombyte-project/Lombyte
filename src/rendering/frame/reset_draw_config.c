#include "types.h"
#include "rnc/rendering/draw_config.h"

void reset_draw_config(void) {
    s32 *draw_config_fill_cursor;
    s32  remaining;
    s32  entry_fill_value;

    draw_config_fill_cursor = (s32 *)&draw_config;
    entry_fill_value = 1;
    remaining = 0x13;
    draw_config_fill_cursor = (s32 *)((u8 *)draw_config_fill_cursor + 0x4C);
    do {
        *draw_config_fill_cursor = entry_fill_value;
        remaining -= 1;
        draw_config_fill_cursor -= 1;
    } while (remaining >= 0);
}

struct DrawConfig draw_config = {0};
