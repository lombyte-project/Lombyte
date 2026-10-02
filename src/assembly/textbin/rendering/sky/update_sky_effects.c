#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/sky/update_sky_effects/FUN_0022ae70.s", FUN_0022ae70);
#else
#include "types.h"

struct M2c_D_0016045C {
    u8 pad_0[4];
    s16 unk4;
    u8 pad_6[2];
    s16 unk8;
    u8 pad_A[0x12];
    s32 unk1C;
};

union M2c_C {
    s16 h0;
    u16 h2;
    u32 w;
};

struct M2c_Ent {
    u8 flag;
    u8 pad1[1];
    u8 b2;
    u8 b3;
    u32 col4;
    f32 f8;
    union M2c_C c;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
};

extern struct M2c_D_0016045C * volatile D_0016045C;
extern u8 D_001D96E0[];
extern void func_001160C8(s32);
extern f32 func_001F99C0(f32);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9DE0(f32);
extern void func_001F9FC8(u8 *);
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA6C0(s32);
extern s32 func_00213260(s32);
extern f32 func_00213308();
extern void func_0022B690(s32);
extern void func_0022BBA0();
extern void func_00233980(s32, u64);
extern s32 rand();

void update_sky_effects(void) __asm__("FUN_0022ae70");

void update_sky_effects(void) {
    f32 t;
    f32 t20;
    f32 t21;
    f32 t22;
    s16 v2;
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    s32 base;
    s32 one;
    s32 col;
    f32 f50;
    f32 fs;
    f32 c16;
    struct M2c_Ent *e;

    D_0016045C->unk4 = 0;
    func_001F9FC8(D_001D96E0);
    func_0022B690(0);
    func_0022B690(1);
    if (D_0016045C->unk8 == 0) {
        D_0016045C->unk8 = 0x100;
        i = 0;
        func_001160C8(0x3039);
        if (D_0016045C->unk8 <= 0) {
            goto end;
        }
        base = 0x30505050;
        col = 0x48;
        one = 1;
        f50 = 50.0f;
loop_3:
        e = (struct M2c_Ent *) (D_0016045C->unk1C + (i << 5));
        if (i >= 0xF6) {
            e->flag = 0;
            e->c.h0 = (s16) (rand() >> 0x10);
            e->c.h2 = (s16) (rand() >> 0x10);
            e->f1C = c16;
            e->b2 = one;
            e->b3 = col;
        } else {
            e->flag = 1;
            v2 = func_00213260(0x100);
            e->b2 = one;
            e->b3 = col;
            e->c.h0 = v2;
            e->f8 = func_00213308();
            e->f1C = func_001FA6C0(func_00213260(0x18) + 0x20) * 0.00390625f;
            func_001FA580(-3.0f, func_00213308() * 0.2f);
            t22 = func_00213308();
            t21 = t22 * 0.09f + 1.2f;
            t20 = func_001F9DC8(t22);
            e->f10 = t20 * func_001F9DE0(t21) * f50;
            t20 = func_001F9DE0(t22);
            e->f14 = t20 * func_001F9DE0(t21) * f50;
            e->f18 = func_001F9DC8(t21) * f50;
            v2 = func_00213260(0x18);
            k = func_00213260(0x20) << 0x18;
            if ((rand() >> 0x10) & 1) {
                e->c.w = k + ((v2 << 0x10) + base);
            } else {
                e->c.w = (k + ((v2 << 8) + base)) | v2;
            }
        }
        i += 1;
        if (i < D_0016045C->unk8) {
            goto loop_3;
        }
    }
    if (D_0016045C->unk8 <= 0) {
        goto end;
    }
    f50 = 50.0f;
    fs = 0.0015339808f;
    j = 0;
loop_15:
    e = (struct M2c_Ent *) (D_0016045C->unk1C + (j << 5));
    if (e->flag == 0) {
        e->c.h0 = (s16) (e->c.h0 + 1);
        e->c.h2 = (u16) (e->c.h2 + 1);
        t22 = func_001FA6C0((e->c.h0 & 0xFFF) - 0x800) * fs;
        t21 = func_001FA6C0((e->c.h2 & 0xFFF) - 0x800) * fs;
        t20 = func_001F9DC8(t22);
        e->f10 = t20 * func_001F9DE0(t21) * f50;
        t20 = func_001F9DE0(t22);
        e->f14 = t20 * func_001F9DE0(t21) * f50;
        e->f18 = func_001F99C0(func_001F9DC8(t21)) * f50;
        if ((u32) (e->c.h0 & 0x3F) < 8U) {
            e->col4 = 0x702020F0;
        } else {
            e->col4 = 0x202020F0;
        }
    } else {
        r = rand() >> 0x10;
        e->col4 = e->c.w + (((r & 0x1F00) << 0xA) + 0xFFDFDFE0) + ((r & 0x1F0) << 6) + ((r & 0x1F) << 2);
    }
    j += 1;
    if (j < D_0016045C->unk8) {
        goto loop_15;
    }
end:
    func_0022BBA0();
    func_00233980(0x42, (0x8000ULL << 0x18) | 0x44);
    func_0022B690(2);
    func_0022B690(3);
}
#endif /* NON_MATCHING */
