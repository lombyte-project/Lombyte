#include "types.h"

/* Ported from rac1-decomp (src/core/001236F0.c, func_00123BA0). */
/* Two counted byte runs, each copied out to its own destination. */
struct SprDescriptor {
    s32 first_count;
    s32 second_count;
    u8 *first_destination;  /* 0x08 */
    u8 *second_destination; /* 0x0C */
    u8  first_bytes[0x40];  /* 0x10 */
    u8  second_bytes[1];    /* 0x50: second_count bytes */
};

/*
 * The counts are re-read from the descriptor on every iteration, so
 * they are written out in full rather than cached.
 *
 * The explicit zero-trip guard around each do/while is what makes this
 * match, and it is worth knowing why. Written as a plain `for`, the
 * source base (first_bytes) is loop-invariant, so GCC hoists it ABOVE the
 * guard; retail computes it BELOW, between the guard and the loop head.
 * That one position shifts the loop head by a word, which in turn costs
 * the alignment nop retail carries -- two instructions per loop, 16
 * bytes over the two loops. Spelling the guard yourself puts the
 * invariant where retail has it. (Folding the +0x10 into the load
 * offset instead of keeping a base pointer costs another 8.)
 */
void unpack_spr_descriptor(void *descriptor) __asm__("FUN_001238a0");

void unpack_spr_descriptor(void *descriptor) {
    struct SprDescriptor *scratchpad_descriptor =
        (struct SprDescriptor *)((unsigned int)descriptor | 0x20000000);
    unsigned char *inline_source_bytes;
    char          *destination;
    int            i;

    if (scratchpad_descriptor->first_count != 0) {
        destination = (char *)scratchpad_descriptor->first_destination;
        i = 0;
        if (i < scratchpad_descriptor->first_count) {
            inline_source_bytes = scratchpad_descriptor->first_bytes;
            do {
                *destination++ = inline_source_bytes[i];
                i++;
            } while (i < scratchpad_descriptor->first_count);
        }
    }
    if (scratchpad_descriptor->second_count != 0) {
        destination = (char *)scratchpad_descriptor->second_destination;
        i = 0;
        if (i < scratchpad_descriptor->second_count) {
            inline_source_bytes = scratchpad_descriptor->second_bytes;
            do {
                *destination++ = inline_source_bytes[i];
                i++;
            } while (i < scratchpad_descriptor->second_count);
        }
    }
}

extern __typeof__(unpack_spr_descriptor) func_001238A0 __attribute__((alias("FUN_001238a0")));
