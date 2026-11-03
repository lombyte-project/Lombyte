#include "types.h"
#include "rnc/game/item_state.h"

s32 count_skill_points(void) __asm__("FUN_00215348");

s32 count_skill_points(void) {
    s32 completed_skill_points = 0;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (item_unlocked[i] != 0) {
            completed_skill_points++;
        }
    }
    if (completed_skill_points < 0) {
        completed_skill_points = 0;
    }
    return (completed_skill_points < 0x1F) ? completed_skill_points : 0x1E;
}

extern __typeof__(count_skill_points) func_00215348 __attribute__((alias("FUN_00215348")));
