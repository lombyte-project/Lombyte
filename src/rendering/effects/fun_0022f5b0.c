#include "types.h"
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

typedef struct {
    u8 pad0[0x26];
    s16 level;
} GameState;

extern GameState D_0013E030;
extern f32 D_0015ED6C MACRO_ADDR;
extern u128 D_001D9B60[][6];

extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern void FUN_001f9cf8(void *, void *, void *);
extern void FUN_001f9a10(void *, void *, void *);
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 func_00218888(void *, void *, void *, s32, s32, s32, s32, s32, s32);

void FUN_0022f5b0(u8 *m, f32 z) __asm__("FUN_0022f5b0");

void FUN_0022f5b0(u8 *m, f32 z) {
    Vec4 vel;
    Vec4 vel2;
    Vec4 pos;
    s32 i;
    s32 a;
    s32 b;
    s32 c;

    for (i = 0; i < 6; i++) {
        vel.f[0] = random_float_between(-D_0015ED6C, D_0015ED6C);
        vel.f[1] = random_float_between(-D_0015ED6C, D_0015ED6C);
        vel.f[2] = z + random_float_between(D_0015ED6C * -0.25f, D_0015ED6C * 0.25f);
        vel.f[3] = 0.4f;
        qcopy(&vel2, &vel);
        vel2.f[3] = 0.6f;
        qcopy(&pos, &D_001D9B60[D_0013E030.level][i]);
        FUN_001f9cf8(&pos, &pos, m + 0xC0);
        FUN_001f9a10(&pos, &pos, m + 0x10);
        a = scale_game_frames(4);
        b = scale_game_frames(4);
        c = scale_game_frames(4);
        func_00218888(&pos, &vel, &vel2, 0x24C0C0C0, 0x14C0C0C0, a, b,
                      c + random_integer_below(scale_game_frames(4)), -1);
    }
}

extern __typeof__(FUN_0022f5b0) func_0022F5B0 __attribute__((alias("FUN_0022f5b0")));
