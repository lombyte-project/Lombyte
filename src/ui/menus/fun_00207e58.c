#include "types.h"

extern void FUN_001f98d0(u8 *, void *, s32);

void FUN_00207e58(u8 *dest, u8 *src) {
    u32 mask[0x100];
    u32 buf[0x10];
    s32 i = 0;
    u32 *q;
    s32 j;
    s32 k;

    for (; i < 0x100; i++) {
        mask[i] = 0;
        if (i & 0x1) {
            mask[i] = 0xF;
        }
        if (i & 0x2) {
            mask[i] |= 0xF0;
        }
        if (i & 0x4) {
            mask[i] |= 0xF00;
        }
        if (i & 0x8) {
            mask[i] |= 0xF000;
        }
        if (i & 0x10) {
            mask[i] |= 0xF0000;
        }
        if (i & 0x20) {
            mask[i] |= 0xF00000;
        }
        if (i & 0x40) {
            mask[i] |= 0xF000000;
        }
        if (i & 0x80) {
            mask[i] |= 0xF0000000;
        }
    }
    for (k = 0; k < 0x80; k++) {
        q = buf;
        for (j = 0; j < 16; j++) {
            *q = mask[*src];
            q++;
            src++;
        }
        FUN_001f98d0(dest, buf, 0x40);
        dest += 0x40;
        FUN_001f98d0(dest, buf, 0x40);
        dest += 0x40;
        FUN_001f98d0(dest, buf, 0x40);
        dest += 0x40;
        FUN_001f98d0(dest, buf, 0x40);
        dest += 0x40;
    }
}

extern __typeof__(FUN_00207e58) func_00207E58 __attribute__((alias("FUN_00207e58")));
