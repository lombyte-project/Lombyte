#include "types.h"
struct Vec3 { f32 x; f32 y; f32 z; };
extern s32 D_001413D0[];
extern struct Vec3 D_001CAA80;
extern void FUN_0020d510(s32, f32 *);
extern f32 FUN_001f9e90(f32, f32);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);
void FUN_0020cfd0(void) {
    f32 v[4];
    f32 angle;

    FUN_0020d510(D_001413D0[0], v);
    angle = FUN_001f9e90(v[0], v[1]);
    D_001CAA80.x = func_001F9DC8(angle) * 0.14f;
    D_001CAA80.y = func_001F9DE0(angle) * 0.14f;
    D_001CAA80.z = -0.99f;
}

extern __typeof__(FUN_0020cfd0) func_0020CFD0 __attribute__((alias("FUN_0020cfd0")));
