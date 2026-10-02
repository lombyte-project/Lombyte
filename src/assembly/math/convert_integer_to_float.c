#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/math/convert_integer_to_float/func_001FA6C0.s", func_001FA6C0);
#else
#include "types.h"

f32 convert_integer_to_float(s32 value) __asm__("func_001FA6C0");

f32 convert_integer_to_float(s32 value) {
    return (f32)value;
}
#endif /* NON_MATCHING */
