#include "types.h"

typedef union {
    f64 value;
    struct {
        u32 lsw;
        u32 msw;
    } parts;
} ieee_double_shape_type;

/* fdlibm math_private.h */
#define EXTRACT_WORDS(ix0, ix1, d) \
    do { \
        ieee_double_shape_type ew_u; \
        ew_u.value = (d); \
        (ix0) = ew_u.parts.msw; \
        (ix1) = ew_u.parts.lsw; \
    } while (0)

s32 double_is_not_nan(f64 x) __asm__("FUN_00116168");

s32 double_is_not_nan(f64 x) {
    s32 hx;
    s32 lx;

    EXTRACT_WORDS(hx, lx, x);
    hx &= 0x7fffffff;
    hx |= (u32)(lx | (-lx)) >> 31;
    hx = 0x7ff00000 - hx;
    return 1 - (s32)(((u32)(hx | (-hx))) >> 31);
}

extern __typeof__(double_is_not_nan) func_00116168 __attribute__((alias("FUN_00116168")));
