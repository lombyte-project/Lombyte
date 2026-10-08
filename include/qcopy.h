#ifndef LOMBYTE_QCOPY_H
#define LOMBYTE_QCOPY_H

/*
 * Copies one 16-byte quadword from src to dst through $2, the way the
 * original game source did: an inline-asm copy shaped like libvu0's
 * sceVu0CopyVector (which uses $6). All 154 adjacent lq/sq copies in the
 * retail game code go through $2 with each address in its own register at
 * offset 0, which no C copy reproduces: a TImode or aligned-struct copy folds
 * the offset into lq/sq, and without the "memory" clobber the compiler also
 * moves other accesses across the copy.
 *
 * This is the project's one inline-asm idiom, because the original had it.
 * Prefer a plain TImode copy (u128) wherever that reproduces retail.
 * Identified by rac1-decomp (include/common.h).
 */
static __inline__ void qcopy(void *dst, void *src) {
    __asm__ __volatile__("lq $2,0x0(%1)\n\tsq $2,0x0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

/* Same copy without the "memory" clobber, for the few spots where retail keeps a value read after the copy in a register instead of reloading it. */
static __inline__ void qcopy_nc(void *dst, void *src) {
    __asm__ __volatile__("lq $2,0x0(%1)\n\tsq $2,0x0(%0)" : : "r"(dst), "r"(src) : "$2");
}

#endif /* LOMBYTE_QCOPY_H */
