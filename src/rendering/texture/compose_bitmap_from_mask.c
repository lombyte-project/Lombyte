#include "types.h"
void compose_bitmap_from_mask(u8 *dst, u8 *src_set, u8 *src_clear, u8 *mask) __asm__("FUN_002053d8");

void compose_bitmap_from_mask(u8 *dst, u8 *src_set, u8 *src_clear, u8 *mask) {
    s32 outer = 0;
    s32 limit = 0x7FFF;
    do {
        s32 bit = 1;
        u8 *next = mask + 1;
        s32 count = 7;
        do {
            u8 value;
            if (*mask & bit)
                value = *src_set;
            else
                value = *src_clear;
            *dst = value;
            src_set += 1;
            src_clear += 1;
            dst += 1;
            count -= 1;
            bit <<= 1;
        } while (count >= 0);
        outer += 1;
        mask = next;
    } while (outer <= limit);
}
