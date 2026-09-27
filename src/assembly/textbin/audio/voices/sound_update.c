#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/voices/sound_update/FUN_0022ca50.s", FUN_0022ca50);
#else
#include "types.h"
struct M2c_D_0013E550 {
    u8 pad_0[0x40];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad_60[0x4];
    s32 unk64;
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
    s32 unk6C;
    u8 pad_70[0xD28];
    s32 unkD98;
};

struct M2c_temp_16_261 {
    u8 pad_0[0x20];
    s32 unk20;
};

struct M2c_temp_16_383 {
    u8 pad_0[0x75];
    u8 unk75;
    u8 pad_76[0x2];
    struct M2c_temp_16_383_unk78 * unk78;
    u8 pad_7C[0x4];
    s32 unk80;
};

struct M2c_temp_16_383_unk78 {
    u8 pad_0[0x19];
    u8 unk19;
    u8 pad_1A[0x3];
};

struct M2c_temp_16_504 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 unk75;
    u8 pad_76[0x3A];
    s32 unkB0;
};

struct M2c_temp_16_722 {
    u8 pad_0[0x70];
    s32 unk70;
    u8 unk74;
    u8 pad_75[0x3];
    struct M2c_temp_16_722_unk78 * unk78;
    u8 pad_7C[0x8];
    s32 unk84;
    s32 unk88;
    s32 unk8C;
};

struct M2c_temp_16_722_unk78 {
    u8 pad_0[0x19];
    u8 unk19;
    u8 pad_1A[0x3];
};

struct M2c_temp_17_306 {
    u8 pad_0[0x75];
    u8 unk75;
    u8 pad_76[0x12];
    struct M2c_temp_17_306_unk88 * unk88;
    u8 pad_8C[0xC];
    f32 unk98;
};

struct M2c_temp_17_306_unk88 {
    u8 pad_0[0x10];
    f32 unk10;
};

struct M2c_temp_3_826 {
    u8 pad_0[0x1A];
    s16 unk1A;
    s32 unk1C;
};

struct M2c_temp_4_809 {
    u8 pad_0[0x74];
    u8 unk74;
    u8 pad_75[0x3];
    struct M2c_temp_3_826 * unk78;
};

struct M2c_var_19_223 {
    u8 pad_0[0x70];
    f32 unk70;
    u8 unk74;
    u8 unk75;
    u8 pad_76[0x2];
    struct M2c_var_19_223_unk78 * unk78;
    u8 pad_7C[0xC];
    struct M2c_temp_16_261 * unk88;
    u8 pad_8C[0xC];
    f32 unk98;
};

struct M2c_var_19_223_unk78 {
    u8 pad_0[0x18];
    u8 unk18;
    u8 unk19;
    u8 pad_1A[0x3];
};

extern struct M2c_D_0013E550 D_0013E550;
extern u8 D_0013E5C0[];
extern u8 D_0013E5E0[];
extern u8 D_0013E604[];
extern s32 D_0013F640[];
extern s32 D_0015ED6C[];
extern s32 D_0015F5E8[];
extern s32 D_0015F604;
extern s32 D_0015F60C;
extern u8 D_00187080[];
extern u8 D_00187290[];
extern s32 D_001872D4[];
extern void FUN_0022dd90();
extern void FUN_0022ddd8();
extern s32 ComputeByteStringHash();
extern s32 ComputeSectorIndex();
extern s32 FillTransferWords();
extern s32 ReadGlobalTableEntry();
extern s32 func_0012DC80();
extern s32 func_0012E208();
extern s32 func_0012E308();
extern s32 func_0012E368();
extern s32 func_0012E448();
extern s32 func_0012E4C0();
extern s32 func_0012EB00();
extern s32 func_0012EF68();
extern s32 func_0012EFE0();
extern s32 func_001F9A10();
extern s32 func_001F9A28();
extern s32 func_001F9A68();
extern s32 func_001F9AB0();
extern s32 func_001F9AF0();
extern s32 func_001F9BF8();
extern s32 func_001F9CF8();
extern s32 func_001FA2D8();
extern s32 func_001FA6C0();
extern s32 func_001FA6D0(f32);
extern s32 func_00216290();
extern s32 func_0022C5A8();
extern s32 func_0022C658();
extern s32 func_0022C7E8();
extern s32 func_0022C830();
s32 sound_update(void) __asm__("FUN_0022ca50");

s32 sound_update(void) {
u8 sp_slot[0x300];    s64 sp10;
    s64 sp20;
    s32 sp30;
    s32 spB0;
    s32 sp130;
    s64 sp1B0;
    s32 sp1C0;
    s32 sp200;
    s32 sp204;
    s32 *sp208;
    s64 *sp20C;
    s32 *sp210;
    s32 *sp214;
    s32 *sp218;
    s32 *sp21C;
    s32 *sp220;
    s32 *sp224;
    s32 sp228;
    s32 sp22C;
    s32 *sp230;
    s64 sp240;
    s32 *temp_4_847;
    struct M2c_var_19_223 *var_19_223;
    f32 var_f21_18;
    s64 *temp_16_340;
    s64 *var_23_230;
    register s32 * temp_5_848 asm("s3");
    s32 *var_20_216;
    register s32 * var_20_491 asm("s2");
    s32 *var_23_714;
    s32 *var_30_228;
    s32 *var_30_711;
    s32 temp_18_543;
    s32 temp_18_724;
    s32 temp_20_762;
    s32 temp_2_662;
    s32 temp_2_692;
    s32 temp_3_157;
    s32 temp_3_254;
    s32 temp_3_399;
    register u32 temp_4_145 asm("s1");
    s32 temp_4_463;
    s32 temp_4_716;
    u32 temp_4_85;
    register s32 temp_4_91 asm("s4");
    register s32 temp_5_105 asm("s1");
    u32 temp_8_480;
    register s32 var_16_114 asm("s0");
    s32 var_16_523;
    s32 var_16_542;
    s32 var_17_548;
    s32 var_17_766;
    s32 var_19_521;
    s32 var_19_769;
    register s32 var_20_116 asm("s0");
    register s32 var_21_707 asm("s5");
    register s32 var_22_220 asm("s0");
    s32 var_22_767;
    s32 var_2_302;
    s32 var_2_596;
    s32 var_2_619;
    register u32 var_3_568 asm("s5");
    s32 var_4_264;
    s32 var_5_123;
    s32 var_5_195;
    register s32 var_6_396 asm("s7");
    register s32 var_7_100 asm("s1");
    register u32 temp_hi_615 asm("s2");
    u8 *temp_2_580;
    register u8 * temp_3_559 asm("s5");
    u8 *var_4_567;
    u8 temp_3_265;
    u8 temp_3_277;
    u8 temp_3_310;
    struct M2c_temp_16_261 *temp_16_261;
    struct M2c_temp_16_383 *temp_16_383;
    struct M2c_temp_16_504 *temp_16_504;
    struct M2c_temp_16_722 *temp_16_722;
    struct M2c_temp_17_306 *temp_17_306;
    s32 *temp_22_546;
    register s32 * temp_30_545 asm("s5");
    struct M2c_temp_3_826 *temp_3_826;
    struct M2c_temp_4_809 *temp_4_809;
    s32 *temp_5_307;
    s32 *var_17_522;
    s32 *var_9_547;

    var_f21_18 = 0.0f;
    sp204 = D_001872D4[0];
    if (D_001872D4[0] == 0) {
        goto block_2;
    }
    var_f21_18 = D_0013F640[0];
block_2:
    func_0012DC80();
    if (!(D_0013E550.unk6B & 8)) {
        goto block_4;
    }
    func_0012EF68(2, 0U, 0, 0U, 0U);
    goto block_8;
block_4:
    if (!(D_0013E550.unk6B & 0x13)) {
        goto block_6;
    }
    func_0012EF68(2, D_0013E550.unk68, D_0013E550.unk64, D_0013E550.unk69, D_0013E550.unk6A);
    goto block_8;
block_6:
    if (!(D_0013E550.unk6B & 4)) {
        goto block_8;
    }
    func_0012EFE0(2, D_0013E550.unk64, 0xC, 3);
block_8:
    D_0013E550.unk6B = 0U;
    if (D_0015F604 == 0) {
        goto block_10;
    }
    if (D_0015F604 != 2) {
        goto block_12;
    }
block_10:
    if (((s32 *)0x15F5E8)[0] != 0) {
        goto block_12;
    }
    func_0022C5A8(sp_slot);
block_12:
    sp10 = 0;
    temp_4_85 = D_0013E550.unk40 + 1;
    temp_4_91 = temp_4_85 - (((s32) ((temp_4_85 > -1) ? temp_4_85 : (D_0013E550.unk40 + 4)) >> 2) * 4);
    D_0013E550.unk40 = temp_4_91;
    *(s32 *)((temp_4_91 * 0x10) + &D_0013E550) = *(s32 *)D_00187080;
    sp20C = &sp10;
    var_7_100 = temp_4_91;
    sp214 = &sp30;
    sp208 = &sp130;
    temp_5_105 = var_7_100 + 3;
    sp210 = &sp1C0;
    sp218 = &spB0;
    var_16_114 = temp_5_105 - (((s32) ((temp_5_105 > -1) ? temp_5_105 : (var_7_100 + 6)) >> 2) * 4);
    var_20_116 = 0;
    if (var_16_114 == var_7_100) {
        goto block_16;
    }
    var_5_123 = var_7_100 * 0x10;
loop_14:
    func_001F9A28(&sp20, var_5_123 + &D_0013E550, (var_16_114 * 0x10) + &D_0013E550, var_7_100);
    if (!(func_001F9AF0(&sp20) < (D_0015ED6C[0] * 60.0f))) {
        goto block_16;
    }
    var_20_116 += 1;
    func_001F9A10(sp20C, sp20C, &sp20);
    var_7_100 = var_16_114;
    temp_4_145 = var_7_100 + 3;
    var_16_114 = temp_4_145 - (((s32) ((temp_4_145 > -1) ? temp_4_145 : (var_7_100 + 6)) >> 2) * 4);
    var_5_123 = var_7_100 * 0x10;
    if (var_16_114 != D_0013E550.unk40) {
        goto loop_14;
    }
block_16:
    temp_3_157 = D_0015F604;
    if (var_20_116 < 2) {
        goto block_18;
    }
    func_001F9A68(sp20C, sp20C, 1.0f / func_001FA6C0(var_20_116));
block_18:
    if (temp_3_157 == 2) {
        goto block_20;
    }
    goto block_22;
block_20:
    func_0012E208(1, 0U);
    func_0012E208(0, (u32) ((s32) (D_0013E550.unk48 + ((u32) D_0013E550.unk48 >> 0x1F)) >> 1));
    func_0012E208(3, (u32) ((s32) (D_0013E550.unk54 + ((u32) D_0013E550.unk54 >> 0x1F)) >> 1));
    goto block_25;
block_22:
    var_5_195 = D_0013E550.unk4C;
    if (sp204 == 0) {
        goto block_24;
    }
    var_5_195 = (s32) (var_5_195 * 3) / 5;
block_24:
    func_0012E208(1, (u32) var_5_195);
    func_0012E208(0, D_0013E550.unk48);
    func_0012E208(3, D_0013E550.unk54);
block_25:
    var_20_216 = sp214;
    var_22_220 = 0;
    sp200 = 0;
    var_19_223 = &D_0013E550;
    func_0012E208(2, D_0013E550.unk50);
    var_30_228 = ((s32 *)&spB0);
    var_23_230 = D_0013E5E0;
    sp22C = 0;
    func_0012E208(4, D_0013E550.unk58);
    func_0012E208(5, D_0013E550.unk5C);
    FillTransferWords(sp214, 0, 0x78);
    FillTransferWords(((s32 *)&spB0), 0, 0x78);
    FillTransferWords(sp208, 0, 0x78);
loop_26:
    if (var_19_223->unk74 == 7) {
        goto block_29;
    }
    temp_3_254 = var_19_223->unk70;
    if (temp_3_254 == 0) {
        goto block_68;
    }
    if (temp_3_254 == 0xFFFFFFFF) {
        goto block_68;
    }
block_29:
    temp_16_261 = var_19_223->unk88;
    var_4_264 = 0;
    if (temp_16_261 == NULL) {
        goto block_33;
    }
    temp_3_265 = temp_16_261->unk20;
    if (temp_3_265 == 0xFE) {
        goto block_32;
    }
    if (temp_3_265 != 0xFD) {
        goto block_33;
    }
block_32:
    var_4_264 = 1;
block_33:
    if (var_4_264 == 0) {
        goto block_35;
    }
    var_19_223->unk88 = NULL;
block_35:
    temp_3_277 = var_19_223->unk74;
    if (temp_3_277 == 4) {
        goto block_39;
    }
    if (temp_3_277 == 6) {
        goto block_40;
    }
    if (var_4_264 == 0) {
        goto block_40;
    }
    if (var_19_223->unk78->unk18 == 0) {
        goto block_40;
    }
block_39:
    *var_20_216 = 0x20;
    goto block_67;
block_40:
    if (D_0015F604 == 0) {
        goto block_44;
    }
    if (D_0015F604 == 2) {
        goto block_44;
    }
    if (D_0015F604 == 6) {
        goto block_44;
    }
    var_2_302 = 0x10;
    if (var_19_223->unk74 != 7) {
        goto block_66;
    }
block_44:
    temp_17_306 = var_22_220 + &D_0013E550;
    temp_5_307 = temp_17_306->unk88;
    if (temp_5_307 == NULL) {
        goto block_49;
    }
    temp_3_310 = temp_17_306->unk75;
    if (temp_3_310 & 8) {
        goto block_49;
    }
    if (!(temp_3_310 & 0x40)) {
        goto block_48;
    }
    func_001F9CF8(&sp1B0, var_22_220 + (((u8 *)&D_0013E550) + 0xA0), ((u8 *)temp_16_261) + 0xC0);
    func_001F9A10(&sp1B0, &sp1B0, ((u8 *)temp_16_261) + 0x10);
    func_001F9A28(&sp20, &sp1B0, var_23_230);
    *var_23_230 = sp1B0;
    goto block_50;
block_48:
    temp_16_340 = var_22_220 + D_0013E5E0;
    temp_17_306->unk98 = (f32)(u32)(temp_17_306->unk98 - 1.0f);
    func_001F9A28(&sp20, temp_5_307 + 0x10, temp_16_340);
    *temp_16_340 = temp_17_306->unk88->unk10;
    temp_17_306->unk98 = (f32)(u32)(temp_17_306->unk98 + 1.0f);
    goto block_50;
block_49:
    sp20 = 0;
block_50:
    func_001F9A28(&sp20, &sp20, sp20C);
    func_001F9A28(&sp1B0, D_00187080, var_23_230);
    func_001F9BF8(&sp1B0, &sp1B0, 0x3F800000);
    temp_16_383 = var_22_220 + (((u8 *)D_0013E5E0 - (u8 *)0x90));
    *(s32 *)((u8 *)sp208 + sp22C) = func_001F9AB0(&sp1B0, &sp20);
    if (temp_16_383->unk75 & 0x10) {
        goto block_53;
    }
    var_6_396 = func_0022C7E8(var_22_220 + (((u8 *)D_0013E5E0 - (u8 *)0x20)), var_23_230);
    temp_3_399 = var_6_396 * (u32)temp_16_383->unk80;
    *var_30_228 = (s32) ((temp_3_399 > -1) ? temp_3_399 : (temp_3_399 + 0x3FF)) >> 0xA;
    if (temp_16_383->unk78->unk19 & 2) {
        goto block_55;
    }
    *var_20_216 |= 8;
    goto block_54;
block_53:
    var_6_396 = 0x400;
    *(s32 *)((u8 *)((s32 *)&spB0) + sp22C) = temp_16_383->unk80;
block_54:
block_55:
    if (sp204 == 0) {
        goto block_59;
    }
    if (var_19_223->unk78->unk19 & 4) {
        goto block_59;
    }
    if (!(var_f21_18 < var_19_223->unk98)) {
        goto block_59;
    }
    *var_30_228 = (s32) *var_30_228 / 2;
block_59:
    *var_20_216 |= 1;
    if (var_6_396 >= 0x20) {
        goto block_63;
    }
    if (*(s32 *)((u8 *)((s32 *)&spB0) + sp22C) >= 0x20) {
        goto block_63;
    }
    if (!(var_19_223->unk75 & 4)) {
        goto block_63;
    }
    *var_20_216 = 0x20;
    goto block_67;
block_63:
    if (!((var_19_223->unk75 ^ 1) & 1)) {
        goto block_68;
    }
    temp_4_463 = *var_20_216;
    *var_20_216 = temp_4_463 | 2;
    if (var_19_223->unk75 & 0x20) {
        goto block_67;
    }
    var_2_302 = temp_4_463 | 6;
block_66:
    *var_20_216 = var_2_302;
block_67:
block_68:
    var_20_216 += 4;
    var_19_223 += 0x70;
    var_30_228 += 4;
    temp_8_480 = sp200 + 1;
    sp22C += 4;
    sp200 = temp_8_480;
    var_23_230 += 0x70;
    var_22_220 += 0x70;
    if (temp_8_480 < 0x1E) {
        goto loop_26;
    }
    sp200 = 0;
    var_20_491 = ((s32 *)&spB0);
    sp224 = D_0013E604;
    sp228 = 0;
    sp230 = sp214;
loop_70:
    if (!(*sp230 & 8)) {
        goto block_100;
    }
    temp_16_504 = sp228 + &D_0013E550;
    if (temp_16_504->unk74 != 7) {
        goto block_86;
    }
    if (D_0015F604 == 0) {
        goto block_74;
    }
    if (D_0015F604 != 2) {
        goto block_101;
    }
block_74:
    if (((s32 *)0x15F5E8)[0] != 0) {
        goto block_101;
    }
    var_19_521 = 0;
    if (*(s32 *)0x15F60C == D_0013E550.unkD98) {
        goto block_79;
    }
    var_17_522 = ((u8 *)&D_0013E550) + 0xDA0;
    var_16_523 = 5;
loop_77:
    var_16_523 -= 1;
    func_0022C5A8(var_17_522);
    var_17_522 += 0x10;
    if (var_16_523 >= 0) {
        goto loop_77;
    }
    D_0013E550.unkD98 = (s32) *(s32 *)0x15F60C;
block_79:
    var_16_542 = 0;
    temp_18_543 = sp200 * 0x70;
    temp_30_545 = ((u8 *)D_0013E5C0 - (u8 *)0x70);
    temp_22_546 = D_0013E5C0 + 0x44;
    var_9_547 = temp_30_545 + 0xDA0;
    var_17_548 = 0;
loop_80:
    sp240 = (s64) var_9_547;
    temp_3_559 = var_16_542 + temp_18_543 + temp_22_546;
    *temp_3_559 = func_0022C658(temp_18_543 + D_0013E5C0, (var_17_548 * 0x10) + var_9_547);
    __asm__ volatile ("" : "+r" (temp_3_559));
    var_4_567 = var_16_542 + (temp_18_543 + 0xB4) + temp_30_545 + 1;
    var_3_568 = 4;
loop_81:
    var_3_568 -= 1;
    *var_4_567 = *temp_3_559;
    var_4_567 += 1;
    if (var_3_568 >= 0) {
        goto loop_81;
    }
    var_17_548 += 1;
    temp_2_580 = var_16_542 + temp_18_543 + temp_22_546;
    var_16_542 += 6;
    var_19_521 = (*temp_2_580 != 0) ? (var_19_521 + 6) : var_19_521;
    if (var_16_542 < 0x24) {
        goto loop_80;
    }
    if (var_19_521 >= 0x24) {
        goto block_96;
    }
    if (var_19_521 < 0x13) {
        goto block_100;
    }
    var_2_596 = (0x24 - var_19_521) * *var_20_491;
    goto block_99;
block_86:
    if (D_0015F604 == 0) {
        goto block_88;
    }
    if (D_0015F604 != 2) {
        goto block_95;
    }
block_88:
    if (((s32 *)0x15F5E8)[0] != 0) {
        goto block_95;
    }
    temp_hi_615 = (u32) (temp_16_504->unkB0 + 1) % 36U;
    var_2_619 = (sp200 ^ *(s32 *)0x15F60C) & 1;
    temp_16_504->unkB0 = temp_hi_615;
    if (temp_16_504->unk75 & 4) {
        goto block_91;
    }
    var_2_619 = ((*(s32 *)sp200 ^ 0x15F60C) & 3) == 0;
block_91:
    if (var_2_619 == 0) {
        goto block_93;
    }__asm__ volatile ("" : : "r" (&D_0013E550));
    
    *(temp_16_504->unkB0 + sp228 + D_0013E604) = func_0022C658(sp228 + (((u8 *)&D_0013E550) + 0x70), sp_slot, 0x24, &D_0013E550, sp200);
    goto block_94;
block_93:
    *(s32 *)(temp_hi_615 + sp228 + D_0013E604) = *(s32 *)(((u32) (temp_hi_615 + 0x23) % 36U) + sp228 + D_0013E604);
block_94:
block_95:
    temp_2_662 = ComputeByteStringHash(sp224, 0x24);
    if (temp_2_662 < 0x24) {
        goto block_97;
    }
block_96:
    *var_20_491 = 0;
    goto block_100;
block_97:
    if (temp_2_662 < 0x13) {
        goto block_101;
    }
    var_2_596 = (0x24 - temp_2_662) * *var_20_491;
block_99:
    *var_20_491 = var_2_596 / 18;
block_100:
block_101:
    var_20_491 += 4;
    temp_2_692 = sp200 + 1;
    sp200 = temp_2_692;
    sp224 += 0x70;
    sp228 += 0x70;
    sp230 += 4;
    if (temp_2_692 < 0x1E) {
        goto loop_70;
    }
    var_21_707 = 0;
    func_001FA2D8(((s32 *)&sp1C0), D_00187290);
    var_30_711 = sp214;
    sp220 = sp208;
    sp21C = D_0013E5C0;
    var_23_714 = ((s32 *)&spB0);
loop_103:
    temp_4_716 = *var_30_711;
    if (temp_4_716 == 0) {
        goto block_121;
    }
    temp_16_722 = var_21_707 + &D_0013E550;
    temp_18_724 = temp_16_722->unk70;
    temp_16_722->unk70 = 0xFFFFFFFF;
    if (!(temp_4_716 & 0x20)) {
        goto block_108;
    }
    if (temp_16_722->unk74 != 7) {
        goto block_107;
    }
    temp_16_722->unk74 = 0U;
    temp_16_722->unk88 = 0;
    temp_16_722->unk8C = 0;
    goto block_121;
block_107:
    func_0012E368(temp_18_724, &D_0013E550);
    temp_16_722->unk74 = 6U;
    func_0012E448(temp_18_724, FUN_0022ddd8, var_21_707 + D_0013E5C0);
    goto block_121;
block_108:
    if (!(temp_4_716 & 0x10)) {
        goto block_110;
    }
    func_0012E448(temp_18_724, FUN_0022ddd8, var_21_707 + D_0013E5C0);
    goto block_121;
block_110:
    temp_20_762 = temp_16_722->unk84;
    var_17_766 = (temp_20_762 != 0) ? 0x11 : 1;
    var_22_767 = 0;
    var_19_769 = 0;
    if (!(temp_4_716 & 2)) {
        goto block_112;
    }
    var_17_766 |= 6;
    var_22_767 = func_0022C830(var_21_707 + D_0013E5C0, var_21_707 + (((u8 *)&D_0013E550) + 0x90), ((s32 *)&sp1C0));
block_112:
    if (!(*var_30_711 & 4)) {
        goto block_114;
    }
    var_17_766 |= 8;
    var_19_769 = ComputeSectorIndex(func_001FA6D0((f32) *sp220 * 300.0f));
block_114:
    if (sp204 == 0) {
        goto block_117;
    }
    if (temp_16_722->unk78->unk19 & 8) {
        goto block_118;
    }
    var_17_766 |= 8;
    var_19_769 -= 0x5F4;
block_117:
block_118:
    temp_4_809 = var_21_707 + &D_0013E550;
    if (temp_4_809->unk74 == 7) {
        goto block_120;
    }
    func_0012E4C0(temp_18_724, var_17_766, *var_23_714, var_22_767, var_19_769, temp_20_762, FUN_0022ddd8, sp21C);
    goto block_121;
block_120:
    temp_3_826 = temp_4_809->unk78;
    temp_4_809->unk74 = 1U;
    func_0012E308(temp_3_826->unk1C, temp_3_826->unk1A, *var_23_714, var_22_767, var_19_769, temp_20_762, FUN_0022dd90, var_21_707 + D_0013E5C0);
block_121:
    var_23_714 += 4;
    var_21_707 += 0x70;
    temp_4_847 = sp21C + 0x70;
    temp_5_848 = sp220 + 4;
    sp21C = temp_4_847;
    sp220 = temp_5_848;
    var_30_711 += 4;
    if ((s32) var_23_714 < (s32) (((s32 *)&spB0) + 0x78)) {
        goto loop_103;
    }
    func_00216290(temp_4_847, temp_5_848);
    func_0012EB00();
    func_0012DC80();
    ReadGlobalTableEntry();
    D_0013E550.unk6C = 0;
    return 0;
}

extern s32 func_0022CA50(void) __attribute__((alias("FUN_0022ca50")));
#endif /* NON_MATCHING */
