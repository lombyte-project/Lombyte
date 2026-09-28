#include "types.h"

typedef int TI __attribute__((mode(TI)));

union V128 {
    TI q;
    f32 f[4];
};

struct Loc {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    union V128 v;
};

extern void FUN_001f9d50(void *dst, s32 src);
extern void FUN_001f9a28(void *dst, void *a, void *b);
extern void FUN_001f9a68(void *dst, void *a, f32 scale);

void load_display_text_resource_entry(void *arg0, s32 *arg1) __asm__("FUN_00214bc0");

void load_display_text_resource_entry(void *arg0, s32 *arg1) {
    struct Loc loc;
    f32 scale;

    loc.v.q = 0;
    loc.v.f[0] = 127.0f;
    loc.v.f[1] = 127.0f;
    loc.v.f[2] = 127.0f;

    FUN_001f9d50(&loc, *arg1);
    scale = loc.d * 0.0001f;
    FUN_001f9a28(&loc, &loc, &loc.v);
    FUN_001f9a68(arg0, &loc, scale);
}

extern __typeof__(load_display_text_resource_entry) func_00214BC0 __attribute__((alias("FUN_00214bc0")));
