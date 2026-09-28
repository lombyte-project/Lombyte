#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/input/pad/process_pad_input/FUN_00217328.s", FUN_00217328);
#else
#include "rnc/input_pad_process_pad_input_types.h"
#include "types.h"






extern u32 D_0015EDB4[];
extern s32 func_001F96F8();
extern s32 func_001F9B20();
extern s32 func_001F9E90();
extern s32 func_001FA688();
extern s32 func_001FA6C0();
extern void arg0();
void process_pad_input(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1, s32 arg2) __asm__("FUN_00217328");

void process_pad_input(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1, s32 arg2) {
u8 sp_slot[0xE0];    f32 sp0;
    f32 sp4;
    f32 *sp10;
    s32 *sp14;
    s32 *sp18;
    f32 *temp_21_15;
    f32 *var_17_51;
    f32 *var_17_97;
    f32 *var_2_140;
    f32 *var_3_112;
    f32 temp_f0_143;
    f32 temp_f0_384;
    f32 temp_f20_338;
    f32 temp_f20_65;
    f32 temp_f20_68;
    f32 temp_f21_342;
    s32 *var_3_28;
    s32 temp_2_27;
    s32 temp_3_116;
    s32 temp_3_205;
    s32 temp_3_252;
    s32 temp_3_269;
    s32 temp_3_444;
    s32 temp_3_455;
    s32 temp_4_209;
    s32 temp_4_215;
    s32 temp_4_463;
    s32 temp_5_207;
    s32 temp_5_56;
    s32 temp_5_59;
    s32 temp_7_213;
    s32 temp_8_203;
    s32 var_17_359;
    s32 var_17_399;
    s32 var_18_11;
    s32 var_18_141;
    s32 var_18_49;
    s32 var_18_96;
    s32 var_2_126;
    s32 var_2_242;
    s32 var_2_260;
    s32 var_2_277;
    s32 var_5_212;
    u8 temp_2_462;
    u8 temp_4_100;
    u8 temp_5_452;
    s32 *temp_30_45;
    struct M2c_var_19_52 *var_19_52;

    var_18_11 = 0xF;
    temp_21_15 = ((u8 *)arg0 + (0x100));
    temp_2_27 = ((arg1->unk0 << 8) | arg1->unk1) ^ 0xFFFF;
    var_3_28 = ((u8 *)arg0 + (0x13C));
    arg0->unk1B0 = temp_2_27;
    arg0->unk1A0 = temp_2_27;
loop_1:
    *var_3_28 = 0;
    var_18_11 -= 1;
    var_3_28 -= 4;
    if (var_18_11 >= 0) {
        goto loop_1;
    }
    sp10 = ((u8 *)arg0 + (0x140));
    temp_30_45 = ((u8 *)arg0 + (0x2D0));
    sp14 = ((u8 *)arg0 + (0x258));
    sp18 = ((u8 *)arg0 + (0x1E0));
    if (arg2 < 6) {
        goto block_12;
    }
    var_18_49 = 0;
    var_17_51 = temp_21_15;
    var_19_52 = arg1;
loop_4:
    temp_5_56 = var_19_52->unk2 - 0x7F;
    temp_5_59 = (temp_5_56 <= -1) ? (0 - temp_5_56) : temp_5_56;
    if (temp_5_59 >= 0x30) {
        goto block_6;
    }
    var_18_49 += 1;
    goto block_11;
block_6:
    temp_f20_65 = func_001FA6C0(temp_5_59 - 0x30, temp_5_59);
    temp_f20_68 = temp_f20_65 / func_001FA6C0(0x4CU);
    *var_17_51 = temp_f20_68;
    if (!(temp_f20_68 > 1.0f)) {
        goto block_9;
    }
    *var_17_51 = 1.0f;
block_9:
    var_18_49 += 1;
    if ((u8) var_19_52->unk2 >= 0x7FU) {
        goto block_11;
    }
    *var_17_51 = -*var_17_51;
block_11:
    var_17_51 += 4;
    var_19_52 += 1;
    if (var_18_49 < 4) {
        goto loop_4;
    }
block_12:
    if (arg2 < 0x12) {
        goto block_15;
    }
    var_18_96 = 4;
    var_17_97 = temp_21_15 + 0x10;
loop_14:
    temp_4_100 = *(s32 *)((u8 *)(((u8 *)arg1 + (var_18_96))) + 0x2);
    var_18_96 += 1;
    *var_17_97 = func_001FA6C0(temp_4_100) * 0.003921569f;
    var_17_97 += 4;
    if (var_18_96 < 0x10) {
        goto loop_14;
    }
block_15:
    var_3_112 = sp10;
    if (D_0015EDB4[0] == 0) {
        goto block_22;
    }
    temp_3_116 = arg0->unk1A0;
    arg0->unk108 = (f32) -arg0->unk108;
    arg0->unk100 = (f32) -arg0->unk100;
    if (!(temp_3_116 & 0x8000)) {
        goto block_18;
    }
    var_2_126 = (temp_3_116 & 0xFFFF7FFF) | 0x2000;
    goto block_20;
block_18:
    if (!(temp_3_116 & 0x2000)) {
        goto block_21;
    }
    var_2_126 = (temp_3_116 & ~0x2000) | 0x8000;
block_20:
    arg0->unk1A0 = var_2_126;
block_21:
    arg0->unk1B0 = (s32) arg0->unk1A0;
    var_3_112 = sp10;
block_22:
    var_2_140 = temp_21_15;
    var_18_141 = 0xF;
loop_23:
    temp_f0_143 = *var_2_140;
    var_18_141 -= 1;
    var_2_140 += 4;
    *var_3_112 = temp_f0_143;
    var_3_112 += 4;
    if (var_18_141 >= 0) {
        goto loop_23;
    }
    if (arg0->unk108 != 0.0f) {
        goto block_28;
    }
    if (arg0->unk10C != 0.0f) {
        goto block_27;
    }
    arg0->unk1D8 = 0;
    goto block_29;
block_27:
block_28:
    arg0->unk1D8 = 1;
block_29:
    if (!(arg0->unk108 < 0.0f)) {
        goto block_31;
    }
    arg0->unk1A0 = (s32) (arg0->unk1A0 | 0x8000);
block_31:
    if (!(arg0->unk108 > 0.0f)) {
        goto block_33;
    }
    arg0->unk1A0 = (s32) (arg0->unk1A0 | 0x2000);
block_33:
    if (!(arg0->unk10C < 0.0f)) {
        goto block_35;
    }
    arg0->unk1A0 = (s32) (arg0->unk1A0 | 0x1000);
block_35:
    if (!(arg0->unk10C > 0.0f)) {
        goto block_37;
    }
    arg0->unk1A0 = (s32) (arg0->unk1A0 | 0x4000);
block_37:
    temp_8_203 = arg0->unk1A0;
    temp_3_205 = arg0->unk1AC;
    temp_5_207 = ~temp_8_203;
    temp_4_209 = ~temp_3_205;
    var_5_212 = temp_5_207 & temp_3_205;
    temp_7_213 = temp_4_209 & arg0->unk1B0;
    temp_4_215 = temp_4_209 & temp_8_203;
    arg0->unk1D4 = (s32) ((temp_8_203 & 0xF000) == 0);
    arg0->unk1D0 = (s32) (temp_8_203 == 0);
    arg0->unk1B4 = temp_7_213;
    arg0->unk1B8 = (s32) (temp_5_207 & arg0->unk1BC);
    arg0->unk1C4 = temp_4_215;
    arg0->unk1C8 = var_5_212;
    arg0->unk1A4 = temp_4_215;
    arg0->unk1A8 = var_5_212;
    arg0->unk1C0 = temp_8_203;
    arg0->unk348 = temp_8_203;
    if (D_0015EDB4[0] == 0) {
        goto block_58;
    }
    arg0->unk148 = (f32) -arg0->unk148;
    arg0->unk140 = (f32) -arg0->unk140;
    if (!(temp_8_203 & 0x8000)) {
        goto block_40;
    }
    var_2_242 = (temp_8_203 & 0xFFFF7FFF) | 0x2000;
    goto block_42;
block_40:
    if (!(temp_8_203 & 0x2000)) {
        goto block_43;
    }
    var_2_242 = (temp_8_203 & ~0x2000) | 0x8000;
block_42:
    arg0->unk1C0 = var_2_242;
block_43:
    temp_3_252 = arg0->unk1C4;
    if (temp_3_252 & 0x8000) {
        goto block_45;
    }
    goto block_47;
block_45:
    var_2_260 = (temp_3_252 & 0xFFFF7FFF) | 0x2000;
    goto block_49;
block_47:
    if (!(temp_3_252 & 0x2000)) {
        goto block_50;
    }
    var_2_260 = (temp_3_252 & ~0x2000) | 0x8000;
block_49:
    arg0->unk1C4 = var_2_260;
block_50:
    temp_3_269 = arg0->unk1C8;
    if (temp_3_269 & 0x8000) {
        goto block_52;
    }
    goto block_55;
block_52:
    var_2_277 = (temp_3_269 & 0xFFFF7FFF) | 0x2000;
    goto block_57;
block_54:
    arg0->unk1A4 = (s32) (arg0->unk1A4 | 0x10000);
    goto block_68;
block_55:
    if (!(temp_3_269 & 0x2000)) {
        goto block_58;
    }
    var_2_277 = (temp_3_269 & ~0x2000) | 0x8000;
block_57:
    arg0->unk1C8 = var_2_277;
block_58:
    if (arg0->unk1CC != 1) {
        goto block_60;
    }
    arg0->unk1A0 = (s32) (arg0->unk1A0 & ~0x5030);
    var_5_212 = arg0->unk1A8 & ~0x5030;
    arg0->unk1A4 = (s32) (arg0->unk1A4 & ~0x5030);
    arg0->unk1A8 = var_5_212;
    arg0->unk100 = 0.0f;
    arg0->unk104 = 0;
    arg0->unk1CC = 0;
block_60:
    if (arg0->unk1CC != 2) {
        goto block_62;
    }
    var_5_212 = arg0->unk1A8 & 0x900;
    arg0->unk1A0 = (s32) (arg0->unk1A0 & 0x900);
    arg0->unk1A4 = (s32) (arg0->unk1A4 & 0x900);
    arg0->unk1A8 = var_5_212;
    arg0->unk1B0 = (s32) (arg0->unk1B0 & 0x900);
    arg0->unk1D4 = 1;
    arg0->unk108 = 0.0f;
    arg0->unk10C = 0.0f;
    arg0->unk1CC = 0;
block_62:
    sp0 = arg0->unk108;
    sp4 = arg0->unk10C;
    temp_f20_338 = func_001F9B20(sp_slot, var_5_212, 1, temp_7_213, temp_8_203);
    temp_f21_342 = func_001F9E90(sp0, sp4);
    *(temp_30_45 + (arg0->unk18E * 4)) = temp_f20_338;
    *(sp14 + (arg0->unk18E * 4)) = temp_f21_342;
    if (!(temp_f20_338 > 0.9f)) {
        goto block_75;
    }
    var_17_359 = 1;
    goto loop_66;
block_65:
    var_17_359 += 1;
    if (temp_f0_384 < 0.25f) {
        goto block_54;
    }
loop_66:
    if (var_17_359 >= func_001F96F8(4)) {
        goto block_68;
    }
    temp_f0_384 = *(temp_30_45 + (((s32) ((arg0->unk18E - var_17_359) + 0x1E) % 30) * 4));
    if (!(temp_f0_384 > 0.9f)) {
        goto block_65;
    }
block_68:
    if (arg0->unk1A4 & 0x10000) {
        goto block_75;
    }
    var_17_399 = 1;
loop_70:
    if (var_17_399 >= func_001F96F8(5)) {
        goto block_76;
    }
    if (func_001FA688(*(sp14 + (((s32) ((arg0->unk18E - var_17_399) + 0x1E) % 30) * 4)), temp_f21_342) > 0.9599311f) {
        goto block_73;
    }
    var_17_399 += 1;
    goto loop_70;
block_73:
    arg0->unk1A4 = (s32) (arg0->unk1A4 | 0x10000);
block_75:
block_76:
    *(sp18 + (arg0->unk18E * 4)) = arg0->unk1A4;
    temp_3_444 = arg0->unk190 + 1;
    arg0->unk190 = temp_3_444;
    arg0->unk18E = (s16) ((s32) (arg0->unk18E + 1) % 30);
    if (temp_3_444 < 0x1F) {
        goto block_78;
    }
    arg0->unk190 = 0x1E;
block_78:
    temp_5_452 = arg0->unk34E;
    if (temp_5_452 == 0) {
        goto block_90;
    }
    temp_3_455 = arg0->unk1A0;
    if (temp_3_455 != 0) {
        goto block_81;
    }
    goto block_88;
block_81:
    if (temp_3_455 == arg0->unk1AC) {
        goto block_84;
    }
    goto block_88;
block_84:
    temp_2_462 = arg0->unk34F - 1;
    temp_4_463 = temp_2_462 & 0xFF;
    arg0->unk34F = temp_2_462;
    if (temp_4_463 == 0) {
        goto block_87;
    }
    if (temp_4_463 != 0xFF) {
        goto block_91;
    }
block_87:
    arg0->unk1A4 = temp_3_455;
    arg0->unk34F = temp_5_452;
    goto block_89;
block_88:
    arg0->unk34F = arg0->unk34D;
block_89:
block_90:
block_91:
    return;
}
#endif /* NON_MATCHING */
