#include "types.h"
#include "rnc/math/asm_helpers.h"

void expand_map_coverage(u8 *dest, u8 *src) __asm__("FUN_00207e58");

void expand_map_coverage(u8 *dest, u8 *packed_coverage_data) {
    u32  mask[0x100];
    u32  expanded_coverage_words[0x10];
    s32  i = 0;
    u32 *expanded_word_cursor;
    s32  j;
    s32  k;

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
        expanded_word_cursor = expanded_coverage_words;
        for (j = 0; j < 16; j++) {
            *expanded_word_cursor = mask[*packed_coverage_data];
            expanded_word_cursor++;
            packed_coverage_data++;
        }
        fast_mem_copy(dest, expanded_coverage_words, 0x40);
        dest += 0x40;
        fast_mem_copy(dest, expanded_coverage_words, 0x40);
        dest += 0x40;
        fast_mem_copy(dest, expanded_coverage_words, 0x40);
        dest += 0x40;
        fast_mem_copy(dest, expanded_coverage_words, 0x40);
        dest += 0x40;
    }
}

extern __typeof__(expand_map_coverage) func_00207E58 __attribute__((alias("FUN_00207e58")));
