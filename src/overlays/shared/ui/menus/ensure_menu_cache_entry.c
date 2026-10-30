/* Menus: menu cache entries and level table values. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"

/* Stores v in D_L00_001791B8 at i plus this level's base from D_L00_001C44B8. */
void store_current_level_table_value(s32 stored_value, s32 i) __asm__("FUN_L00_0024a798");

void store_current_level_table_value(s32 stored_value, s32 i) {
    if (current_level_index < 0x13) {
        i += D_L00_001C44B8[current_level_index];
        D_L00_001791B8[i] = stored_value;
    }
}

typedef unsigned int MenuCacheQuad __attribute__((mode(TI), aligned(16)));

typedef struct {
    s32 key;
    s32 last_used;
    u8 *pixels;
    s32 unused;
} MenuCacheEntry;

typedef struct {
    u8            *source;
    s16           *offsets;
    u8             pad08[0x20];
    s32            enabled;
    u8             pad2c[4];
    MenuCacheEntry entries[8];
} MenuCache;

extern MenuCache D_L00_001841F0_lui __asm__("D_L00_001841F0");
extern s32       D_L00_0015F3F8;
extern void      FUN_L00_002483c8(u8 *, s32, u8 *, s16 *);

s32 ensure_menu_cache_entry(s32 key) __asm__("FUN_L00_00249d80");

s32 ensure_menu_cache_entry(s32 key) {
    s32            i;
    s32            victim;
    s32            miss;
    s32           *entry;
    s32           *times;
    MenuCacheQuad *base;
    MenuCacheQuad *other;
    MenuCacheQuad  selected_entry_copy;

    if (!D_L00_001841F0_lui.enabled) {
        return 0;
    }
    miss = 0;
    victim = 0;
    i = 0;
    if (D_L00_001841F0_lui.entries[0].key != key) {
        times = &D_L00_001841F0_lui.entries[0].last_used;
        entry = &D_L00_001841F0_lui.entries[0].key;
        /* Fakematch: the outer do-while (0) and the miss++/miss-- below give
         * retail's registers. */
        do {
            do {
                if (entry[1] < times[victim * 4]) {
                    victim = i;
                }
                ++i;
                entry += 4;
            } while (i < 8 && *entry != key);
        } while (0);
    }
    if (i == 8) {
        FUN_L00_002483c8(D_L00_001841F0_lui.entries[victim].pixels,
                         key,
                         D_L00_001841F0_lui.source,
                         D_L00_001841F0_lui.offsets);
        D_L00_001841F0_lui.entries[victim].key = key;
        i = victim;
        miss = 1;
    }
    if (i != 0) {
        base = (MenuCacheQuad *)&D_L00_001841F0_lui.entries[0];
        other = base + i;
        selected_entry_copy = *other;
        /* Fakematch: see the loop above. */
        miss++;
        miss--;
        *other = *base;
        *base = selected_entry_copy;
    }
    D_L00_001841F0_lui.entries[0].last_used = D_L00_0015F3F8;
    return miss;
}
