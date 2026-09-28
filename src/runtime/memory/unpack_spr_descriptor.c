/* Ported from rac1-decomp, the PAL decompilation (src/core/001236F0.c, func_00123BA0). */
/*
 * Unpack a scratchpad-resident descriptor: arg0 is forced into the SPR
 * window (| 0x20000000), then two counted byte runs are copied out of
 * it -- the one at +0x10 to the pointer at +0x8 for the count at +0x0,
 * and the one at +0x50 to the pointer at +0xC for the count at +0x4.
 * The counts are re-read from the descriptor on every iteration, so
 * they are written out in full rather than cached.
 *
 * The explicit zero-trip guard around each do/while is what makes this
 * match, and it is worth knowing why. Written as a plain `for`, the
 * source base (p + 0x10) is loop-invariant, so GCC hoists it ABOVE the
 * guard; retail computes it BELOW, between the guard and the loop head.
 * That one position shifts the loop head by a word, which in turn costs
 * the alignment nop retail carries -- two instructions per loop, 16
 * bytes over the two loops. Spelling the guard yourself puts the
 * invariant where retail has it. (Folding the +0x10 into the load
 * offset instead of keeping a base pointer costs another 8.)
 */
void unpack_spr_descriptor(void *arg0) __asm__("FUN_001238a0");

void unpack_spr_descriptor(void *arg0) {
    char *p = (char *)((unsigned int)arg0 | 0x20000000);
    unsigned char *src;
    char *d;
    int i;

    if (*(int *)(p + 0x0) != 0) {
        d = *(char **)(p + 0x8);
        i = 0;
        if (i < *(int *)(p + 0x0)) {
            src = (unsigned char *)(p + 0x10);
            do {
                *d++ = src[i];
                i++;
            } while (i < *(int *)(p + 0x0));
        }
    }
    if (*(int *)(p + 0x4) != 0) {
        d = *(char **)(p + 0xC);
        i = 0;
        if (i < *(int *)(p + 0x4)) {
            src = (unsigned char *)(p + 0x50);
            do {
                *d++ = src[i];
                i++;
            } while (i < *(int *)(p + 0x4));
        }
    }
}

extern __typeof__(unpack_spr_descriptor) func_001238A0 __attribute__((alias("FUN_001238a0")));
