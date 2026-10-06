#include "types.h"
struct RingBufferConfig {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
};

extern s32 D_00160F0C;
extern struct RingBufferConfig D_001940C0;
s32 calculate_ring_buffer_bounds(u32 size, s32 *out_start, s32 *out_end) __asm__("FUN_001fd6e0");

s32 calculate_ring_buffer_bounds(u32 size, s32 *out_start, s32 *out_end) {
    if (size > 0x20000U) {
        *out_start = 0;
        *out_end = 0;
        return -1;
    }
    *out_start = (D_001940C0.unk4 + D_00160F0C) - size;
    *out_end = (D_001940C0.unk8 + D_00160F0C) - size;
    return 0;
}
extern __typeof__(calculate_ring_buffer_bounds) func_001FD6E0
    __attribute__((alias("FUN_001fd6e0")));
