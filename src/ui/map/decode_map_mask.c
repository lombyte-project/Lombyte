#include "types.h"

extern void FUN_00206860(u8 *arg0, s32 arg1, s32 arg2, s16 *arg3);

void decode_map_mask(u8 *arg0, s32 arg1, s16 *arg2) __asm__("FUN_00206710");

void decode_map_mask(u8 *arg0, s32 arg1, s16 *arg2) {
    s32 s;
    s32 i;
    s32 col;
    s32 k;
    s32 acc;
    s32 t;
    u8 *p;
    u8 *src;

    src = (u8 *)0x70000000;
    for (i = 0; i < 0x100; i++) {
        FUN_00206860((u8 *)0x70000000, i, arg1, arg2);
        s = i % 0x10;
        p = arg0 + ((s + ((i / 0x10) * 0x200)) * 4);
        for (col = 0; col < 0x20; col++) {
            acc = 0;
            for (k = 0; k < 0x20; k++) {
                t = (u32)src[col * 0x10 + (k >> 1)];
                if (k & 1) {
                    t = (u32)t >> 4;
                } else {
                    t = (u32)t & 0xF;
                }
                if (t != 0) {
                    acc |= 1 << k;
                }
                if ((k & 0x1F) == 0x1F) {
                    *(s32 *)p = acc;
                    p += 4;
                    acc = 0;
                }
            }
            p += 0x3C;
        }
    }
}

extern __typeof__(decode_map_mask) func_00206710 __attribute__((alias("FUN_00206710")));
