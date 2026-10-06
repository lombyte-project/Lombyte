#include "types.h"

s32 read_buf_begin_get(s32 *arg0, s32 *arg1) __asm__("FUN_0023b9d8");

s32 read_buf_begin_get(s32 *arg0, s32 *arg1) {
    s32 *p = (s32 *)((u8 *)arg0 + 0x50000);
    s32 q;

    if (p[1] != 0) {
        q = (p[0] - p[1] + p[2]) % p[2];
        *arg1 = (s32)arg0 + q;
    }
    return p[1];
}

extern s32 func_0023B9D8(s32 *arg0, s32 *arg1) __attribute__((alias("FUN_0023b9d8")));
extern s32 readBufBeginGet__FP7ReadBufPPUc(s32 *arg0, s32 *arg1)
    __attribute__((alias("FUN_0023b9d8")));
