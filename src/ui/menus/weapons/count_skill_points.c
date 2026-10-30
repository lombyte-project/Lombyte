#include "types.h"
#include "rnc/game/item_state.h"

s32 count_skill_points(void) __asm__("FUN_00215348");

s32 count_skill_points(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (item_unlocked[i] != 0) {
            count++;
        }
    }
    if (count < 0) {
        count = 0;
    }
    return (count < 0x1F) ? count : 0x1E;
}

extern __typeof__(count_skill_points) func_00215348 __attribute__((alias("FUN_00215348")));
