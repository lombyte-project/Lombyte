#include "types.h"

struct M2c_D_001A00F0 {
    u8 pad_0[0x224];
    s32 unk224;
    u8 pad_228[0x50];
    s32 unk278[5];
    s32 unk28C[5];
    s32 unk2A0;
};

extern struct M2c_D_001A00F0 D_001A00F0;
extern s32 SubtractIntegerWithClamp(s32 value);
extern s32 func_00205220(s32 id);

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
    if (D_001A00F0.unk224 == 0) {
        for (j = 4; j >= 0; j--) {
            if (D_001A00F0.unk278[j] != 0 && j != D_001A00F0.unk2A0 && D_001A00F0.unk28C[j] == -1) {
                return j;
            }
        }
    }
    base = func_00205220(D_001A00F0.unk224);
    if (base == -1) {
        return 1;
    }
    for (i = 0; i < 5; i++) {
        if (D_001A00F0.unk278[i] != 0 && i != D_001A00F0.unk2A0) {
            v = D_001A00F0.unk28C[i];
            if (v == -1) {
                return i;
            }
            delta = SubtractIntegerWithClamp(func_00205220(v & 0xFF) - base);
            if (max < delta) {
                max = delta;
                best = i;
            }
        }
    }
    if (best == -1) {
        best = 0;
    }
    D_001A00F0.unk28C[best] = -1;
    return best;
}

extern __typeof__(pick_map_slot_to_evict) func_00205278 __attribute__((alias("FUN_00205278")));
