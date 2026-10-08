#include "types.h"
#include "rnc/ui/hud/hud_state.h"

extern s32 InitializeResourceEntry();
s32 hud_heap_alloc(s32 size) __asm__("FUN_001ff288");

s32 hud_heap_alloc(s32 size) {
    struct HudState *p = &hud_state;
    s32 cur;

    if (p->heap_cur == 0) {
        InitializeResourceEntry();
    }
    if (p->heap_end - p->heap_cur < size) {
        return 0;
    }
    cur = p->heap_cur;
    size = (size + 15) & 0xFFFFFFF0;
    p->heap_cur = cur + size;
    return cur;
}

extern __typeof__(hud_heap_alloc) func_001ff288 __attribute__((alias("FUN_001ff288")));
