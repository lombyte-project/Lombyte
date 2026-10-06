#include "types.h"

extern u32 D_00141EA0[];

int pause_stage_sprite_lists(u32 *mode) __asm__("FUN_0021ca60");

int pause_stage_sprite_lists(u32 *mode) {
    u32 *src = mode + 12;
    u32 *dst = D_00141EA0;
    int count = 7;

    do {
        *dst = *src;
        src++;
        dst++;
        count--;
    } while (count >= 0);
    return 0;
}

extern __typeof__(pause_stage_sprite_lists) func_0021CA60 __attribute__((alias("FUN_0021ca60")));
/* Recovered original symbol name. */
extern __typeof__(pause_stage_sprite_lists) pause_stageSpriteLists__FP19PauseSpriteListMode
    __attribute__((alias("FUN_0021ca60")));
