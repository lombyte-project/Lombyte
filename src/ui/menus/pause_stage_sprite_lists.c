#include "types.h"

extern s32 pause_iop_sprite_lists[] __asm__("D_00141EA0");

int pause_stage_sprite_lists(u32 *mode) __asm__("FUN_0021ca60");

int pause_stage_sprite_lists(u32 *mode) {
    u32 *sprite_list_source = mode + 12;
    u32 *iop_sprite_list_destination = pause_iop_sprite_lists;
    int count = 7;

    do {
        *iop_sprite_list_destination = *sprite_list_source;
        sprite_list_source++;
        iop_sprite_list_destination++;
        count--;
    } while (count >= 0);
    return 0;
}

extern __typeof__(pause_stage_sprite_lists) func_0021CA60 __attribute__((alias("FUN_0021ca60")));
/* Recovered original symbol name. */
extern __typeof__(pause_stage_sprite_lists) pause_stageSpriteLists__FP19PauseSpriteListMode
    __attribute__((alias("FUN_0021ca60")));
