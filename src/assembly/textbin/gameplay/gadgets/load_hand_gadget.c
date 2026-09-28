#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/gadgets/load_hand_gadget/FUN_00224368.s", FUN_00224368);
#else
#include "rnc/gameplay_gadgets_load_hand_gadget_types.h"
#include "types.h"


















































extern struct M2c_D_0013D4C0 D_0013D4C0;
extern u8 D_0013E520[];
extern struct M2c_D_0013F350 D_0013F350;
extern s32 D_0015FF50[];
extern u8 D_001863D0[];
extern u8 D_001B3200[];
extern u8 D_001B3AC0[];
extern u8 D_001D52E8[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern struct M2c_D_001D5DD0 D_001D5DD0;
extern struct M2c_D_001D5E10 D_001D5E10;
extern struct M2c_D_001D5E50 D_001D5E50;
extern u8 D_001D5E90[];
extern u8 D_001D5EA8[];
extern s32 func_001E9470();
extern s32 func_001E9478();
extern void func_00204A40();
extern s32 func_0020CB10();
extern s32 func_0020CB88();
extern s32 func_00212F90();
extern s32 func_00225490();
extern s32 func_00225530();
extern s32 func_00225E70();
extern void func_002265D8();
extern s32 func_00226718();
extern void func_00224B70();
extern void func_00224D28();
extern void func_00224E18();
extern void func_00224FC0();
extern void func_002250F0();
extern void func_00225180();
s32 load_hand_gadget(struct M2c_arg0 *arg0) __asm__("FUN_00224368");

s32 load_hand_gadget(struct M2c_arg0 *arg0) {
    s32 *sp30;
    s32 *sp34;
    s32 *sp38;
    u8 *sp3C;
    f32 *var_23_393;
    register f32 var_f0_428 asm("$22");
    s16 temp_5_30;
    s16 var_5_121;
    s16 var_5_158;
    s16 var_5_301;
    s16 var_5_333;
    s16 var_5_363;
    s16 var_5_407;
    register s16 var_6_19 asm("s4");
    s16 var_6_251;
    register s32 * temp_16_486 asm("s4");
    s32 temp_16_131;
    register s32 temp_16_168 asm("s0");
    register s32 temp_16_261 asm("s0");
    s32 temp_16_310;
    s32 temp_16_340;
    s32 temp_16_370;
    s32 temp_16_416;
    s32 temp_18_32;
    register s32 temp_19_26 asm("s3");
    register s32 temp_20_469 asm("s4");
    s32 var_30_7;
    s8 var_20_398;
    register u8 * var_3_460 asm("s2");
    u8 *var_4_464;
    u8 temp_2_528;
    void **var_18_402;
    register struct M2c_temp_17_118 * temp_17_118 asm("s1");
    register struct M2c_temp_17_155 * temp_17_155 asm("s1");
    register struct M2c_temp_17_17 * temp_17_17 asm("s1");
    struct M2c_temp_17_248 *temp_17_248;
    register struct M2c_temp_17_298 * temp_17_298 asm("s1");
    register struct M2c_temp_17_331 * temp_17_331 asm("s1");
    register struct M2c_temp_17_361 * temp_17_361 asm("s1");
    struct M2c_temp_17_405 *temp_17_405;
    struct M2c_temp_2_475 *temp_2_475;
    struct M2c_temp_2_83 *temp_2_83;
    struct M2c_var_17_136 *var_17_136;
    struct M2c_var_17_174 *var_17_174;
    struct M2c_var_17_230 *var_17_230;
    struct M2c_var_17_266 *var_17_266;
    struct M2c_var_17_316 *var_17_316;
    struct M2c_var_17_346 *var_17_346;
    struct M2c_var_17_376 *var_17_376;
    register struct M2c_var_17_422 * var_17_422 asm("s6");

    var_30_7 = 0;
    temp_17_17 = arg0->unk54;
    var_6_19 = -1;
    if (temp_17_17 == NULL) {
        goto block_2;
    }
    var_6_19 = temp_17_17->unkA6;
block_2:
    temp_19_26 = D_001D5BF0.unk30;
    temp_5_30 = temp_19_26 * 0x4C;
    temp_18_32 = *(s32 *)((u8 *)(D_001863D0 + temp_5_30) + 0x10);
    if (var_6_19 == temp_18_32) {
        goto block_15;
    }
    if (temp_18_32 != D_001D5BF0.unk120) {
        goto block_15;
    }
    func_00225530(temp_17_17, temp_5_30, var_6_19);
    if (D_001D5DD0.unk1 == 0) {
        goto block_6;
    }
    func_0020CB88(arg0->unk44, &D_001D5DD0);
block_6:
    if (D_0013F350.unk10B8 == 0) {
        goto block_9;
    }
    if (temp_19_26 == D_0013F350.unk10B8) {
        goto block_9;
    }
    func_001E9470(0, 0);
block_9:
    D_0015FF50[0] = D_001D5BF0.unk118 == 0;
    func_00204A40(temp_18_32, -1);
    D_001D5BF0.unk144 = (s32) D_0015FF50[0];
    D_001D5BF0.unk11C = temp_18_32;
    D_001D5BF0.unk118 = (s32) D_0015FF50[0];
    D_001D5BF0.unk140 = temp_18_32;
    *(s32 *)((u8 *)(*((*(s32 *)(temp_18_32 + D_001B3AC0) * 4) + D_001B3200)) + 0xD) = 0;
    temp_2_83 = func_00225490(temp_18_32, D_001B3200, D_0015FF50[0]);
    if (temp_2_83 == NULL) {
        goto block_14;
    }
    var_30_7 = temp_19_26;
    if (*(s32 *)(var_30_7 + D_0013E520) == 0) {
        goto block_12;
    }
    func_001E9478(temp_2_83, arg0->unk44);
block_12:
    *(s32 *)temp_2_83->unk78 = arg0;
    temp_2_83->unk74 = &func_00224B70;
    temp_2_83->unk34 = 4;
    if (var_30_7 != 0x12) {
        goto block_14;
    }
    func_0020CB10(arg0->unk44, 0, &D_001D5DD0);
    D_001D5DD0.unk28 = 0;
    D_001D5DD0.unk20 = 0;
    D_001D5DD0.unk24 = 0;
block_14:
    arg0->unk54 = temp_2_83;
block_15:
    temp_17_118 = arg0->unk50;
    var_5_121 = -1;
    if (temp_17_118 == NULL) {
        goto block_17;
    }
    var_5_121 = temp_17_118->unkA6;
block_17:
    temp_16_131 = *(s32 *)((u8 *)(D_001863D0 + (D_001D5BF0.unk38 * 0x4C)) + 0x10);
    if (var_5_121 == temp_16_131) {
        goto block_22;
    }
    var_17_136 = func_00225530(temp_17_118, var_5_121);
    if (temp_16_131 == -1) {
        goto block_21;
    }
    var_17_136 = func_00225490(temp_16_131);
    if (var_17_136 == NULL) {
        goto block_21;
    }
    var_30_7 = D_001D5BF0.unk38;
    *(s32 *)var_17_136->unk78 = arg0;
    var_17_136->unk74 = &func_00224D28;
    var_17_136->unk34 = 4;
block_21:
    arg0->unk50 = var_17_136;
block_22:
    temp_17_155 = arg0->unk58;
    var_5_158 = -1;
    if (temp_17_155 == NULL) {
        goto block_24;
    }
    var_5_158 = temp_17_155->unkA6;
block_24:
    temp_16_168 = *(s32 *)((u8 *)(D_001863D0 + (D_001D5BF0.unk34 * 0x4C)) + 0x10);
    if (var_5_158 == temp_16_168) {
        goto block_36;
    }
    var_17_174 = func_00225530(temp_17_155, var_5_158);
    if (D_001D5E10.unk1 == 0) {
        goto block_27;
    }
    func_0020CB88(arg0->unk44, &D_001D5E10);
block_27:
    if (D_001D5E50.unk1 == 0) {
        goto block_29;
    }
    func_0020CB88(arg0->unk44, &D_001D5E50);
block_29:
    if (temp_16_168 == -1) {
        goto block_32;
    }
    var_17_174 = func_00225490(temp_16_168);
    if (var_17_174 == NULL) {
        goto block_32;
    }
    var_30_7 = D_001D5BF0.unk34;
    *(s32 *)var_17_174->unk78 = arg0;
    var_17_174->unk34 = 4;
    var_17_174->unk74 = &func_00224E18;
    func_0020CB10(arg0->unk44, 0x16, &D_001D5E10);
    func_0020CB10(arg0->unk44, 0x17, &D_001D5E50);
    D_001D5E50.unk28 = 0.01f;
    D_001D5E10.unk28 = 0.01f;
    D_001D5E10.unk20 = 0.01f;
    D_001D5E10.unk24 = 0.01f;
    D_001D5E50.unk20 = 0.01f;
    D_001D5E50.unk24 = 0.01f;
block_32:
    arg0->unk58 = var_17_174;
    var_17_230 = func_00225530(arg0->unk5C);
    if (temp_16_168 == -1) {
        goto block_35;
    }
    var_17_230 = func_00225490(temp_16_168);
    if (var_17_230 == NULL) {
        goto block_35;
    }
    *(s32 *)var_17_230->unk78 = arg0;
    var_17_230->unk74 = &func_00224E18;
    var_17_230->unk34 = 4;
block_35:
    arg0->unk5C = var_17_230;
block_36:
    temp_17_248 = arg0->unk4C;
    var_6_251 = -1;
    if (temp_17_248 == NULL) {
        goto block_38;
    }
    var_6_251 = temp_17_248->unkA6;
block_38:
    temp_16_261 = *(s32 *)((u8 *)(D_001863D0 + (D_001D5BF0.unk3C * 0x4C)) + 0x10);
    if (var_6_251 == temp_16_261) {
        goto block_46;
    }
    var_17_266 = func_00225530(temp_17_248, 0x4C, var_6_251);
    if (temp_16_261 == -1) {
        goto block_45;
    }
    var_17_266 = func_00225490(temp_16_261);
    if (var_17_266 == NULL) {
        goto block_45;
    }
    *(s32 *)var_17_266->unk78 = arg0;
    var_17_266->unk74 = &func_00224FC0;
    var_17_266->unk34 = 4;
    if (var_17_266->unkA6 != 0x25F) {
        goto block_45;
    }
    if (var_17_266->unk53 == 6) {
        goto block_44;
    }
    func_00212F90(var_17_266, 6, 0, 0xA);
block_44:
    var_17_266->unk20 = 8;
block_45:
    arg0->unk4C = var_17_266;
block_46:
    temp_17_298 = arg0->unk6C;
    var_5_301 = -1;
    if (temp_17_298 == NULL) {
        goto block_48;
    }
    var_5_301 = temp_17_298->unkA6;
block_48:
    temp_16_310 = (D_0013D4C0.unk23 != 0) ? 0x197 : -1;
    if (var_5_301 == temp_16_310) {
        goto block_53;
    }
    var_17_316 = func_00225530(temp_17_298, var_5_301);
    if (temp_16_310 == -1) {
        goto block_52;
    }
    var_17_316 = func_00225490(temp_16_310);
    if (var_17_316 == NULL) {
        goto block_52;
    }
    *(s32 *)var_17_316->unk78 = arg0;
    var_17_316->unk74 = &func_002250F0;
    var_17_316->unk34 = 4;
block_52:
    arg0->unk6C = var_17_316;
block_53:
    temp_17_331 = arg0->unk70;
    var_5_333 = -1;
    if (temp_17_331 == NULL) {
        goto block_55;
    }
    var_5_333 = temp_17_331->unkA6;
block_55:
    temp_16_340 = (D_0013D4C0.unk21 != 0) ? 0x266 : -1;
    if (var_5_333 == temp_16_340) {
        goto block_60;
    }
    var_17_346 = func_00225530(temp_17_331, var_5_333);
    if (temp_16_340 == -1) {
        goto block_59;
    }
    var_17_346 = func_00225490(temp_16_340);
    if (var_17_346 == NULL) {
        goto block_59;
    }
    *(s32 *)var_17_346->unk78 = arg0;
    var_17_346->unk74 = &func_002250F0;
    var_17_346->unk34 = 4;
block_59:
    arg0->unk70 = var_17_346;
block_60:
    temp_17_361 = arg0->unk74;
    var_5_363 = -1;
    if (temp_17_361 == NULL) {
        goto block_62;
    }
    var_5_363 = temp_17_361->unkA6;
block_62:
    temp_16_370 = (D_0013D4C0.unk22 != 0) ? 0x26A : -1;
    if (var_5_363 == temp_16_370) {
        goto block_67;
    }
    var_17_376 = func_00225530(temp_17_361, var_5_363);
    if (temp_16_370 == -1) {
        goto block_66;
    }
    var_17_376 = func_00225490(temp_16_370);
    if (var_17_376 == NULL) {
        goto block_66;
    }
    *(s32 *)var_17_376->unk78 = arg0;
    var_17_376->unk74 = &func_002250F0;
    var_17_376->unk34 = 4;
block_66:
    arg0->unk74 = var_17_376;
block_67:
    var_23_393 = D_001D5E90;
    sp30 = ((u8 *)arg0) + 0x60;
    sp3C = ((u8 *)arg0) + 0xA4;
    var_20_398 = 0;
    sp38 = ((u8 *)arg0) + 0x68;
    sp34 = ((u8 *)arg0) + 0x64;
    var_18_402 = ((u8 *)arg0) + 0x78;
loop_68:
    temp_17_405 = *var_18_402;
    var_5_407 = -1;
    if (temp_17_405 == NULL) {
        goto block_70;
    }
    var_5_407 = temp_17_405->unkA6;
block_70:
    temp_16_416 = (!(var_20_398 < (s32) D_0013F350.unk1FF7)) ? (-1) : (0x1DF);
    if (var_5_407 == temp_16_416) {
        goto block_77;
    }
    var_17_422 = func_00225530(temp_17_405, var_5_407);
    if (temp_16_416 == -1) {
        goto block_76;
    }
    var_f0_428 = 3.0f;
    var_17_422 = func_00225490(temp_16_416);
    if (var_20_398 >= (s32) D_0013F350.unk1FF6) {
        goto block_74;
    }
    var_f0_428 = 0.0f;
block_74:
    *var_23_393 = var_f0_428;
    *(s32 *)((var_20_398 * 4) + D_001D5EA8) = 0;
    if (var_17_422 == NULL) {
        goto block_76;
    }
    *(s32 *)var_17_422->unk78 = arg0;
    var_17_422->unk74 = &func_00225180;
    var_17_422->unkBC = var_20_398;
    var_17_422->unk34 = 4;
block_76:
    *var_18_402 = var_17_422;
block_77:
    var_20_398 += 1;
    var_18_402 += 4;
    var_23_393 += 4;
    if (var_20_398 < 8) {
        goto loop_68;
    }
    var_3_460 = sp3C;
    if (var_30_7 == D_001D5BF0.unk1C) {
        goto block_84;
    }
    if (var_30_7 <= 0) {
        goto block_84;
    }
    var_4_464 = var_3_460 + 0x18;
    if (var_30_7 >= 0x24) {
        goto loop_85;
    }
    D_001D5BF0.unk1C = var_30_7;
    func_00226718(var_4_464);
    temp_20_469 = var_30_7 * 0x30;
    temp_2_475 = D_001D52E8 + temp_20_469;
    temp_16_486 = temp_20_469 + (D_001D52E8 + 0xC);
    func_002265D8(temp_2_475->unk8 + D_001D5BF0.unkCC, 0, *temp_16_486, var_30_7, temp_2_475->unk10, temp_2_475->unk18, temp_2_475->unk1C, temp_2_475->unk20, temp_2_475->unk24, temp_2_475->unk28, temp_2_475->unk2C, *(s32 *)(temp_20_469 + D_001D52E8), temp_2_475->unk4);
    if (*temp_16_486 == 0) {
        goto block_83;
    }
    func_002265D8(*(s32 *)((u8 *)(D_001D52E8 + temp_20_469) + 0x14) + D_001D5BF0.unkCC, 2, 0, var_30_7, 1, -1, 0, -1, 0, -1, 0, 0, 0);
block_83:
    var_3_460 = sp3C;
block_84:
    var_4_464 = var_3_460 + 0x18;
loop_85:
    temp_2_528 = *var_3_460;
    if (temp_2_528 == 0) {
        goto block_87;
    }
    *var_3_460 = temp_2_528 - 1;
block_87:
    var_3_460 += 1;
    if ((s32) var_3_460 < (s32) var_4_464) {
        goto loop_85;
    }
    func_00225E70(arg0->unk44, arg0->unk54, arg0->unk50, sp30, sp34, sp38);
    return 0;
}

extern s32 func_00224368(struct M2c_arg0 *arg0) __attribute__((alias("FUN_00224368")));
#endif /* NON_MATCHING */
