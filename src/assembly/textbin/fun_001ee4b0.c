#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ee4b0/FUN_001ee4b0.s", FUN_001ee4b0);
#else
#include "types.h"

struct Fog {
    u8 pad0[0x50];
    s32 flags;
    s32 color0;
    s32 color1;
    f32 near0;
    f32 far0;
    f32 lo0;
    f32 hi0;
    f32 near1;
    f32 far1;
    f32 lo1;
    f32 hi1;
    u8 pad7C[4];
};

extern struct Fog D_0019ADC0[];
extern u8 D_0015F484;
extern u8 D_0015F485;
extern u8 D_0015F486;
extern f32 D_0015F488;
extern f32 D_0015F48C;
extern f32 D_0015F490;
extern f32 D_0015F494;
extern s32 func_00212C28(void *, f32 *, s32 *);
extern s32 func_001FA6D0(f32);

void FUN_001ee4b0(void *pos) {
    f32 t;
    s32 idx;
    struct Fog *f;
    u32 a;
    u32 b;
    s32 c0;
    s32 c1;
    f32 u;
    s32 x;
    s32 y;

    if (func_00212C28(pos, &t, &idx) == 0) {
        return;
    }
    f = &D_0019ADC0[idx];
    if (!(f->flags & 2)) {
        return;
    }
    a = func_001FA6D0(t * 255.0f);
    b = 255 - a;
    u = 1.0f - t;
    c0 = f->color1;
    c1 = f->color0;
    x = ((c0 & 0xFF) * a + (c1 & 0xFF) * b) >> 8;
    y = (((c0 >> 8) & 0xFF) * a + ((c1 >> 8) & 0xFF) * b) >> 8;
    D_0015F486 = (((c0 >> 16) & 0xFF) * a + ((c1 >> 16) & 0xFF) * b) >> 8;
    D_0015F484 = x;
    D_0015F485 = y;
    D_0015F488 = (f->near1 * t + f->near0 * u) * 1024.0f;
    D_0015F48C = (f->lo1 * t + f->lo0 * u) * 1024.0f;
    D_0015F490 = 255.0f - (f->far1 * t + f->far0 * u) * 255.0f;
    D_0015F494 = 255.0f - (f->hi1 * t + f->hi0 * u) * 255.0f;
}
#endif /* NON_MATCHING */
