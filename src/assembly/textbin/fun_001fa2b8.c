#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fa2b8/FUN_001fa2b8.s", FUN_001fa2b8);
#else
#include "rnc/math/matrix3x4.h"

void copy_matrix3x4(volatile struct Matrix3x4 *destination,
                    const struct Matrix3x4 *source) __asm__("FUN_001fa2b8");

/* Copy the three 16-byte basis columns; retain all source values before stores. */
void copy_matrix3x4(volatile struct Matrix3x4 *destination, const struct Matrix3x4 *source) {
    u128 first = source->columns[0];
    u128 second = *(const volatile u128 *)&source->columns[1];
    u128 third = *(const volatile u128 *)&source->columns[2];

    destination->columns[0] = first;
    destination->columns[1] = second;
    destination->columns[2] = third;
}

extern __typeof__(copy_matrix3x4) func_001FA2B8 __attribute__((alias("FUN_001fa2b8")));
#endif /* NON_MATCHING */
