#include "types.h"
#include "rnc/rendering/render_globals.h"

struct RingBufferConfig {
    u8  pad_0[0x4];
    s32 ring_start; /* 0x04: memory slot 1, start of the VU chain ring */
    s32 ring_end;   /* 0x08: memory slot 2, its end */
};

extern struct RingBufferConfig D_001940C0;
s32 calculate_ring_buffer_bounds(u32 size, s32 *out_start, s32 *out_end) __asm__("FUN_001fd6e0");

s32 calculate_ring_buffer_bounds(u32 byte_count, s32 *out_start, s32 *out_end) {
    if (byte_count > 0x20000U) {
        *out_start = 0;
        *out_end = 0;
        return -1;
    }
    *out_start = (D_001940C0.ring_start + current_vu_chain) - size;
    *out_end = (D_001940C0.ring_end + current_vu_chain) - size;
    return 0;
}

extern __typeof__(calculate_ring_buffer_bounds) func_001FD6E0
    __attribute__((alias("FUN_001fd6e0")));
