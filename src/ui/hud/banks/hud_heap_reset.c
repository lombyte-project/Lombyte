#include "types.h"
#include "rnc/ui/hud/hud_state.h"

extern u32 hud_heap_base[4] __asm__("D_001940CC");

void hud_heap_reset(void) __asm__("InitializeResourceEntry");

void hud_heap_reset(void) {
    u32              heap_start_address;
    struct HudState *hud_state_ptr;

    heap_start_address = hud_heap_base[0];
    hud_state_ptr = &hud_state;
    hud_state_ptr->heap_cur = heap_start_address;
    hud_state_ptr->heap_end = heap_start_address + 0x64000;
}

/* Recovered original symbol name. */
extern __typeof__(hud_heap_reset) Hud_HeapReset__Fv
    __attribute__((alias("InitializeResourceEntry")));
