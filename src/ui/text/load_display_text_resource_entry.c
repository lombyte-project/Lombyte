#include "types.h"
#include "rnc/math/fast_math.h"

typedef int TI __attribute__((mode(TI)));

union V128 {
    TI  q;
    f32 f[4];
};

struct Loc {
    f32        a;
    f32        b;
    f32        c;
    f32        d;
    union V128 v;
};

extern void fast_vector_from_packed_chars(void *dst, s32 src) __asm__("FUN_001f9d50");

void load_display_text_resource_entry(void *destination,
                                      s32  *packed_chars_source) __asm__("FUN_00214bc0");

void load_display_text_resource_entry(void *destination, s32 *packed_chars_source) {
    struct Loc loc;
    f32        scale;

    loc.v.q = 0;
    loc.v.f[0] = 127.0f;
    loc.v.f[1] = 127.0f;
    loc.v.f[2] = 127.0f;

    fast_vector_from_packed_chars(&loc, *packed_chars_source);
    scale = loc.d * 0.0001f;
    fast_vec_sub(&loc, &loc, &loc.v);
    fast_vec_scale(destination, &loc, scale);
}

extern __typeof__(load_display_text_resource_entry) func_00214BC0
    __attribute__((alias("FUN_00214bc0")));
