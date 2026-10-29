#include "types.h"

#include "rnc/ui/map/map_state.h"

extern void fast_mem_copy(s32, s32, s32) __asm__("func_001F98D0");

void move_map_entry_slot(s32 dst, s32 src) __asm__("FUN_00205000");

void move_map_entry_slot(s32 destination_slot, s32 source_slot) {
    s32 source_slot_id;

    fast_mem_copy(level_map_selection.slot[destination_slot],
                  level_map_selection.slot[source_slot],
                  level_map_selection.slot_size[source_slot] * 0x10);
    source_slot_id = level_map_selection.slot_id[source_slot];
    level_map_selection.slot_id[destination_slot] = source_slot_id;
    level_map_selection.slot_size[destination_slot] = level_map_selection.slot_size[source_slot];
    level_map_selection.slot_id[source_slot] = -1;
}

extern __typeof__(move_map_entry_slot) func_00205000 __attribute__((alias("FUN_00205000")));
