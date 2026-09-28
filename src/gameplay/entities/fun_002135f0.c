#include "types.h"

struct M2c_vec3 {
    f32 x;
    f32 y;
    f32 z;
};

extern s32 D_00161198;
extern s32 D_0016119C;
extern s32 D_001611A0;
extern f32 D_001611AC;
extern f32 func_001F99C0(f32);
extern void FUN_001f9a00(s32);
extern f32 FUN_001f9b80(struct M2c_vec3 *, s32 *);
extern s32 func_00239F58(f32 *, f32, f32, f32);

f32 FUN_002135f0(struct M2c_vec3 *arg0, s32 arg1) {
    f32 sp0;
    if (D_00161198 != 0) {
        sp0 = arg0->z;
        if (func_00239F58(&sp0, arg0->x, arg0->y, sp0) != 0) {
            return sp0;
        }
    }
    if (D_0016119C != 0) {
        if (func_001F99C0(arg0->z - *(f32 *)0x1611A8) < 0.5f) {
            if (FUN_001f9b80(arg0, &D_001611A0) < D_001611AC) {
                if (arg1 != 0) {
                    FUN_001f9a00(arg1);
                }
                return *(f32 *)0x1611A8;
            }
        }
    }
    if (arg1 != 0) {
        FUN_001f9a00(arg1);
    }
    return arg0->z;
}

extern __typeof__(FUN_002135f0) func_002135F0 __attribute__((alias("FUN_002135f0")));
