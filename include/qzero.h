#ifndef QZERO_H
#define QZERO_H

/*
 * Clears one 16-byte quadword with `sq $0`. GCC 2.9's `sq` template takes a
 * register, so C zeroing a u128 always emits `por $2,$0,$0` + `sq $2`; retail
 * has the `sq $0` form in code the compiler otherwise reproduces exactly
 * (see fun_00218d10), so the original source had it as inline asm, the way
 * qcopy() has its lq/sq copy. No memory or register clobber: with one, the
 * compiler reloads the base address and the unit is one word off.
 *
 * Use it only where the retail bytes have `sq $0` in compiler-shaped code;
 * routines marked "Handwritten function" stay intentional asm.
 */
#define qzero(p) __asm__ __volatile__("sq $0,0x0(%0)" : : "r"(p))

#endif /* QZERO_H */
