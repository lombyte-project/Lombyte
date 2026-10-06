#include "types.h"

#include "rnc/ui/map/map_state.h"
extern void func_001F98D0(s32, s32, s32);

void move_map_entry_slot(s32 dst, s32 src) __asm__("FUN_00205000");

void move_map_entry_slot(s32 dst, s32 src) {
    s32 value;

    func_001F98D0(D_001A00F0.slot[dst], D_001A00F0.slot[src], D_001A00F0.slot_size[src] * 0x10);
    value = D_001A00F0.slot_id[src];
    D_001A00F0.slot_id[dst] = value;
    D_001A00F0.slot_size[dst] = D_001A00F0.slot_size[src];
    D_001A00F0.slot_id[src] = -1;
}

extern __typeof__(move_map_entry_slot) func_00205000 __attribute__((alias("FUN_00205000")));
