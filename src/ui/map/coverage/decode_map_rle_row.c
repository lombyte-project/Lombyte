#include "types.h"

void decode_map_rle_row(u8 *buf, s32 row, u8 *data, s16 *offsets) __asm__("FUN_00206860");

void decode_map_rle_row(u8 *decoded_row_dst, s32 row, u8 *data, s16 *offsets) {
    u8  *decoded_row_cursor;
    u8  *compressed_row_start;
    s16 *op;
    s32  run_count;
    s32  i;
    s32  k;
    s32  half;
    u8  *packed_run_src;
    u32  lo;
    u32  hi;
    s32  count;
    u32  pair;

    decoded_row_cursor = decoded_row_dst;
    half = 0;
    if (row != 0) {
        compressed_row_start = data + offsets[row - 1];
    } else {
        compressed_row_start = data + 0x200;
    }
    run_count = ((data + offsets[row]) - compressed_row_start) * 2 / 3;
    for (i = 0; i < run_count; i++) {
        k = i * 3;
        packed_run_src = compressed_row_start + (k >> 1);
        lo = packed_run_src[0];
        hi = packed_run_src[1];
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
            *decoded_row_cursor = *decoded_row_cursor | (lo << 4);
            half = 0;
            count--;
            decoded_row_cursor++;
        }
        if (count != 0) {
            do {
                *decoded_row_cursor++ = pair;
                count -= 2;
            } while (count > 0);
            if (count != 0) {
                decoded_row_cursor--;
                half = 1;
                *decoded_row_cursor = lo;
            }
        }
    }
}

extern __typeof__(decode_map_rle_row) func_00206860 __attribute__((alias("FUN_00206860")));
