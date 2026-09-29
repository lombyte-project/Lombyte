#include "types.h"

/* fdlibm s_isnan.c: EXTRACT_WORDS() as a do { ... } while (0) macro body */
typedef union {
    f64 value;
    struct {
        u32 lsw;
        u32 msw;
    } parts;
} ieee_double_shape_type;

s32 ClassifyDoubleNaN(f64 x) {
    s32 hx;
    s32 lx;
    ieee_double_shape_type ew_u;

    do {
        ew_u.value = x;
        hx = ew_u.parts.msw;
        lx = ew_u.parts.lsw;
    } while (0);
    hx &= 0x7fffffff;
    hx |= (u32)(lx | (-lx)) >> 31;
    hx = 0x7ff00000 - hx;
    return (u32)hx >> 31;
}
