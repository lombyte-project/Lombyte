#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203b08/FUN_00203b08.s", FUN_00203b08);
#else
#include "rnc/fun_00203b08_types.h"
#include "types.h"


















extern s32 D_0015EE8C;
extern s32 D_001603CC;
extern struct M2c_D_0019E540 D_0019E540;
extern u8 D_001D7F30[];
extern u8 D_001D8030[];
extern u8 D_001D80B0[];
extern u8 D_001D8CB0[];
extern u8 D_001D92B0[];
extern s32 func_001F97A0();
extern s32 func_001FA6D0();
void FUN_00203b08(struct M2c_arg0 *arg0, s32 arg1, s64 *arg2, struct M2c_arg3 *arg3, s16 arg4) {    s32 sp0;
u8 sp_slot[0x110];    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s64 *sp14;
    s32 sp18;
    s64 sp20;
    s64 sp30;
u8 *sp40;
    s64 sp50;
    s64 sp60;
    s32 *var_14_242;
    s64 *temp_3_220;
    s64 var_11_229;
    s16 *temp_10_26;
    s16 temp_5_89;
    s32 *temp_4_53;
    struct M2c_var_13_234 *var_13_234;
    s32 *var_3_92;
    s32 temp_10_248;
    s32 temp_18_262;
    s32 temp_19_263;
    s32 temp_19_87;
    s32 temp_20_88;
    s32 temp_22_249;
    s32 temp_23_253;
    s32 temp_2_45;
    s32 temp_2_96;
    s32 temp_30_251;
    s32 temp_3_190;
    s32 temp_3_338;
    s32 temp_4_260;
    s32 temp_4_281;
    s32 temp_4_86;
    s32 temp_6_84;
    s32 temp_8_126;
    s32 var_2_227;
    s32 var_4_176;
    s32 var_4_226;
    s32 var_4_67;
    s32 var_4_90;
    s64 *var_3_177;
    s64 temp_16_110;
    s64 temp_16_275;
    s64 temp_18_324;
    s64 temp_2_109;
    s64 temp_5_303;
    u16 temp_4_257;
    u8 temp_21_250;
    void **temp_8_32;
    void **var_3_68;
    struct M2c_temp_17_256 *temp_17_256;
    struct M2c_temp_18_82 *temp_18_82;
    s32 *temp_2_58;
    s32 *temp_2_80;
    struct M2c_temp_3_233 *temp_3_233;
    struct M2c_var_12_57 *var_12_57;
    struct M2c_var_20_240 *var_20_240;

    temp_10_26 = (D_001603CC * 2) + D_001D8030;
    *(s32 *)((u8 *)D_001D80B0 + arg4) = (u8) D_001603CC;
    temp_8_32 = (D_001603CC * 4) + D_001D7F30;
    sp10 = arg1;
    *temp_10_26 = arg4;
    *temp_8_32 = arg0;
    arg0->unk26 = (u16) D_001603CC;
    sp40 = (s64) arg0;
    temp_2_45 = func_001FA6D0(temp_8_32, D_001D7F30, temp_10_26, (u8) D_001603CC, arg0->unk10 * 1024.0f);
    temp_4_53 = (D_001603CC * 4) + D_001D8CB0;
    D_001603CC += 1;
    *temp_4_53 = temp_2_45;
    var_12_57 = arg0;
    temp_2_58 = var_12_57->unk2C;
    var_12_57->unk18 = 0;
    var_12_57->unk16 = 0;
    if (temp_2_58 != NULL) {
        var_12_57->unk2C = (void *) (((u8 *)var_12_57 + ((u32)temp_2_58)));
    }
    var_4_67 = 0;
    if (var_12_57->unk28 > 0) {
        var_3_68 = ((u8 *)var_12_57) + 0x40;
        do {
            var_4_67 += 1;
            *(s32 *)var_3_68 += (u32)var_12_57;
            var_3_68 += 8;
        } while (var_4_67 < var_12_57->unk28);
    }
    temp_2_80 = var_12_57->unk1C;
    temp_18_82 = ((u8 *)var_12_57 + ((u32)temp_2_80));
    if (temp_2_80 != NULL) {
        var_12_57->unk1C = temp_18_82;
        temp_6_84 = temp_18_82->unk10;
        temp_4_86 = temp_18_82->unk14;
        if (arg3 != NULL) {
            temp_19_87 = temp_4_86;
            temp_20_88 = temp_6_84;
            temp_5_89 = arg3->unk0;
            var_4_90 = 0;
            var_3_92 = sp_slot;
            do {
                temp_2_96 = temp_5_89 >> (var_4_90 + 6);
                *var_3_92 = temp_2_96;
                if (temp_2_96 <= 0) {
                    *var_3_92 = 1;
                }
                var_4_90 += 1;
                var_3_92 += 4;
            } while (var_4_90 < 4);
            sp40 = (s64) var_12_57;
            temp_16_110 = func_001F97A0(arg3->unk0, temp_5_89, 1);
            temp_2_109 = func_001F97A0(arg3->unk2);
            temp_8_126 = (s32) D_0015EE8C >> 8;
            temp_18_82->unk10 = (s64) (((arg3->unk4 - 1) * 4) | (((s64) temp_19_87 << 6) | 0x20) | ((s64) temp_20_88 << 0x20));
            temp_18_82->unk20 = (s64) ((arg3->unk8 + temp_8_126) | ((s64) sp0 << 0xE) | ((temp_16_110 << 0x1A) | 0x01300000) | (temp_2_109 << 0x1E) | (((arg3->unk6 + temp_8_126) << 0x25) | (0x8000 << 0x13)) | (-1 << 0x3F));
            temp_18_82->unk30 = (s64) ((arg3->unkA + temp_8_126) | ((s64) sp4 << 0xE) | ((arg3->unkC + temp_8_126) << 0x14) | ((s64) sp8 << 0x22) | ((arg3->unkE + temp_8_126) << 0x28) | ((s64) spC << 0x36));
        } else {
            var_4_176 = 0;
            if (D_0019E540.unk18 != 0) {
                var_3_177 = ((u8 *)&D_0019E540) + 0x18;
                do {
                    var_3_177 += 0x18;
                    var_4_176 += 1;
                } while (*var_3_177 != 0);
            }
            temp_3_190 = var_4_176 * 3;
            temp_18_82->unk20 = (s64) *(s32 *)((var_4_176 * 0x18) + &D_0019E540);
            temp_18_82->unk30 = (s64) *(s32 *)(((temp_3_190 + 2) * 8) + &D_0019E540);
            temp_18_82->unk10 = (s64) ((*(s32 *)(((temp_3_190 + 1) * 8) + &D_0019E540) & 0x1C) | (((s64) temp_4_86 << 6) | 0x20) | ((s64) temp_6_84 << 0x20));
        }
    }
    temp_3_220 = (*(s32 *)((u8 *)D_001D80B0 + arg4) * 0x10) + D_001D92B0;
    sp14 = temp_3_220;
    *temp_3_220 = *arg2;
    var_4_226 = 0;
    if (var_12_57->unk28 > 0) {
        var_2_227 = 0 * 8;
        do {
            var_11_229 = 0;
            sp18 = var_4_226 + 1;
            temp_3_233 = *(s32 *)((u8 *)(((u8 *)var_12_57) + var_2_227) + 0x40);
            var_13_234 = ((u8 *)temp_3_233) + 0x10;
            var_20_240 = ((u8 *)var_13_234) + ((u32)var_13_234->unk4 * 0x10) + 0x10;
            if (temp_3_233->unk10 > 0) {
                var_14_242 = &D_0019E540;
                do {
                    temp_10_248 = var_20_240->unk0;
                    temp_22_249 = var_20_240->unk4;
                    temp_21_250 = *(s32 *)((u8 *)sp14 + var_20_240->unk20);
                    temp_30_251 = var_20_240->unk10;
                    temp_23_253 = var_20_240->unk14;
                    if (sp10 != 0) {
                        temp_17_256 = sp10 + (temp_21_250 * 0x10);
                        temp_4_257 = temp_17_256->unk4;
                        sp20 = (s64) temp_10_248;
                        temp_4_260 = temp_4_257 << 0x10;
                        sp30 = var_11_229;
                        temp_18_262 = temp_4_260 >> 0x16;
                        temp_19_263 = temp_4_260 >> 0x17;
                        sp40 = (s64) var_12_57;
                        sp50 = (s64) var_13_234;
                        sp60 = (s64) var_14_242;
                        temp_16_275 = func_001F97A0((s16) temp_4_257, 1);
                        temp_4_281 = (s32) D_0015EE8C >> 8;
                        temp_5_303 = 0x8000 << 0x13;
                        temp_18_324 = ((s64) ((temp_18_262 <= 0) ? 1 : temp_18_262) << 0xE) | ((temp_16_275 << 0x1A) | 0x01300000) | (func_001F97A0(temp_17_256->unk6) << 0x1E) | (((temp_17_256->unkA + temp_4_281) << 0x25) | temp_5_303) | (-1 << 0x3F);
                        var_20_240->unk0 = (s64) (((temp_17_256->unk8 - 1) * 4) | (((s64) temp_22_249 << 6) | 0x20) | ((s64) temp_10_248 << 0x20));
                        var_20_240->unk10 = (s64) (temp_30_251 | ((s64) temp_23_253 * 4) | ((s64) temp_21_250 << 0x18));
                        var_20_240->unk20 = (s64) (((s64) ((temp_19_263 <= 0) ? 1 : temp_19_263) << 0xE) | ((temp_17_256->unkC + temp_4_281) << 0x14) | (((temp_17_256->unkE + temp_4_281) << 0x28) | temp_5_303) | (0x8000 << 0x27));
                        var_20_240->unk30 = temp_18_324;
                    } else {
                        temp_3_338 = temp_21_250 * 3;
                        var_20_240->unk0 = (s64) ((*(s32 *)(((temp_3_338 + 1) * 8) + var_14_242) & 0x1C) | (((s64) temp_22_249 << 6) | 0x20) | ((s64) temp_10_248 << 0x20));
                        var_20_240->unk10 = (s64) (temp_30_251 | ((s64) temp_23_253 * 4) | ((s64) temp_21_250 << 0x18));
                        var_20_240->unk20 = (s64) *(s32 *)(((temp_3_338 + 2) * 8) + var_14_242);
                        var_20_240->unk30 = (s64) *(s32 *)((temp_21_250 * 0x18) + var_14_242);
                    }
                    var_11_229 += 1;
                    var_20_240 += 0x40;
                } while (var_11_229 < *(s32 *)var_13_234);
            }
            var_4_226 = sp18;
            var_2_227 = var_4_226 * 8;
        } while (var_4_226 < var_12_57->unk28);
    }
}

extern void func_00203B08(struct M2c_arg0 *arg0, s32 arg1, s64 *arg2, struct M2c_arg3 *arg3, s16 arg4) __attribute__((alias("FUN_00203b08")));
#endif /* NON_MATCHING */
