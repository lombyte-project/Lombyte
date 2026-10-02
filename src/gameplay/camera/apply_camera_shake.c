#include "types.h"

struct M2c_D_001870C0 {
    u8 pad_0[0x86];
    s16 unk86;
    u8 pad_88[0x2];
};

struct M2c_arg0 {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    s32 unkC;
};

extern struct M2c_D_001870C0 *D_001870C0[];
extern u8 D_00187080[];
extern u8 D_00187290[];
extern u8 D_001872B0[];

extern s32 func_001F9740(s32 *);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_001f9bf8(f32 *, f32 *, f32);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 func_001FA610(f32);
extern f32 func_001FA6C0(s32);

void apply_camera_shake(struct M2c_arg0 *arg0, s32 arg1) __asm__("FUN_001ed360");

void apply_camera_shake(struct M2c_arg0 *arg0, s32 arg1) {
    f32 sp_slot[4];
    f32 v1;
    f32 v2;
    f32 v3;
    f32 v4;
    f32 val;
    s32 v5;

    if (D_001870C0[0] != 0 && D_001870C0[0]->unk86 == 6) {
                arg0->unk8 = 0;
        arg0->unkC = 0;
    } else {
        v5 = arg0->unk8;
        if (v5 != 0) {
            if (arg0->unkC < v5) {
                arg0->unkC = v5;
            }
            func_001F9740(&arg0->unk8);
            v1 = func_001FA6C0(arg0->unk8);
            v2 = func_001FA6C0(arg0->unkC);
            v3 = v1 / v2;
            v4 = fast_cos(func_001FA610(2.0f * func_001FA6C0(arg0->unk8)));
            val = arg0->unk0 * v4 * v3 * v3;
            arg0->unk4 = val;
            if (arg1 == 0) {
                FUN_001f9bf8(sp_slot, D_001872B0, val);
            } else {
                FUN_001f9bf8(sp_slot, D_00187290, val);
            }
            FUN_001f9a10(D_00187080, D_00187080, sp_slot);
        } else {
            arg0->unkC = 0;
        }
    }
}

extern __typeof__(apply_camera_shake) func_001ED360 __attribute__((alias("FUN_001ed360")));
