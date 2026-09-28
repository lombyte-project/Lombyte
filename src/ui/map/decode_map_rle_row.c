#include "types.h"

void decode_map_rle_row(u8 *buf, s32 row, u8 *data, s16 *offsets) __asm__("FUN_00206860");

void decode_map_rle_row(u8 *buf, s32 row, u8 *data, s16 *offsets) {
    u8 *dst;
    u8 *src;
    s16 *op;
    s32 n;
    s32 i;
    s32 k;
    s32 half;
    u8 *p;
    u32 lo;
    u32 hi;
    s32 count;
    u32 pair;

    dst = buf;
    half = 0;
    if (row != 0) {
        src = data + offsets[row - 1];
    } else {
        src = data + 0x200;
    }
    n = ((data + offsets[row]) - src) * 2 / 3;
    for (i = 0; i < n; i++) {
        k = i * 3;
        p = src + (k >> 1);
        lo = p[0];
        hi = p[1];
        if (k & 1) {
            lo = lo >> 4;
        } else {
            hi = ((hi << 4) | (lo >> 4)) & 0xFF;
            lo = lo & 0xF;
        }
        count = 0x100;
        if (hi != 0) {
            count = hi;
        }
        pair = (lo | (lo << 4)) & 0xFF;
        if (half) {
            *dst = *dst | (lo << 4);
            half = 0;
            count--;
            dst++;
        }
        if (count != 0) {
            do {
                *dst++ = pair;
                count -= 2;
            } while (count > 0);
            if (count != 0) {
                dst--;
                half = 1;
                *dst = lo;
            }
        }
    }
}

extern __typeof__(decode_map_rle_row) func_00206860 __attribute__((alias("FUN_00206860")));
