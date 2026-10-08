#include "types.h"

#include "rnc/ui/map/map_state.h"
extern s32 SubtractIntegerWithClamp(s32 value);
extern s32 find_id_in_terminated_table(s32 id) __asm__("func_00205220");

s32 pick_map_slot_to_evict(s32 arg0, s32 arg1) __asm__("FUN_00205278");

s32 pick_map_slot_to_evict(s32 arg0, s32 arg1) {
    s64 max;
    s32 base;
    s64 delta;
    s32 best;
    s32 i;
    s32 j;
    s32 v;

    max = 0;
    best = -1;
    if (D_001A00F0.level == 0) {
        for (j = 4; j >= 0; j--) {
            if (D_001A00F0.slot[j] != 0 && j != D_001A00F0.sel && D_001A00F0.slot_id[j] == -1) {
                return j;
            }
        }
    }
    base = find_id_in_terminated_table(D_001A00F0.level);
    if (base == -1) {
        return 1;
    }
    for (i = 0; i < 5; i++) {
        if (D_001A00F0.slot[i] != 0 && i != D_001A00F0.sel) {
            v = D_001A00F0.slot_id[i];
            if (v == -1) {
                return i;
            }
            delta = SubtractIntegerWithClamp(find_id_in_terminated_table(v & 0xFF) - base);
            if (max < delta) {
                max = delta;
                best = i;
            }
        }
    }
    if (best == -1) {
        best = 0;
    }
    D_001A00F0.slot_id[best] = -1;
    return best;
}

extern __typeof__(pick_map_slot_to_evict) func_00205278 __attribute__((alias("FUN_00205278")));
