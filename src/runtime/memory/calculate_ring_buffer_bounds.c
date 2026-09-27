#include "types.h"
struct M2c_D_001940C0 {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
};

extern s32 D_00160F0C;
extern struct M2c_D_001940C0 D_001940C0;
s32 calculate_ring_buffer_bounds(u32 arg0, s32 *arg1, s32 *arg2) __asm__("FUN_001fd6e0");

s32 calculate_ring_buffer_bounds(u32 arg0, s32 *arg1, s32 *arg2) {
    if (arg0 > 0x20000U) {
        *arg1 = 0;
        *arg2 = 0;
        return -1;
    }
    *arg1 = (D_001940C0.unk4 + D_00160F0C) - arg0;
    *arg2 = (D_001940C0.unk8 + D_00160F0C) - arg0;
    return 0;
}
extern __typeof__(calculate_ring_buffer_bounds) func_001FD6E0 __attribute__((alias("FUN_001fd6e0")));
