#include "types.h"
extern s32 D_001601B4 __attribute__((sda));
extern s32 FUN_001f96f8(s32);
extern void func_001FA6E0(s32, s32, f32);
void FUN_0021b6d8(s32 delay, s32 from, s32 to) {
    s32 start;
    s32 b;
    s32 a;
    s32 now;
    f32 t;

    start = delay > -1 ? delay : 0;
    a = from != -1 ? from : 0x80FFA888;
    b = to != -1 ? to : 0x8020FFFF;
    if (FUN_001f96f8(D_001601B4) >= start) {
        now = FUN_001f96f8(D_001601B4);
        t = 1.0f - (f32)(now - start) / (f32)FUN_001f96f8(D_001601B4);
    } else {
        t = 1.0f;
    }
    func_001FA6E0(a, b, t);
}

extern __typeof__(FUN_0021b6d8) func_0021B6D8 __attribute__((alias("FUN_0021b6d8")));
