#include "types.h"
extern f32 D_00187080[4];
extern void FUN_001f9a28(f32 *, f32 *, f32 *);
extern void FUN_001f9d20(f32 *, f32 *, void *);
extern f32 FUN_001f9b20(f32 *);
extern f32 FUN_001f99e8(f32, f32, f32);
extern f32 FUN_001f9e90(f32, f32);
extern void truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void FUN_0022c830(s32 unused, f32 *pos, void *mtx) {
    f32 v[4] __attribute__((aligned(16)));
    f32 scale;

    FUN_001f9a28(v, pos, D_00187080);
    FUN_001f9d20(v, v, mtx);
    scale = FUN_001f99e8(FUN_001f9b20(v) - 1.0f, 0.0f, 1.0f);
    truncate_float_to_s32(-FUN_001f9e90(v[0], v[1]) * 180.0f * scale * 0.31830987f);
}

extern __typeof__(FUN_0022c830) func_0022C830 __attribute__((alias("FUN_0022c830")));
