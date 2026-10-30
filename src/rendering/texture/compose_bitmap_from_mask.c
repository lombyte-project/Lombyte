#include "types.h"

void compose_bitmap_from_mask(u8 *dst,
                              u8 *src_set,
                              u8 *src_clear,
                              u8 *mask) __asm__("FUN_002053d8");

void compose_bitmap_from_mask(u8 *composed_bitmap, u8 *src_set, u8 *src_clear, u8 *mask) {
    s32 outer = 0;
    s32 limit = 0x7FFF;
    do {
        s32 bit = 1;
        u8 *next_mask_byte = mask + 1;
        s32 count = 7;
        do {
            u8 selected_source_byte;
            if (*mask & bit)
                selected_source_byte = *src_set;
            else
                selected_source_byte = *src_clear;
            *composed_bitmap = selected_source_byte;
            src_set += 1;
            src_clear += 1;
            composed_bitmap += 1;
            count -= 1;
            bit <<= 1;
        } while (count >= 0);
        outer += 1;
        mask = next_mask_byte;
    } while (outer <= limit);
}
