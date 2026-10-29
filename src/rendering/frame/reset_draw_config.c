#include "types.h"
#include "rnc/rendering/draw_config.h"

void reset_draw_config(void) {
    s32 *entry;
    s32  remaining;
    s32  entry_fill_value;

    entry = (s32 *)&draw_config;
    entry_fill_value = 1;
    remaining = 0x13;
    entry = (s32 *)((u8 *)entry + 0x4C);
    do {
        *entry = entry_fill_value;
        remaining -= 1;
        entry -= 1;
    } while (remaining >= 0);
}

struct DrawConfig draw_config = {0};
