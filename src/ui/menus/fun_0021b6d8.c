#include "types.h"
extern s32 D_001601B4 __attribute__((sda));
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 func_001FA6E0(s32, s32, f32);

/* Fades from one color to another once the delay has passed. -1 picks the
   default color. The defaults are written back into the parameters: with
   separate locals the extra copies change the register choice. */
s32 FUN_0021b6d8(s32 delay, s32 from, s32 to) {
    s32 now;
    f32 t;

    if (delay < 0) {
        delay = 0;
    }
    if (from == -1) {
        from = 0x80FFA888;
    }
    if (to == -1) {
        to = 0x8020FFFF;
    }
    if (scale_game_frames(D_001601B4) >= delay) {
        now = scale_game_frames(D_001601B4);
        t = 1.0f - (f32)(now - delay) / (f32)scale_game_frames(D_001601B4);
    } else {
        t = 1.0f;
    }
    return func_001FA6E0(from, to, t);
}

extern __typeof__(FUN_0021b6d8) func_0021B6D8 __attribute__((alias("FUN_0021b6d8")));
