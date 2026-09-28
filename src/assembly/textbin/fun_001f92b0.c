#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f92b0/FUN_001f92b0.s", FUN_001f92b0);
#else
#include "rnc/fun_001f92b0_types.h"
#include "types.h"










extern s32 D_001608E0;
extern struct M2c_D_00160F00 *D_00160F00;
extern u8 D_00187080[];
extern struct M2c_D_0018CD00 D_0018CD00;
extern u8 D_0018CE80[];
extern u8 D_0018ED00[];
extern s32 FillTransferWords();
extern s32 func_001F44B8();
extern s32 func_001F9958();
extern s32 func_001F9A28();
extern s32 func_001F9A68();
extern s32 func_001F9A98();
extern s32 func_001F9AF0();
extern s32 func_001F9D20();
extern s32 func_001F9DC8();
extern s32 func_001F9DE0();
extern s32 func_001FA6C0();
extern s32 func_001FA6D0(f32);
extern void func_00233980(s32, s64);
void FUN_001f92b0(void) {    s32 sp0;
u8 sp_slot[0xE0];    s32 sp4;
    s32 sp8;
    f32 spC;
    s32 sp10;
    f32 temp_f20_133;
    f32 var_f0_102;
    f32 var_f21_52;
    s32 temp_17_89;
    s32 temp_19_108;
    s32 var_21_14;
    s64 temp_16_95;
    s64 temp_17_145;
    s64 temp_18_138;
    s64 temp_20_87;
    struct M2c_temp_16_167 *temp_16_167;
    struct M2c_temp_18_34 *temp_18_34;
    struct M2c_temp_5_160 *temp_5_160;

    var_21_14 = 0;
    func_00233980(0x42, (0x8000 << 0x18) | 0x48);
    do {
        temp_18_34 = (var_21_14 << 5) + D_0018ED00;
        if (temp_18_34->unk10 > 0) {
            func_001F9A28(sp_slot, temp_18_34, D_00187080);
            spC = 1.0f;
            var_f21_52 = func_001F9AF0(sp_slot);
            func_001F9A68(sp_slot, sp_slot, 1024.0f);
            func_001F9D20(sp_slot, sp_slot, ((u8 *)D_00187080 - (u8 *)0x100));
            func_001F9A98(&sp10, sp_slot, D_0018CE80);
            if (func_001F9958(&sp10) != 0) {
                FillTransferWords(temp_18_34, 0, 0x20);
            } else {
                func_001F9A68(sp_slot, sp_slot, D_0018CD00.unk210 / spC);
                temp_20_87 = (temp_18_34->unk12 << 0x18) | 0x808080;
                temp_17_89 = func_001FA6D0(sp0 * 16.0f) + 0x8000;
                temp_16_95 = func_001FA6D0(sp4 * 16.0f) + 0x8000;
                var_f0_102 = 18.0f;
                temp_19_108 = (func_001FA6D0((sp8 * 0.9997f) + D_0018CD00.unk1A8) << 0x20) | (temp_16_95 << 0x10) | temp_17_89;
                if ((var_f21_52 > 18.0f) || (var_f0_102 = 2.0f, (var_f21_52 < 2.0f))) {
                    var_f21_52 = var_f0_102;
                }
                temp_f20_133 = temp_18_34->unk1C * (func_001FA6C0(temp_18_34->unk12 + 0x10) * 0.015625f) * ((24.0f - var_f21_52) * 16.0f);
                temp_18_138 = func_001FA6D0(temp_f20_133 * func_001F9DE0(temp_18_34->unk18));
                temp_17_145 = func_001FA6D0(temp_f20_133 * func_001F9DC8(temp_18_34->unk18));
                D_00160F00->unk0 = 0x10000009;
                D_00160F00->unk4 = 0;
                D_00160F00->unk8 = 0;
                D_00160F00->unkC = 0x50000009;
                temp_5_160 = D_00160F00;
                D_00160F00 = ((u8 *)temp_5_160 + (0x10));
                temp_5_160->unk10 = (s64) D_001608E0;
                temp_16_167 = ((u8 *)temp_5_160 + (0x20));
                D_00160F00 = temp_16_167;
                temp_5_160->unk20 = 5;
                temp_16_167->unk8 = func_001F44B8(0x13, temp_5_160);
                temp_16_167->unk10 = 0x154;
                temp_16_167->unk28 = (s64) ((temp_17_145 << 0x10) + temp_19_108 + temp_18_138);
                temp_16_167->unk38 = 0x200;
                temp_16_167->unk40 = (s64) ((temp_18_138 * -0x10000) + temp_19_108 + temp_17_145);
                temp_16_167->unk50 = 0x02000000;
                temp_16_167->unk58 = (s64) (((temp_18_138 << 0x10) + temp_19_108) - temp_17_145);
                temp_16_167->unk60 = temp_20_87;
                temp_16_167->unk68 = 0x02000200;
                temp_16_167->unk70 = (s64) (((temp_17_145 * -0x10000) + temp_19_108) - temp_18_138);
                temp_16_167->unk78 = 0;
                temp_16_167->unk18 = temp_20_87;
                temp_16_167->unk20 = 0;
                temp_16_167->unk30 = temp_20_87;
                temp_16_167->unk48 = temp_20_87;
                D_00160F00 += 0x80;
            }
        }
        var_21_14 += 1;
    } while (var_21_14 < 0x10);
    func_00233980(0x42, (0x8000 << 0x18) | 0x44);
}
#endif /* NON_MATCHING */
