#include "types.h"
#include "rnc/ui/hud/hud_state.h"

extern u32 hud_heap_base[4] __asm__("D_001940CC");

void hud_heap_reset(void) __asm__("InitializeResourceEntry");

void hud_heap_reset(void) {
    u32              base;
    struct HudState *entry;

    base = hud_heap_base[0];
    entry = &hud_state;
    entry->heap_cur = base;
    entry->heap_end = base + 0x64000;
}

/* Recovered original symbol name. */
extern __typeof__(hud_heap_reset) Hud_HeapReset__Fv
    __attribute__((alias("InitializeResourceEntry")));
