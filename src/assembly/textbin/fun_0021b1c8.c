#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021b1c8/FUN_0021b1c8.s", FUN_0021b1c8);
#else
#include "rnc/fun_0021b1c8_types.h"
#include "types.h"






extern s32 D_001601B8;
extern s32 D_001601BC;
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];
extern void func_001F4280();
extern void func_001F4398();
extern s32 func_001F44B8();
extern void func_001F61E8();
extern void func_001F61F8();
extern s32 func_001F6200();
extern void func_001F62B0();
extern s32 func_001FDD10();
extern s32 func_0021B6D8();
extern void func_00233980(s32, s64);
s32 FUN_0021b1c8(struct M2c_arg0 *arg0) {
u8 *sp0;
    s32 *sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s16 *temp_5_118;
    s16 *temp_5_99;
    s16 *var_2_105;
    s16 *var_4_55;
    s32 temp_16_237;
    s32 temp_16_283;
    s32 temp_16_309;
    s32 temp_17_235;
    s32 temp_17_281;
    s32 temp_18_286;
    s32 temp_2_113;
    s32 temp_2_70;
    s32 temp_3_126;
    s32 temp_3_216;
    s32 temp_4_169;
    s32 temp_5_20;
    s32 temp_5_260;
    s32 temp_5_311;
    s32 var_16_104;
    s32 var_16_12;
    s32 var_17_103;
    s32 var_20_200;
    s32 var_21_217;
    s32 var_22_98;
    s32 var_23_153;
    s32 var_3_59;
    s32 var_4_131;
    s32 var_5_157;
    s32 var_6_74;
    s64 var_18_172;
    struct M2c_temp_2_165 *temp_2_165;

    var_16_12 = 0xC;
    temp_5_20 = arg0->unk30;
    sp4 = D_001DF050;
    sp8 = 1;
    sp0 = D_001D5BF4[0]->unk40 == (s32) arg0;
    if (!(temp_5_20 & 4)) {
        goto block_2;
    }
    sp8 = 3;
    sp4 = D_001DF790;
    var_16_12 = 0xE;
block_2:
    if (!(temp_5_20 & 8)) {
        goto block_4;
    }
    sp8 = 2;
    sp4 = D_001DF3F0;
    var_16_12 = 0xA;
block_4:
    func_00233980(0x42, 0x44);
    func_00233980(0x47, 0x2004B);
    func_001F4280(0);
    var_4_55 = arg0->unk34;
    var_3_59 = 0;
    if (*var_4_55 == 0) {
        goto block_7;
    }
loop_6:
    var_4_55 += 0xC;
    var_3_59 += 1;
    if (*var_4_55 != 0) {
        goto loop_6;
    }
block_7:
    temp_2_70 = arg0->unk30;
    var_6_74 = var_16_12 + 3;
    if (!(temp_2_70 & 0x10)) {
        goto block_9;
    }
    spC = var_6_74;
    goto block_10;
block_9:
    spC = (s32) arg0->unk24 / (s32) (var_3_59 + 1);
block_10:
    sp10 = (s32) arg0->unk20 >> 1;
    sp14 = 0;
    var_22_98 = (spC - ((s32) (var_16_12 + ((u32) (var_16_12 >> 0x1F) >> 0x1F)) >> 1)) - 1;
    if (!(temp_2_70 & 0x4000)) {
        goto block_14;
    }
    temp_5_99 = arg0->unk34;
    if (*temp_5_99 == 0) {
        goto block_14;
    }
    var_17_103 = 0;
    var_16_104 = 0;
    var_2_105 = temp_5_99;
loop_13:
    var_17_103 += 0xC;
    var_16_104 += 0xC;
    temp_2_113 = func_001F6200(func_001FDD10(*var_2_105, arg0->unk34, var_6_74), -1, sp4);
    temp_5_118 = arg0->unk34;
    var_6_74 = (sp14 < temp_2_113) ? temp_2_113 : sp14;
    sp14 = var_6_74;
    var_2_105 = var_16_104 + temp_5_118;
    if (*(s32 *)(var_17_103 + temp_5_118) != 0) {
        goto loop_13;
    }
block_14:
    temp_3_126 = arg0->unk30;
    var_4_131 = arg0->unk20;
    if (!(temp_3_126 & 0x20000)) {
        goto block_18;
    }
    if (var_4_131 >= (sp14 + 6)) {
        goto block_18;
    }
    if (temp_3_126 & 8) {
        goto block_19;
    }
    arg0->unk30 = (s32) (temp_3_126 | 8);
    return 1;
block_18:
block_19:
    sp18 = 0;
    sp14 = (var_4_131 < sp14) ? var_4_131 : sp14;
    var_23_153 = 0;
    if (*(s32 *)arg0->unk34 == 0) {
        goto block_46;
    }
loop_21:
    var_5_157 = 0;
    if (sp0 == 0) {
        goto block_23;
    }
    var_5_157 = arg0->unk40 == sp18;
block_23:
    temp_2_165 = var_23_153 + arg0->unk34;
    temp_4_169 = temp_2_165->unk2 != 0;
    if (!(arg0->unk30 & 2)) {
        goto block_25;
    }
    var_18_172 = 0x80FFA888;
    goto block_31;
block_25:
    if (var_5_157 == 0) {
        goto block_28;
    }
    if (temp_4_169 != 0) {
        goto block_29;
    }
    var_18_172 = 0x80006060;
    goto block_31;
block_28:
    if (temp_4_169 == 0) {
        goto block_30;
    }
block_29:
    var_18_172 = func_0021B6D8(temp_2_165->unkA, -1, -1);
    __asm__ volatile ("" : "+r" (var_18_172));
    goto block_31;
block_30:
    var_18_172 = 0x80303030;
block_31:
    var_20_200 = func_001FDD10(*(s32 *)(var_23_153 + arg0->unk34));
    if (*(s32 *)((u8 *)(var_23_153 + arg0->unk34) + 0x2) != 2) {
        goto block_33;
    }
    var_20_200 = func_001FDD10(0x4F54);
block_33:
    temp_3_216 = arg0->unk30;
    var_21_217 = sp10 - (func_001F6200(var_20_200, -1, sp4) >> 1);
    if (!(temp_3_216 & 0x40)) {
        goto block_35;
    }
    var_21_217 = 4;
    goto block_37;
block_35:
    if (!(temp_3_216 & 0x4000)) {
        goto block_37;
    }
    var_21_217 = sp10 - (sp14 >> 1);
block_37:
    func_001F61F8(sp10);
    temp_17_235 = var_21_217 + D_001601B8;
    temp_16_237 = var_22_98 + D_001601BC;
    func_001F62B0(temp_17_235, temp_16_237, 0x8000 << 0x10, var_20_200, -1, func_001F44B8(sp8), sp4);
    func_001F61E8();
    if (!(arg0->unk30 & 0x80)) {
        goto block_39;
    }
    func_001F61F8(sp8);
block_39:
    temp_5_260 = var_22_98;
    var_22_98 += spC;
    func_001F62B0(var_21_217, temp_5_260, var_18_172, var_20_200, -1, func_001F44B8(sp8), sp4);
    if (*(s32 *)((u8 *)(var_23_153 + arg0->unk34) + 0x8) == 0) {
        goto block_43;
    }
    func_001F61F8();
    temp_17_281 = var_21_217 + D_001601B8;
    temp_16_283 = var_22_98 + D_001601BC;
    temp_18_286 = func_001FDD10(*(s32 *)((u8 *)(var_23_153 + arg0->unk34) + 0x8));
    func_001F62B0(temp_17_281, temp_16_283, 0x8000 << 0x10, temp_18_286, -1, func_001F44B8(sp8), sp4);
    if (arg0->unk30 & 0x80) {
        goto block_42;
    }
    func_001F61E8();
block_42:
    temp_16_309 = func_001FDD10(*(s32 *)((u8 *)(var_23_153 + arg0->unk34) + 0x8));
    temp_5_311 = var_22_98;
    var_22_98 += spC;
    func_001F62B0(var_21_217, temp_5_311, var_18_172, temp_16_309, -1, func_001F44B8(sp8), sp4);
block_43:
    if (!(arg0->unk30 & 0x80)) {
        goto block_45;
    }
    func_001F61E8(sp18);
block_45:
    var_23_153 += 0xC;
    var_4_131 = sp18 + 1;
    sp18 = var_4_131;
    if (*(s32 *)(var_23_153 + arg0->unk34) != 0) {
        goto loop_21;
    }
block_46:
    func_001F4398(var_4_131);
    return 2;
}

extern s32 func_0021B1C8(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0021b1c8")));
#endif /* NON_MATCHING */
