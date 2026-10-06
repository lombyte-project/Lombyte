#include "types.h"

#include "rnc/ui/map/map_state.h"
extern void func_001F98D0(s32, s32, s32);

void move_map_entry_slot(s32 arg0, s32 arg1) __asm__("FUN_00205000");

void move_map_entry_slot(s32 arg0, s32 arg1)
{
    s32 value;

    func_001F98D0(D_001A00F0.slot[arg0], D_001A00F0.slot[arg1], D_001A00F0.slot_size[arg1] * 0x10);
    value = D_001A00F0.slot_id[arg1];
    D_001A00F0.slot_id[arg0] = value;
    D_001A00F0.slot_size[arg0] = D_001A00F0.slot_size[arg1];
    D_001A00F0.slot_id[arg1] = -1;
}

extern __typeof__(move_map_entry_slot) func_00205000 __attribute__((alias("FUN_00205000")));
