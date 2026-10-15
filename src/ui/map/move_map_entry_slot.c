#include "types.h"

#include "rnc/ui/map/map_state.h"
extern void fast_mem_copy(s32, s32, s32) __asm__("func_001F98D0");

void move_map_entry_slot(s32 dst, s32 src) __asm__("FUN_00205000");

void move_map_entry_slot(s32 dst, s32 src) {
    s32 value;

    fast_mem_copy(level_map_selection.slot[dst], level_map_selection.slot[src], level_map_selection.slot_size[src] * 0x10);
    value = level_map_selection.slot_id[src];
    level_map_selection.slot_id[dst] = value;
    level_map_selection.slot_size[dst] = level_map_selection.slot_size[src];
    level_map_selection.slot_id[src] = -1;
}

extern __typeof__(move_map_entry_slot) func_00205000 __attribute__((alias("FUN_00205000")));
