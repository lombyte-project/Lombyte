#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fa2b8/FUN_001fa2b8.s", FUN_001fa2b8);
#else
#include "rnc/fun_001fa2b8_types.h"

void copy_matrix3x4(volatile struct Matrix3x4 *destination,
                   const volatile struct Matrix3x4 *source)
    __asm__("FUN_001fa2b8");

/* Copy all three matrix columns. */
void copy_matrix3x4(volatile struct Matrix3x4 *destination,
                   const volatile struct Matrix3x4 *source) {
    u128 first;
    u128 second;
    u128 third;

    /* Read every source column before writing any destination column. */
    first = source->columns[0];
    second = source->columns[1];
    third = source->columns[2];

    destination->columns[0] = first;
    destination->columns[1] = second;
    destination->columns[2] = third;
}

extern __typeof__(copy_matrix3x4) func_001FA2B8
    __attribute__((alias("FUN_001fa2b8")));
#endif /* NON_MATCHING */
