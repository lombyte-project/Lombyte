#include "types.h"

extern s32 D_0019A3E8[];
extern s32 InitializeResourceEntry();
s32 hud_heap_alloc(s32 size) __asm__("FUN_001ff288");

s32 hud_heap_alloc(s32 size) {
    s32 *p = D_0019A3E8;
    s32 cur;

    if (p[4] == 0) {
        InitializeResourceEntry();
    }
    if (p[5] - p[4] < size) {
        return 0;
    }
    cur = p[4];
    size = (size + 15) & 0xFFFFFFF0;
    p[4] = cur + size;
    return cur;
}

extern __typeof__(hud_heap_alloc) func_001ff288 __attribute__((alias("FUN_001ff288")));
