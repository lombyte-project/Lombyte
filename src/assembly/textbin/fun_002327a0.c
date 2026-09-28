#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002327a0/FUN_002327a0.s", FUN_002327a0);
#else
#include "rnc/fun_002327a0_types.h"
#include "types.h"












extern s32 D_0013E050;
extern s32 D_0015F604;
extern s32 D_001604A4;
extern s32 D_001604A8;
extern u8 D_001604AC[];
extern u8 D_00160520[];
extern u8 D_00160530[];
extern u8 D_00160540[];
extern u8 D_00160550[];
extern u8 D_00160560[];
extern u8 D_00160570[];
extern struct M2c_D_00186F40 D_00186F40;
extern u8 D_00187080[];
extern u8 D_001DC4E0[];
extern u8 D_001DCB40[];
extern u8 D_001DCE70[];
extern s32 func_001F44B8();
extern void func_001F7D30();
extern s32 func_001F96F8();
extern s32 func_001F9740();
extern s32 func_001F9988();
extern s32 func_001F99C0();
extern s32 func_001F9A28();
extern s32 func_001F9A68();
extern s32 func_001F9AB0();
extern s32 func_001F9BF8();
extern s32 func_001F9D20();
extern s32 func_001FA6C0();
extern s32 func_0020CCA8();
extern void sp50();
extern void sp54();
extern void spD0();
extern void spE0();
extern void spF0();
void FUN_002327a0(struct M2c_arg0 *arg0) {    s32 spE4;
u8 sp_slot[0x1D0];    f32 sp50;
    f32 sp54;
    s64 sp70;
    s64 sp78;
    s64 sp80;
    s64 sp88;
    s32 sp90;
    f32 spD0;
    f32 spE0;
    f32 spE8;
    f32 spF0;
    s32 sp100;
    s32 sp104;
    s32 sp108;
    s32 *sp10C;
    struct M2c_var_16_258 *var_16_258;
    struct M2c_var_18_257 *var_18_257;
    struct M2c_var_20_147 *var_20_147;
    struct M2c_var_30_149 *var_30_149;
    s32 var_4_50;
    f32 *var_18_281;
    f32 *var_20_256;
    f32 *var_23_150;
    f32 *var_6_315;
    f32 *var_7_313;
    f32 temp_f0_194;
    u32 temp_f20_130;
    f32 temp_f21_137;
    f32 temp_f2_220;
    f32 temp_f2_224;
    f32 temp_f4_201;
    f32 var_f1_211;
    s64 *var_5_316;
    s16 *var_8_314;
    s16 temp_4_320;
    s32 temp_16_29;
    s32 temp_2_30;
    s32 temp_3_65;
    s32 temp_4_325;
    s32 temp_5_264;
    s32 temp_5_286;
    s32 temp_6_33;
    s32 var_16_280;
    s32 var_17_148;
    s32 var_17_255;
    s32 var_17_282;
    s32 var_17_301;
    s32 var_17_57;
    s32 var_19_35;
    s32 var_21_142;
    s32 var_22_37;
    s32 var_9_317;

    temp_16_29 = arg0->unkA6 - 0x212;
    temp_2_30 = temp_16_29 * 4;
    temp_6_33 = *(s32 *)(temp_2_30 + D_00160570);
    var_19_35 = *(s32 *)(temp_2_30 + D_00160560);
    var_22_37 = *(s32 *)(temp_2_30 + D_00160550);
    sp100 = temp_6_33;
    sp104 = *(s32 *)(temp_2_30 + D_00160540);
    sp108 = *(s32 *)(temp_2_30 + D_00160530);
    if (D_0015F604 != 6) {
        goto block_3;
    }
    var_4_50 = 0x15;
    if (D_0013E050 != 4) {
        goto block_4;
    }
    var_4_50 = 1;
    goto block_4;
block_3:
    var_4_50 = 0x15;
block_4:
    var_17_57 = 0;
    sp78 = func_001F44B8(var_4_50, 6, temp_6_33, D_00160540, D_0015F604);
    temp_3_65 = *(s32 *)((temp_16_29 * 4) + D_00160520);
    sp88 = (0x8000 << 0x18) | 0x44;
    sp80 = (0xFF90 << 0x20) | 0x260;
    sp70 = 0;
    func_0020CCA8(arg0, 0, &sp90, temp_3_65, temp_3_65, temp_3_65, temp_3_65);
    if (D_0015F604 != 0) {
        goto block_7;
    }
    if (!(func_001F99C0(D_00186F40.unk140 - arg0->unk10) < 16.0f)) {
        goto block_8;
    }
    if (!(func_001F99C0(D_00186F40.unk144 - arg0->unk14) < 16.0f)) {
        goto block_8;
    }
block_7:
    var_17_57 = 1;
block_8:
    if (D_0015F604 != 6) {
        goto block_10;
    }
    var_17_57 = ((D_0013E050 ^ 4) == 0) ? 0 : var_17_57;
block_10:
    if (var_17_57 != 0) {
        goto block_13;
    }
    if (D_001604A4 != 1) {
        goto block_22;
    }
block_13:
    D_001604A8 = 1;
    func_001F9740(D_001604AC);
    temp_f20_130 = func_001FA6C0(*(s32 *)D_001604AC);
    temp_f21_137 = temp_f20_130 / func_001FA6C0(func_001F96F8(0x3C));
    if (sp108 <= 0) {
        goto block_20;
    }
    sp10C = &sp90;
    var_21_142 = var_19_35;
    var_20_147 = D_001DCB40;
    var_17_148 = sp108;
    var_30_149 = D_001DCE70;
    var_23_150 = D_001DC4E0;
loop_15:
    func_001F9D20(var_23_150, var_21_142, sp10C);
    func_001F9A28(&spF0, var_23_150, D_00187080);
    func_001F9BF8(&spF0, &spF0, 0x3F800000);
    func_001F9D20(&spD0, var_22_37, sp10C);
    func_001F9BF8(&spD0, &spD0, 0x3DCCCCCD);
    func_001F9A68(&spE0, &spD0, 2.0f * func_001F9AB0(&spD0, &spF0));
    func_001F9A28(&spE0, &spF0, &spE0);
    func_001F9BF8(&spE0, &spE0, 0x3F800000);
    temp_f0_194 = spE8 + 1.0f;
    spE8 = temp_f0_194;
    temp_f4_201 = 2.0f * func_001F9988(2.0f * temp_f0_194);
    if (D_001604A4 == 1) {
        goto block_17;
    }
    if (*(s32 *)D_001604AC != 0) {
        goto block_18;
    }
block_17:
    var_f1_211 = (spE4 / temp_f4_201) + 0.5f;
    var_20_147->unk0 = (f32) ((spE0 / temp_f4_201) + 0.5f);
    goto block_19;
block_18:
    temp_f2_220 = (spE0 / temp_f4_201) + 0.5f;
    temp_f2_224 = (spE4 / temp_f4_201) + 0.5f;
    var_20_147->unk0 = (f32) (temp_f2_220 + ((var_30_149->unk0 - temp_f2_220) * temp_f21_137));
    var_f1_211 = temp_f2_224 + ((var_30_149->unk4 - temp_f2_224) * temp_f21_137);
block_19:
    var_20_147->unk4 = var_f1_211;
    var_20_147 += 8;
    var_30_149 += 8;
    var_22_37 += 0x10;
    var_21_142 += 0x10;
    var_17_148 -= 1;
    var_23_150 += 0x10;
    if (var_17_148 != 0) {
        goto loop_15;
    }
block_20:
    if (D_001604A4 != 1) {
        goto block_32;
    }
    D_001604A4 = 2;
    goto block_31;
block_22:
    if (D_001604A8 != 1) {
        goto block_27;
    }
    D_001604A8 = 0;
    if (sp108 <= 0) {
        goto block_30;
    }
    var_17_255 = sp108;
    var_20_256 = D_001DC4E0;
    var_18_257 = D_001DCE70;
    var_16_258 = D_001DCB40;
loop_25:
    temp_5_264 = var_19_35;
    var_18_257->unk0 = (f32)(u32)var_16_258->unk0;
    var_18_257->unk4 = (f32)(u32)var_16_258->unk4;
    var_19_35 += 0x10;
    func_001F9D20(var_20_256, temp_5_264, &sp90);
    var_20_256 += 0x10;
    var_18_257 += 8;
    var_17_255 -= 1;
    var_16_258 += 8;
    if (var_17_255 != 0) {
        goto loop_25;
    }
    goto block_30;
block_27:
    if (sp108 <= 0) {
        goto block_30;
    }
    var_16_280 = var_19_35;
    var_18_281 = D_001DC4E0;
    var_17_282 = sp108;
loop_29:
    temp_5_286 = var_16_280;
    var_16_280 += 0x10;
    func_001F9D20(var_18_281, temp_5_286, &sp90);
    var_17_282 -= 1;
    var_18_281 += 0x10;
    if (var_17_282 != 0) {
        goto loop_29;
    }
block_30:
    *(s32 *)D_001604AC = func_001F96F8(0x3C);
block_31:
block_32:
    var_17_301 = 0;
    if (sp104 <= 0) {
        goto block_37;
    }
loop_34:
    var_7_313 = &sp54;
    var_8_314 = (var_17_301 * 0x10) + sp100;
    var_6_315 = &sp50;
    var_5_316 = sp_slot;
    var_9_317 = 3;
loop_35:
    temp_4_320 = *var_8_314;
    *var_5_316 = *(s32 *)((temp_4_320 * 0x10) + D_001DC4E0);
    temp_4_325 = temp_4_320 * 8;
    var_5_316 += 0x10;
    var_8_314 += 4;
    var_9_317 -= 1;
    *var_6_315 = *(s32 *)(temp_4_325 + D_001DCB40);
    *var_7_313 = *(s32 *)(temp_4_325 + (D_001DCB40 + 4));
    var_6_315 += 8;
    var_7_313 += 8;
    if (var_9_317 >= 0) {
        goto loop_35;
    }
    var_17_301 += 1;
    func_001F7D30(sp_slot, 0, 0, var_7_313, var_8_314, var_9_317);
    if (var_17_301 < sp104) {
        goto loop_34;
    }
block_37:
    return;
}

extern void func_002327A0(struct M2c_arg0 *arg0) __attribute__((alias("FUN_002327a0")));
#endif /* NON_MATCHING */
