#include "types.h"

extern f32 func_001F9B48(s32, u32);
extern s32 FUN_0022c6f8(f32 *, f32, f32, f32);
extern u8 D_00187080[];

void FUN_0022c7e8(s32 *arg0, s32 arg1) {
    f32 v = func_001F9B48(arg1, (u32)D_00187080);
    f32 *q = (f32 *)arg0[2];

    FUN_0022c6f8(q, v, q[0], q[1]);
}
