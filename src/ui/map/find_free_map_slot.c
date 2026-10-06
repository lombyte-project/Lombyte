#include "types.h"
struct Slots {
    s32 used[5];
    s32 owner[5];
};
extern struct Slots D_001A0368;
s32 find_free_map_slot(s32 reverse) __asm__("FUN_00204ef8");

s32 find_free_map_slot(s32 reverse) {
    struct Slots *s = &D_001A0368;
    s32 *used;
    s32 *owner;
    s32 i;
    s32 j;

    used = s->used;
    owner = s->owner;
    for (i = 0; i < 5; i++) {
        j = reverse ? 4 - i : i;
        if (used[j] != 0 && owner[j] == -1) {
            return j;
        }
    }
    return -1;
}

extern __typeof__(find_free_map_slot) func_00204EF8 __attribute__((alias("FUN_00204ef8")));
