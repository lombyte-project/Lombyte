#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");

/* Fades from one color to another once the delay has passed. -1 picks the
   default color. The defaults are written back into the parameters: with
   separate locals the extra copies change the register choice. */
s32 fade_menu_color(s32 delay, s32 from, s32 to) __asm__("FUN_0021b6d8");

s32 fade_menu_color(s32 delay, s32 from, s32 to) {
    s32 now;
    f32 tween_factor;

    if (delay < 0) {
        delay = 0;
    }
    if (from == -1) {
        from = 0x80FFA888;
    }
    if (to == -1) {
        to = 0x8020FFFF;
    }
    if (scale_game_frames(menu_fade_duration) >= delay) {
        now = scale_game_frames(menu_fade_duration);
        tween_factor = 1.0f - (f32)(now - delay) / (f32)scale_game_frames(menu_fade_duration);
    } else {
        tween_factor = 1.0f;
    }
    return fast_tween_color(from, to, tween_factor);
}

extern __typeof__(fade_menu_color) func_0021B6D8 __attribute__((alias("FUN_0021b6d8")));
