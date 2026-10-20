#include "types.h"

s32 read_buf_begin_get(s32 *read_buf, s32 *out_ptr) __asm__("FUN_0023b9d8");

s32 read_buf_begin_get(s32 *read_buf, s32 *out_ptr) {
    s32 *buffer_state = (s32 *)((u8 *)read_buf + 0x50000);
    s32 read_offset;

    if (buffer_state[1] != 0) {
        read_offset = (buffer_state[0] - buffer_state[1] + buffer_state[2]) % buffer_state[2];
        *out_ptr = (s32)read_buf + read_offset;
    }
    return buffer_state[1];
}

extern s32 func_0023B9D8(s32 *read_buf, s32 *out_ptr) __attribute__((alias("FUN_0023b9d8")));
extern s32 readBufBeginGet__FP7ReadBufPPUc(s32 *read_buf, s32 *out_ptr)
    __attribute__((alias("FUN_0023b9d8")));
