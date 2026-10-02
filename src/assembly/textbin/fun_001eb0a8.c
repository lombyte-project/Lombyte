#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001eb0a8/FUN_001eb0a8.s", FUN_001eb0a8);
#else
#include "types.h"

typedef struct { f32 x, y, z, w; } Vec4 __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    Vec4 pos;
    u8 pad20[0x30];
    u8 k0;
    u8 k1;
    u8 pad52[2];
    f32 t;
    u8 pad58[0x19];
    u8 b71;
    u8 pad72[0xD];
    u8 b7F;
    u8 pad80[0x26];
    s16 hA6;
} Actor;

typedef struct {
    u8 pad0[0x78];
    Vec4 *keys;
} ActorKeys;

typedef struct {
    u8 pad0[0x34];
    s32 time;
    s32 frame;
    s32 index;
    s16 len;
    u8 pad42[2];
    s16 count;
    u8 pad46[0x132];
    Actor *actors[1];
} Cutscene;

extern Cutscene D_0018CB20;
extern f32 D_0015F43C;
extern s32 D_0015F604;
extern s32 D_0015EF50;
extern s32 D_0015EF54;
extern s32 D_0015EF58;
extern s32 D_0013CAE4[];
extern void InitializeTransferCommand(void);
extern void func_001E9410(Actor *);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void func_001EAF88(void);
extern s32 func_001F96F8(s32);
extern void func_001F9A10(Vec4 *, Vec4 *, Vec4 *);
extern void func_001F9A68(Vec4 *, Vec4 *, f32);
extern f32 func_001F9DC8(f32);
extern f32 func_001FA6C0(s32);
extern void func_001FCE28(void);
extern void func_002049F0(s32);
extern void func_0020C880(Actor *);
extern void func_0020DEF8(Actor *);
extern void func_002192A8(void);
extern void func_0022CA50(void);

void FUN_001eb0a8(void)
{
    Vec4 a;
    Vec4 b;
    Actor *act;
    Vec4 *keys;
    s32 i;
    s32 k;

    func_001E9430();
    D_0015F43C -= 0.0625f;
    D_0018CB20.frame++;
    D_0018CB20.time++;
    if (D_0015F43C < 0.0f) {
        D_0015F43C = 0.0f;
    }
    if (D_0018CB20.time >= D_0018CB20.len) {
        D_0018CB20.index = 0;
        D_0018CB20.time = 0;
        func_002049F0(0);
    } else if (D_0018CB20.frame >= 0x60) {
        func_002049F0(++D_0018CB20.index);
    }
    func_001EAF88();
    for (i = 0; i < D_0018CB20.count; i++) {
        act = D_0018CB20.actors[i];
        k = D_0018CB20.frame >> 1;
        act->k0 = k;
        act->k1 = k + 1;
        func_0020C880(act);
        act->t = func_001FA6C0(D_0018CB20.frame & 1) * 0.5f;
        keys = ((ActorKeys *)act)->keys;
        func_001F9A68(&a, &keys[act->k0], 1.0f - act->t);
        func_001F9A68(&b, &keys[act->k1], act->t);
        func_001F9A10(&act->pos, &a, &b);
        act->b71 = 0xFF;
        func_0020DEF8(act);
        act->b7F = 0;
        if (act->hA6 == 0) {
            func_001E9410(act);
        }
    }
    func_001E9428();
    if (D_0015F604 == 0) {
        D_0015EF58++;
        if (func_001F96F8(0x3C) < D_0015EF58) {
            if (++D_0015EF50 > 0x40) {
                D_0015EF50 = 0x40;
            }
        }
        if (func_001F96F8(0x78) < D_0015EF58) {
            D_0015EF54 = (s32)(func_001F9DC8((D_0015EF58 - func_001F96F8(0x78)) % 60 * 0.10471976f + -3.1415927f) * 32.0f) + 0x60;
        }
        if (D_0013CAE4[0] & 0x840) {
            InitializeTransferCommand();
        }
        func_0022CA50();
    } else if (D_0015F604 == 3) {
        D_0015EF58 = func_001F96F8(0x3C);
        if ((D_0015EF50 -= 0x10) < 0) {
            D_0015EF50 = 0;
        }
        if ((D_0015EF54 -= 0x10) < 0) {
            D_0015EF54 = 0;
        }
        func_002192A8();
        func_0022CA50();
    } else if (D_0015F604 == 4) {
        func_001FCE28();
        func_0022CA50();
    }
}
#endif /* NON_MATCHING */
