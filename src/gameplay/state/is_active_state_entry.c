#include "types.h"
#include "rnc/data_labels.h"
struct StateEntry {
    u8 pad_0[0x74];
    u8 unk74;
    u8 pad_75[0x9];
    s16 unk7E;
};

extern u8 D_0013E550[];

s32 is_active_state_entry(s32 entry_index, s32 id) __asm__("FUN_001eb740");

s32 is_active_state_entry(s32 entry_index, s32 id) {
    struct StateEntry *state_entry;

    if (entry_index >= 0) {
        state_entry = (struct StateEntry *)(entry_index * 0x70 + D_0013E550);
        if (state_entry->unk7E == id + D_0015F5B4) {
            if ((u32)(state_entry->unk74 - 1) < 2) {
                return 1;
            }
        }
    }
    return 0;
}

extern __typeof__(is_active_state_entry) func_001EB740 __attribute__((alias("FUN_001eb740")));
