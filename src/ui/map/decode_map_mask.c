#include "types.h"

extern void decode_map_rle_row(u8 *dst, s32 arg1, s32 arg2, s16 *arg3) __asm__("FUN_00206860");

void decode_map_mask(u8 *dst, s32 arg1, s16 *arg2) __asm__("FUN_00206710");

void decode_map_mask(u8 *dst, s32 compressed_data_base, s16 *row_end_offsets) {
    s32 row_in_group;
    s32 i;
    s32 col;
    s32 k;
    s32 acc;
    s32 nibble_value;
    u8 *mask_word_dst;
    u8 *src;

    src = (u8 *)0x70000000;
    for (i = 0; i < 0x100; i++) {
        decode_map_rle_row((u8 *)0x70000000, i, compressed_data_base, row_end_offsets);
        row_in_group = i % 0x10;
        mask_word_dst = dst + ((row_in_group + ((i / 0x10) * 0x200)) * 4);
        for (col = 0; col < 0x20; col++) {
            acc = 0;
            for (k = 0; k < 0x20; k++) {
                nibble_value = (u32)src[col * 0x10 + (k >> 1)];
                if (k & 1) {
                    nibble_value = (u32)nibble_value >> 4;
                } else {
                    nibble_value = (u32)nibble_value & 0xF;
                }
                if (nibble_value != 0) {
                    acc |= 1 << k;
                }
                if ((k & 0x1F) == 0x1F) {
                    *(s32 *)mask_word_dst = acc;
                    mask_word_dst += 4;
                    acc = 0;
                }
            }
            mask_word_dst += 0x3C;
        }
    }
}

extern __typeof__(decode_map_mask) func_00206710 __attribute__((alias("FUN_00206710")));
