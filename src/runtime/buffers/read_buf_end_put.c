/* Commits N bytes after a put, clamped to the free space. */
#include "types.h"

void read_buf_end_put(s32 *buf, s32 n) __asm__("FUN_0023b990");

void read_buf_end_put(s32 *buf, s32 n) {
    s32 *p = (s32 *)((u8 *)buf + 0x50000);
    s32 space = p[2] - p[1];
    s32 m = (n < space) ? n : space;

    p[0] = (p[0] + m) % p[2];
    p[1] += m;
}

extern __typeof__(read_buf_end_put) func_0023B990 __attribute__((alias("FUN_0023b990")));
/* Recovered original symbol name. */
extern __typeof__(read_buf_end_put) readBufEndPut__FP7ReadBufi
    __attribute__((alias("FUN_0023b990")));
