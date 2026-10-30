#include "types.h"

struct Table16 {
    s32 v[16];
};

extern u8            *D_001A00FC[];
extern struct Table16 map_coverage_nibble_contribution_table __asm__("D_001E8080");
extern void           fill_transfer_words(void *dst, s32 value, s32 size) __asm__("func_001F97E8");

/* Builds a 1bpp mask from the 4bpp map at D_001A00FC[0]: each nibble is
   weighted through the 16-entry table D_001E8080, four source rows are summed
   per column, and a column whose sum reaches 8 sets its bit in the output. */
void downsample_map_coverage(u8 *out) __asm__("FUN_00208030");

void downsample_map_coverage(u8 *coverage_mask) {
    s32            acc[128];
    struct Table16 nibble_contributions;
    u8            *packed_map_cursor;
    u8            *coverage_mask_cursor;
    s32            i;
    s32            j;
    s32            k;
    s32            packed_column_start;
    u32            packed_source_byte;

    coverage_mask_cursor = coverage_mask;
    packed_map_cursor = D_001A00FC[0];
    nibble_contributions = map_coverage_nibble_contribution_table;
    for (i = 0; i < 0x200; i++) {
        if ((i & 3) == 0) {
            fill_transfer_words(acc, 0, 0x200);
        }
        for (j = 0; j < 128; j += 2) {
            packed_source_byte = *packed_map_cursor++;
            acc[j] += nibble_contributions.v[packed_source_byte & 0xF];
            acc[j + 1] += nibble_contributions.v[packed_source_byte >> 4];
        }
        if (i % 4 == 3) {
            for (k = 0; k < 128; k++) {
                if (acc[k] < 8) {
                    acc[k] = 0;
                } else {
                    acc[k] = 1 << (k % 8);
                }
            }
            for (packed_column_start = 0; packed_column_start < 128; packed_column_start += 8) {
                *coverage_mask_cursor++ =
                    acc[packed_column_start] | acc[packed_column_start + 1] |
                    acc[packed_column_start + 2] | acc[packed_column_start + 3] |
                    acc[packed_column_start + 4] | acc[packed_column_start + 5] |
                    acc[packed_column_start + 6] | acc[packed_column_start + 7];
            }
        }
    }
    coverage_mask[0] = (coverage_mask[0] & 0xFE) | 2;
}

extern __typeof__(downsample_map_coverage) func_00208030 __attribute__((alias("FUN_00208030")));
