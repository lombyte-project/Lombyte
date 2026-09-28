#include "types.h"

void vo_buf_create(void *buffer, s32 a1, s32 a2, s32 count) __asm__("FUN_0023d190");
void vo_buf_create(void *buffer, s32 a1, s32 a2, s32 count) {
    s32 offset;
    volatile s32 *p;

    p = (volatile s32 *)buffer;
    p[3] = 0;
    p[0] = a1;
    p[1] = a2;
    p[4] = count;
    p[2] = 0;
    if (count > 0) {
        offset = 0;
        do {
            *(s32 *)(offset + (u32)p[1]) = 0;
            count--;
            offset += 0x138C0;
        } while (count != 0);
    }
}

extern __typeof__(vo_buf_create) func_0023D190 __attribute__((alias("FUN_0023d190")));
