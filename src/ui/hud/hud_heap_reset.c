#include "types.h"
#include "rnc/ui/hud/hud_state.h"

extern u32 D_001940CC[4] __asm__("D_001940CC");

void InitializeResourceEntry(void) __asm__("InitializeResourceEntry");

void InitializeResourceEntry(void) {
    u32 base;
    struct HudState *entry;

    base = D_001940CC[0];
    entry = &hud_state;
    entry->heap_cur = base;
    entry->heap_end = base + 0x64000;
}

/* Recovered original symbol name. */
extern __typeof__(InitializeResourceEntry) Hud_HeapReset__Fv
    __attribute__((alias("InitializeResourceEntry")));
