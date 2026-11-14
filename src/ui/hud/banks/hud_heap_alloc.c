#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/ui/hud/banks/hud_heap_reset.h"

s32 hud_heap_alloc(s32 size) __asm__("FUN_001ff288");

s32 hud_heap_alloc(s32 allocation_size_bytes) {
    struct HudState *hud_state_ptr = &hud_state;
    s32              allocation_address;

    if (hud_state_ptr->heap_cur == 0) {
        hud_heap_reset();
    }
    if (hud_state_ptr->heap_end - hud_state_ptr->heap_cur < allocation_size_bytes) {
        return 0;
    }
    allocation_address = hud_state_ptr->heap_cur;
    allocation_size_bytes = (allocation_size_bytes + 15) & 0xFFFFFFF0;
    hud_state_ptr->heap_cur = allocation_address + allocation_size_bytes;
    return allocation_address;
}

extern __typeof__(hud_heap_alloc) func_001ff288 __attribute__((alias("FUN_001ff288")));
