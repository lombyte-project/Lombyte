#include "types.h"
#include "eetypes.h"
typedef union { u128 q; f32 f[4]; } Vec4;
extern s32 D_001A03A8[];
extern f32 FUN_001f9b80(Vec4 *, Vec4 *);
s32 FUN_00207a18(s32 x, s32 y) {
    Vec4 a;
    Vec4 b;
    s32 near;

    if (D_001A03A8[0] == 0) {
        a.q = 0;
        b.q = 0;
        a.f[0] = x;
        a.f[1] = y;
        b.f[0] = 161.5f;
        b.f[1] = 249.0f;
        near = FUN_001f9b80(&a, &b) <= 35.0f;
    } else {
        near = 1;
    }
    return near;
}

extern __typeof__(FUN_00207a18) func_00207A18 __attribute__((alias("FUN_00207a18")));
