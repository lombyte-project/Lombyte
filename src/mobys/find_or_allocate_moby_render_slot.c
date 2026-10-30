#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

s32 find_or_allocate_moby_render_slot(s32 id) __asm__("FUN_0020cc18");

s32 find_or_allocate_moby_render_slot(s32 id) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (moby_render_slots[i] == 0 || moby_render_slots[i] == id) {
            moby_render_slots[i] = id;
            return i;
        }
    }
    return -1;
}

/* The render slot ids this allocator hands out (prune_moby_references
   frees them). */
s32 moby_render_slots[16] __asm__("D_001B2BC0") = {0};

extern __typeof__(find_or_allocate_moby_render_slot) func_0020CC18
    __attribute__((alias("FUN_0020cc18")));
