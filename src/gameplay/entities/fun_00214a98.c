#include "types.h"

extern f32 func_001F99C0(f32);
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 FUN_001f9d68(void *);

void FUN_00214a98(f32 *v, s32 *out) {
    f32 buf[4];
    f32 a;
    f32 b;
    f32 c;
    f32 m;
    s32 n;
    f32 s;

    a = func_001F99C0(v[0]);
    b = func_001F99C0(v[1]);
    c = func_001F99C0(v[2]);
    if (b < a) {
        b = a;
    }
    m = (c < b) ? b : c;
    n = truncate_float_to_s32(m * 10000.0f / 63.0f);
    n = (n < 0x100) ? n : 0xFF;
    if (n <= 0) {
        n = 1;
    }
    buf[3] = (f32)n;
    s = 1.0f / (buf[3] * 0.0001f);
    buf[0] = v[0] * s + 127.0f;
    buf[1] = v[1] * s + 127.0f;
    buf[2] = v[2] * s + 127.0f;
    *out = FUN_001f9d68(buf);
}

extern __typeof__(FUN_00214a98) func_00214A98 __attribute__((alias("FUN_00214a98")));
