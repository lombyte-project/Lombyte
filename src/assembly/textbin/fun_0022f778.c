#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING

#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022f778/FUN_0022f778.s", FUN_0022f778);
#else
#include "rnc/fun_0022f778_types.h"
#include "types.h"
extern s32 D_0013CAE4[];
extern struct M2c_D_0013D4C0 D_0013D4C0;
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_0013F350 D_0013F350;
extern u8 D_001413F5;
extern struct M2c_D_001516D0 D_001516D0;
extern f32 D_0015ED60;
extern f32 D_0015ED6C;
extern s32 D_0015ED70[];
extern s32 D_0015ED80;
extern s32 D_0015ED84;
extern u8 D_0015EDB4;
extern f32 D_0015F43C;
extern f32 D_0015F440;
extern s32 D_0015F5B0;
extern s32 D_0015F604;
extern s32 D_0015F618;
extern s32 D_0015FF1C[];
extern s32 D_00160034;
extern s32 D_00160510[1];
extern u8 D_00187080[];
extern struct M2c_D_0018CB20 D_0018CB20;
extern f32 D_0018CDB0;
extern u8 D_001CC3B0[];
extern s32 D_001D5BF0[1];
extern u8 D_001D99B0[];
extern u8 D_001D9C80[];
extern u8 D_001D9CB0[];
extern u8 D_0022E1B0[];
extern s32 CalculateDmaTransferAddress();
extern void func_00125218(void *);
extern void func_00125360(void *, void *, f32);
extern void func_00125408(void *, void *, f32);
extern void func_001252B8(void *, void *, f32);
extern s32 InitializeRenderState();
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern s32 UpdateRenderStateCommand();
extern s32 func_001E93F0();
extern s32 func_001E93F8();
extern s32 func_001E9400();
extern s32 func_001E9408();
extern s32 func_001E9410();
extern s32 func_001E9418();
extern s32 func_001E9420();
extern s32 func_001E9428();
extern s32 func_001E9430();
extern s32 func_001E9438();
extern s32 func_001E9440();
extern s32 func_001E9450();
extern s32 func_001EDAA8();
extern s32 func_001F2D98();
extern s32 func_001F4600();
extern s32 func_001F47B8();
extern s32 func_001F4A58();
extern f32 func_001F96E8(f32);
extern s32 func_001F96F8();
extern f32 func_001F99C0(f32);
extern s32 func_001F99F8();
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A40(void *, void *, void *, f32);
extern void func_001F9A68(void *, void *, f32);
extern s32 func_001F9AD8();
extern f32 func_001F9B48(void *, void *);
extern f32 func_001F9B80(void *, void *, void *);
extern void func_001F9CF8(void *, void *, void *);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 func_001F9E90(f32, f32);
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA5C8(f32, f32);
extern f32 func_001FA6C0(s32);
extern s32 func_00201A28();
extern s32 func_002049F0();
extern s32 func_0020C4F8();
extern s32 func_0020C828();
extern s32 func_0020C880();
extern s32 func_0020CFD0();
extern s32 func_0020DEF8();
extern s32 func_00212E28();
extern s32 func_00213700();
extern void func_00213F38(void *, void *, s32, f32, f32, f32);
extern s32 func_00214E58(void *, s32, void *, void *, s32, f32);
extern s32 func_00215B10();
extern s32 func_002168A8();
extern s32 func_00217B88();
extern s32 pause_all_sounds();
extern s32 func_0022CA50();
extern s32 func_0022DC50();
extern s32 func_0022EAA8();
extern void func_0022F5B0(void *, f32);
extern s32 sceGsSyncV();
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern void sceVu0UnitMatrix(void *);
extern void func_0022DF40();
extern void func_0022E420();
extern void func_002327A0();
extern void jtbl_001E8930();
void FUN_0022f778(void) {
    u8 sp_slot[0x40];
    s32 sp40;
    f32 *sp44;
    s32 var_4_225;
    f32 *temp_17_430;
    struct M2c_temp_2_349 *temp_2_349;
    f32 *temp_4_1462;
    f32 *temp_4_1483;
    f32 *temp_4_1532;
    f32 *temp_4_1554;
    s32 temp_5_661;
    f32 temp_f0_1016;
    f32 temp_f0_1035;
    f32 temp_f0_1307;
    f32 temp_f0_1336;
    f32 temp_f0_1506;
    f32 temp_f0_51;
    f32 temp_f0_644;
    f32 temp_f0_716;
    f32 temp_f0_727;
    f32 temp_f12_1401;
    f32 temp_f13_1163;
    f32 temp_f13_1198;
    f32 temp_f13_1220;
    f32 temp_f1_1017;
    f32 temp_f20_1029;
    f32 temp_f20_1070;
    f32 temp_f20_1249;
    f32 temp_f20_333;
    f32 var_f20_1267;
    f32 var_f21_1004;
    f32 var_f2_715;
    s16 temp_16_1239;
    s16 temp_16_1264;
    s16 temp_2_240;
    s16 temp_2_465;
    s16 var_16_73;
    s32 *temp_16_1342;
    s32 temp_16_434;
    register s32 temp_18_70 asm("s2");
    s32 temp_2_1053;
    s32 temp_2_132;
    s32 temp_2_299;
    s32 temp_3_394;
    s32 temp_3_400;
    s32 temp_3_997;
    s32 temp_4_1437;
    s32 temp_4_29;
    s32 temp_4_775;
    s32 temp_5_913;
    s32 temp_7_1526;
    s32 temp_7_1548;
    s32 temp_lo_134;
    s32 var_16_184;
    s32 var_18_1006;
    s32 var_22_383;
    s32 var_2_236;
    s32 var_2_634;
    s32 var_2_664;
    s32 var_2_781;
    s32 var_2_853;
    s32 var_3_662;
    s32 var_3_855;
    s32 var_4_1085;
    s32 var_4_628;
    s32 var_condition_bit_717;
    u8 temp_2_231;
    u8 temp_2_251;
    u8 temp_4_309;
    u8 *var_17_196;
    struct M2c_temp_16_1014 *temp_16_1014;
    struct M2c_temp_16_1110 *temp_16_1110;
    struct M2c_temp_16_308 *temp_16_308;
    struct M2c_temp_16_893 *temp_16_893;
    struct M2c_temp_17_1007 *temp_17_1007;
    struct M2c_temp_17_1234 *temp_17_1234;
    s32 *temp_17_1453;
    struct M2c_temp_17_310 *temp_17_310;
    s32 *temp_18_1054;
    s32 *temp_18_1458;
    struct M2c_temp_18_399 *temp_18_399;
    struct M2c_temp_18_940 *temp_18_940;
    s32 *temp_19_1055;
    struct M2c_temp_19_1379 *temp_19_1379;
    struct M2c_temp_2_1026 *temp_2_1026;
    struct M2c_temp_2_1061 *temp_2_1061;
    struct M2c_temp_2_1075 *temp_2_1075;
    struct M2c_temp_2_570 *temp_2_570;
    struct M2c_temp_2_955 *temp_2_955;
    struct M2c_temp_2_973 *temp_2_973;
    struct M2c_temp_3_1015 *temp_3_1015;
    struct M2c_temp_3_1060 *temp_3_1060;
    struct M2c_temp_3_198 *temp_3_198;
    struct M2c_temp_3_205 *temp_3_205;
    struct M2c_temp_4_1517 *temp_4_1517;
    struct M2c_temp_4_1538 *temp_4_1538;
    struct M2c_temp_4_981 *temp_4_981;
    struct M2c_temp_5_201 *temp_5_201;
    struct M2c_temp_5_966 *temp_5_966;
    struct M2c_temp_5_971 *temp_5_971;
    struct M2c_var_16_1502 *var_16_1502;
    struct M2c_var_17_904 *var_17_904;
    u8 *var_3_1096;
    struct M2c_var_5_227 *var_5_227;

    D_0013E030.unk24 = (u16) (D_0013E030.unk24 + 1);
    if ((u32) D_0013E030.unk20 < 9U) {
        switch (D_0013E030.unk20) {
        case 1:
        case 2:
        case 5:
        case 6:
        case 7:
            break;
        case 0:
        case 8:
            func_001E9430();
            func_00212E28();
            func_0022DC50();
            func_001E9420();
            func_00217B88();
            temp_f0_51 = D_0015F43C - 0.125f;
            D_0018CB20.unk38 = (s32) (D_0018CB20.unk38 + 1);
            D_0018CB20.unk34 = (s32) (D_0018CB20.unk34 + 1);
            D_0015F43C = temp_f0_51;
            if (temp_f0_51 < 0.0f) {
                D_0015F43C = 0.0f;
            }
            temp_18_70 = (D_0015ED80 == 0) ? 0x60 : 0x50;
            var_16_73 = D_0013E030.unk28;
            if (D_0013CAE4[0] & 0x50) {
                if (func_001F96F8(0x1E) < D_0018CB20.unk34) {
                    if (D_0015F43C == 0.0f) {
                        if (D_0013E030.unk20 == 0) {
                            var_16_73 = (D_0018CB20.unk34 < func_001F96F8(D_00160510[D_0013E030.unk26] - 0x1E)) ? 1 : var_16_73;
                        }
                        if (D_0013E030.unk20 == 8) {
                            var_16_73 = (D_0018CB20.unk34 < (D_0018CB20.unk40 - func_001F96F8(0x1E))) ? 1 : var_16_73;
                        }
                    }
                }
            }
            if (var_16_73 != 0) {
                if ((u32) ((u16) D_001516D0.unk5A - 6) >= 2U) {
                    D_001516D0.unk5A = 5U;
                }
                if (D_0013E030.unk20 == 0) {
                    temp_2_132 = func_001F96F8(D_00160510[D_0013E030.unk26]);
                    temp_lo_134 = temp_2_132 / temp_18_70;
                    D_0018CB20.unk34 = temp_2_132;
                    D_0018CB20.unk3C = temp_lo_134;
                    func_002049F0(temp_lo_134);
                    D_0018CB20.unk38 = (s32) ((s32) D_0018CB20.unk34 % temp_18_70);
                    if (D_0013E030.unk28 != 0) {
                        D_0013E030.unk28 = 0;
                    } else {
                        func_001F4A58(4);
                    }
                    D_0015F43C = 1.0f;
                } else {
                    D_0018CB20.unk34 = (s32) D_0018CB20.unk40;
                }
            }
            if (D_0018CB20.unk34 >= D_0018CB20.unk40) {
                if ((u32) ((u16) D_001516D0.unk5A - 6) >= 2U) {
                    D_001516D0.unk5A = 5U;
                }
                var_16_184 = 0;
                CalculateDmaTransferAddress();
                D_0018CDB0 = 0.63f;
                func_001F2D98();
                if (D_0018CB20.unk44 > 0) {
                    var_17_196 = ((u8 *)&D_0018CB20) + 0x178;
                    do {
                        temp_3_198 = *(struct M2c_temp_3_198 **)var_17_196;
                        if (temp_3_198 != NULL) {
                            temp_5_201 = temp_3_198->unk24;
                            temp_5_201->unkC = (u8) (temp_5_201->unkC - 1);
                            temp_3_205 = temp_3_198->unk24;
                            *(s32 *)((u8 *)(((u8 *)temp_3_205) + (temp_3_205->unkC * 4)) + 0x48) = 0;
                            func_0020C828(temp_3_198, temp_5_201);
                        }
                        if (D_0013E030.unk8 != NULL) {
                            func_0020C828(D_0013E030.unk8);
                        }
                        var_16_184 += 1;
                        var_17_196 += 4;
                    } while (var_16_184 < D_0018CB20.unk44);
                }
                var_4_225 = 0xFF;
                var_5_227 = D_0015FF1C[0];
                D_0013E030.unk0->unk34 = (u16) (D_0013E030.unk0->unk34 & 0xFFFE);
                temp_2_231 = var_5_227->unk20;
                if (temp_2_231 != 0xFF) {
                    var_4_225 = 0xCB;
                    var_2_236 = temp_2_231 & 0x80;
                    do {
                        if (var_2_236 == 0) {
                            temp_2_240 = var_5_227->unkA6;
                            if (temp_2_240 != 0x4A) {
                                if (temp_2_240 == 0xCB) {
                                    goto block_35;
                                }
                            } else {
block_35:
                                var_5_227->unk34 = (u16) (var_5_227->unk34 & 0xFF7F);
                            }
                        }
                        var_5_227 += 0x100;
                        temp_2_251 = var_5_227->unk20;
                        var_2_236 = temp_2_251 & 0x80;
                    } while (temp_2_251 != 0xFF);
                }
                if (D_0013E030.unk20 == 0) {
                    pause_all_sounds(0, var_5_227, 0x4A);
                    D_001D5BF0[0] = 0xE;
                    D_0015F618 = 1;
                    return;
                }
                D_0015F604 = 0;
                UpdateRenderStateCommand(var_4_225, var_5_227, 0x4A);
                D_0015F618 = 1;
                D_001413F5 = 0;
                func_001E9438();
                if (D_0013E030.unk2C != 0) {
                    func_001E9408();
                    return;
                }
                func_001E9440(((u8 *)&D_0013E030) + 0x60, ((u8 *)&D_0013E030) + 0x70, 0, 1);
                return;
            }
            if (D_0018CB20.unk38 >= temp_18_70) {
                temp_2_299 = D_0018CB20.unk3C + 1;
                D_0018CB20.unk3C = temp_2_299;
                func_002049F0(temp_2_299, D_0015ED80);
            }
            temp_16_308 = D_0018CB20.unk54 + (D_0018CB20.unk38 << 5);
            temp_4_309 = temp_16_308->unkC;
            temp_17_310 = ((u8 *)temp_16_308) + 0x10;
            sp40 = (s32) temp_4_309;
            D_0018CDB0 = temp_17_310->unkC;
            func_001F2D98(temp_4_309);
            *(Vec4 *)D_00187080 = *(Vec4 *)temp_16_308;
            func_001F9CF8(D_00187080, D_00187080, (void *)((u8 *)D_0013E030.unk0 + 0xC0));
            func_001F9A10(D_00187080, D_00187080, (void *)((u8 *)D_0013E030.unk0 + 0x10));
            temp_f20_333 = func_001FA580(temp_17_310->unk8, D_0013E030.unk0->unk48);
            func_00125218(sp_slot);
            func_00125360(sp_slot, sp_slot, temp_16_308->unk10);
            func_00125408(sp_slot, sp_slot, temp_17_310->unk4);
            func_001252B8(sp_slot, sp_slot, temp_f20_333);
            temp_2_349 = ((u8 *)D_00187080 - (u8 *)0x140);
            temp_2_349->unk378 = *(f32 *)(sp_slot + 0x18);
            temp_2_349->unk350 = -*(f32 *)(sp_slot + 0x20);
            temp_2_349->unk360 = -*(f32 *)(sp_slot + 0x0);
            temp_2_349->unk370 = *(f32 *)(sp_slot + 0x10);
            temp_2_349->unk354 = -*(f32 *)(sp_slot + 0x24);
            temp_2_349->unk364 = -*(f32 *)(sp_slot + 0x4);
            temp_2_349->unk374 = *(f32 *)(sp_slot + 0x14);
            temp_2_349->unk358 = -*(f32 *)(sp_slot + 0x28);
            temp_2_349->unk368 = -*(f32 *)(sp_slot + 0x8);
            if (D_0015EDB4 != 0) {
                func_001F9AD8(D_00187080 + 0x220, D_00187080 + 0x230, D_00187080 + 0x210);
            }
            var_22_383 = 0;
            if (D_0018CB20.unk44 > 0) {
                sp44 = (f32 *)(sp_slot + 0x10);
                do {
                temp_18_399 = *(s32 *)((u8 *)(((u8 *)&D_0018CB20) + (var_22_383 * 4)) + 0x178);
                temp_3_400 = D_0018CB20.unk38 >> 1;
                temp_18_399->unk50 = (u8) temp_3_400;
                temp_18_399->unk51 = (u8) (temp_3_400 + 1);
                func_0020C880(temp_18_399);
                temp_18_399->unk54 = (f32) (func_001FA6C0(D_0018CB20.unk38 & 1) * 0.5f);
                if (sp40 != 0) {
                    if (D_0018CB20.unk38 & 1) {
                        temp_18_399->unk54 = 1.0f;
                    }
                }
                temp_17_430 = ((u8 *)temp_18_399) + 0x10;
                temp_16_434 = temp_18_399->unk78;
                func_001F9A68(sp_slot, temp_16_434 + (temp_18_399->unk50 * 0x10), 1.0f - temp_18_399->unk54);
                func_001F9A68(sp44, temp_16_434 + (temp_18_399->unk51 * 0x10), temp_18_399->unk54);
                func_001F9A10(temp_17_430, sp_slot, sp44);
                func_001F9CF8(temp_17_430, temp_17_430, (void *)((u8 *)D_0013E030.unk0 + 0xC0));
                func_001F9A10(temp_17_430, temp_17_430, (void *)((u8 *)D_0013E030.unk0 + 0x10));
                temp_18_399->unk71 = 0xFF;
                temp_18_399->unk48 = (f32) D_0013E030.unk0->unk48;
                func_0020DEF8(temp_18_399);
                temp_2_465 = temp_18_399->unkA6;
                if (temp_2_465 == 0) {
                    if (D_0013E030.unk20 == 0) {
                        if (D_0015ED84 == 0xA) {
                            if (D_0013D4C0.unk6 == 0) {
                                temp_18_399->unk7F = 0U;
                            } else {
                                goto block_57;
                            }
                        } else {
block_57:
                            if (func_001F96F8(0x3E) < D_0018CB20.unk34) {
                                temp_18_399->unk7F = 0U;
                            } else {
                                goto block_69;
                            }
                        }
                    } else {
                        if (func_001F96F8(0x168) < D_0018CB20.unk34) {
                            if (D_0015ED84 == 0xA) {
                                if (D_0013D4C0.unk6 != 0) {
                                    goto block_72;
                                }
                                goto block_71;
                            }
                            goto block_72;
                        }
                        goto block_71;
                    }
                } else if (temp_2_465 == 0xA) {
                    if (D_0013E030.unk20 == 0) {
                        if (func_001F96F8(0x15E) < D_0018CB20.unk34) {
                            temp_18_399->unk7F = 0U;
                        } else {
block_69:
                            temp_18_399->unk7F = 0x18U;
                        }
                    } else if (func_001F96F8(0x20C) >= D_0018CB20.unk34) {
block_71:
                        temp_18_399->unk7F = 0U;
                    } else {
block_72:
                        temp_18_399->unk7F = 0x18U;
                    }
                }
                if (temp_18_399->unk7F != 0) {
                    func_00213700(temp_18_399);
                }
                if (temp_18_399->unkA6 == 0xA) {
                    func_001E9418(temp_18_399);
                }
                if (temp_18_399->unkA6 == 0) {
                    func_001E9410(temp_18_399);
                    if (D_0015ED84 == 0xA) {
                        if (D_0013D4C0.unk6 == 0) {
                            goto block_81;
                        }
                        goto block_83;
                    }
block_81:
                    if (D_0015ED84 == 0xD) {
block_83:
                        if (D_0013E030.unk8 == NULL) {
                            temp_2_570 = func_0020C4F8(0x509);
                            D_0013E030.unk8 = temp_2_570;
                            temp_2_570->unk32 = 0x40;
                            D_0013E030.unk8->unk34 = (u16) (D_0013E030.unk8->unk34 | 0x806);
                            D_0013E030.unk8->unk38 = (s64) temp_18_399->unk38;
                            if (D_0013E030.unk8->unk24->unk6 != 0) {
                                D_0013E030.unk8->unk73 = 0x18;
                            }
                        }
                        func_001E9450(temp_18_399, D_0013E030.unk8);
                    }
                }
                if ((u32) ((u16) temp_18_399->unkA6 - 0x213) < 3U) {
                    func_001F47B8(&func_002327A0, temp_18_399);
                    func_001F4600(&func_0022DF40, temp_18_399);
                    if (D_0013E030.unk20 == 0) {
                        if (D_0018CB20.unk34 < func_001F96F8(0x168)) {
                            var_4_628 = (s32) (fast_cos(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
                            if (temp_18_399->unkA6 == 0x215) {
                                var_2_634 = (var_4_628 >> 1) | (var_4_628 << 8) | (var_4_628 << 0x10);
                            } else {
                                goto block_120;
                            }
                            goto block_123;
                        }
                        if (D_0018CB20.unk34 < func_001F96F8(0x1F8)) {
                            temp_f0_644 = func_001F96E8(360.0f);
                            temp_5_661 = (s32) ((((f32) D_0018CB20.unk34) - temp_f0_644) * (D_0015ED60 * 1.25f));
                            if (temp_5_661 >= 256) {
                                temp_5_661 = 255;
                            }
                            if (temp_18_399->unkA6 == 0x215) {
                                var_3_662 = temp_5_661 << 0x10;
                                var_2_664 = temp_5_661 << 8;
                            } else {
                                var_3_662 = temp_5_661 << 0x10;
                                var_2_664 = temp_5_661 | (temp_5_661 << 8);
                            }
                            temp_18_399->unk90 = (s32) (var_2_664 | var_3_662);
                            if (func_001F96F8(0x1A4) < D_0018CB20.unk34) {
                                temp_5_661 = (s32) ((((f32) D_0018CB20.unk34) - func_001F96E8(420.0f)) * (D_0015ED60 * 1.2f));
                                D_0013E030.unk0->unkBC = (s8)(u8) temp_5_661;
                                D_0013E030.unk0->unkB2 = 0;
                                func_001F4600(D_0022E1B0, D_0013E030.unk0);
                            }
                            if (func_001F96F8(0x1D0) < D_0018CB20.unk34) {
                                var_f2_715 = 1.0f;
                                temp_f0_716 = D_0015F440 + 0.025f;
                                var_condition_bit_717 = temp_f0_716 > 1.0f;
                                D_0015F440 = temp_f0_716;
                                goto block_102;
                            }
                        } else {
                            var_f2_715 = 0.0f;
                            temp_f0_727 = D_0015F440 - 0.025f;
                            temp_18_399->unk90 = 0xA0A0A0;
                            var_condition_bit_717 = temp_f0_727 < 0.0f;
                            D_0015F440 = temp_f0_727;
block_102:
                            if (var_condition_bit_717) {
                                D_0015F440 = var_f2_715;
                            }
                        }
                    } else if (D_0018CB20.unk34 < func_001F96F8(0xF0)) {
                        if (D_0018CB20.unk34 < func_001F96F8(0xC8)) {
                            func_0022F5B0(temp_18_399, D_0015ED6C * -4.0f);
                            func_0022F5B0(temp_18_399, D_0015ED6C * -3.0f);
                        }
                        temp_4_775 = (s32) (fast_cos(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
                        if (temp_18_399->unkA6 == 0x215) {
                            var_2_781 = (temp_4_775 >> 1) | (temp_4_775 << 8) | (temp_4_775 << 0x10);
                        } else {
                            var_2_781 = temp_4_775 | (temp_4_775 << 8) | (temp_4_775 << 0x10);
                        }
                        temp_18_399->unk90 = var_2_781;
                        temp_18_399->unkBC = 0x32;
                        temp_18_399->unkB2 = 0xA;
                        func_001F4600(D_0022E1B0, temp_18_399);
                    } else {
                        if (func_001F96F8(0x168) >= D_0018CB20.unk34) {
                            if (D_0018CB20.unk34 < func_001F96F8(0x12C)) {
                                temp_5_661 = (s32) ((func_001F96E8(300.0f) - ((f32) D_0018CB20.unk34)) * (D_0015ED60 * 1.5f));
                                D_0013E030.unk0->unkBC = (s8)(u8) temp_5_661;
                                D_0013E030.unk0->unkB2 = 0;
                                func_001F4600(D_0022E1B0, D_0013E030.unk0);
                            }
                            var_4_628 = (s32) ((func_001F96E8(360.0f) - ((f32) D_0018CB20.unk34)) * (D_0015ED60 * 1.25f));
                            var_2_853 = var_4_628 << 8;
                            if (temp_18_399->unkA6 == 0x215) {
                                var_3_855 = var_4_628 << 0x10;
                            } else {
                                goto block_121;
                            }
                            goto block_122;
                        }
                        var_4_628 = (s32) (fast_cos(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
                        if (temp_18_399->unkA6 == 0x215) {
                            var_2_634 = (var_4_628 >> 1) | (var_4_628 << 8) | (var_4_628 << 0x10);
                        } else {
block_120:
                            var_2_853 = var_4_628 << 8;
block_121:
                            var_3_855 = var_4_628 << 0x10;
                            var_2_853 |= var_4_628;
block_122:
                            var_2_634 = var_2_853 | var_3_855;
                        }
block_123:
                        temp_18_399->unk90 = var_2_634;
                    }
                }
                var_22_383 += 1;
                } while (var_22_383 < D_0018CB20.unk44);
            }
            goto block_180;
        case 3:
            temp_16_893 = &D_0013E030;
            func_001E9430();
            func_00212E28();
            func_0022DC50();
            func_001E9420();
            func_00217B88();
            var_17_904 = &D_0013E030;
            if (temp_16_893->unk24 == 0) {
                InitializeRenderState(0);
                func_002168A8(0);
                temp_5_913 = temp_16_893->unk26 + 0x9C4F;
                D_001516D0.unk1C = temp_5_913;
                func_001F4A58(func_001F96F8(0xC, temp_5_913));
                if ((s16) D_001516D0.unk5A != 3) {
                    do {
                        func_0022CA50();
                        sceGsSyncV(0);
                    } while ((s16) D_001516D0.unk5A != 3);
                }
                func_00215B10();
                temp_18_940 = &D_0013E030;
                *(u8 *)((u8 *)D_0013E030.unk0 + 0x52) = 1;
                *(u8 *)((u8 *)D_0013E030.unk0 + 0x53) = 2;
                *(f32 *)((u8 *)D_0013E030.unk0 + 0x54) = 0.0f;
                func_0020C880(*(s32 *)((u8 *)&D_0013E030), 2);
                *(u8 *)((u8 *)D_0013E030.unk0 + 0x72) = 0xFF;
                *(s32 *)((u8 *)D_0013E030.unk0 + 0x94) = 0;
                if (temp_18_940->unk30 >= 0) {
                    temp_2_955 = func_0020C4F8(0);
                    temp_18_940->unk10 = temp_2_955;
                    temp_2_955->unk32 = 0x1FF;
                    temp_18_940->unk10->unk72 = 0xFF;
                    temp_18_940->unk10->unk94 = 0;
                    temp_5_966 = temp_18_940->unk10;
                    temp_5_966->unk34 = (u16) (temp_5_966->unk34 | 6);
                    temp_5_971 = temp_18_940->unk10;
                    temp_5_971->unk38 = (s64) D_0013F350.unk2080->unk38;
                    temp_2_973 = func_0020C4F8(0xA, temp_5_971);
                    temp_18_940->unk14 = temp_2_973;
                    temp_2_973->unk32 = 0x1FF;
                    temp_18_940->unk14->unk72 = 0xFF;
                    temp_18_940->unk14->unk94 = 0;
                    temp_4_981 = temp_18_940->unk14;
                    temp_4_981->unk34 = (u16) (temp_4_981->unk34 | 6);
                    temp_18_940->unk14->unk38 = (s64) D_0013F350.unk2080->unk38;
                }
                func_001E93F0(D_00187080, D_00187080 + 0x10, 1, 0, 0);
                temp_3_997 = temp_18_940->unk30;
                temp_18_940->unk48 = 0.0f;
                temp_18_940->unk4C = 0;
                if (temp_3_997 >= 0) {
                    var_f21_1004 = temp_18_940->unk48;
                    var_18_1006 = 1;
                    temp_17_1007 = *(struct M2c_temp_17_1007 **)((u8 *)D_001CC3B0 + (temp_3_997 * 4));
                    temp_17_1007->unk1C = var_f21_1004;
                    while (var_18_1006 < (temp_17_1007->unk0 - 1)) {
                        __asm__("" : "+r"(var_18_1006));
                        temp_16_1014 = ((u8 *)temp_17_1007) + (var_18_1006 * 0x10);
                        temp_3_1015 = ((u8 *)temp_17_1007) + ((var_18_1006 - 1) * 0x10);
                        temp_f0_1016 = temp_16_1014->unk10;
                        temp_f1_1017 = temp_3_1015->unk14;
                        var_18_1006 += 1;
                        temp_2_1026 = ((u8 *)temp_17_1007) + (var_18_1006 * 0x10);
                        temp_f20_1029 = func_001F9E90(temp_f0_1016 - temp_3_1015->unk10, temp_16_1014->unk14 - temp_f1_1017);
                        temp_f0_1035 = func_001FA5C8(func_001F9E90(temp_2_1026->unk10 - temp_16_1014->unk10, temp_2_1026->unk14 - temp_16_1014->unk14), temp_f20_1029);
                        temp_16_1014->unk1C = temp_f0_1035;
                        if (var_f21_1004 < func_001F99C0(temp_f0_1035)) {
                            var_f21_1004 = func_001F99C0(temp_16_1014->unk1C);
                        }
                    }
                    temp_2_1053 = temp_17_1007->unk0;
                    temp_18_1054 = ((u8 *)temp_17_1007) + 0x10;
                    temp_19_1055 = ((u8 *)temp_17_1007) + 0x20;
                    temp_3_1060 = ((u8 *)temp_17_1007) + ((temp_2_1053 - 2) * 0x10);
                    temp_2_1061 = ((u8 *)temp_17_1007) + ((temp_2_1053 - 1) * 0x10);
                    temp_f20_1070 = func_001F9E90(temp_2_1061->unk10 - temp_3_1060->unk10, temp_2_1061->unk14 - temp_3_1060->unk14);
                    temp_2_1075 = ((u8 *)temp_17_1007) + ((temp_17_1007->unk0 - 1) * 0x10);
                    var_4_1085 = 0;
                    *(f32 *)((u8 *)(((u8 *)temp_17_1007) + ((temp_17_1007->unk0 - 1) * 0x10)) + 0x1C) = func_001FA5C8(func_001F9E90(temp_17_1007->unk10 - temp_2_1075->unk10, temp_17_1007->unk14 - temp_2_1075->unk14), temp_f20_1070);
                    if (temp_17_1007->unk0 > 0) {
                        var_3_1096 = temp_17_1007;
                        do {
                            var_4_1085 += 1;
                            *(f32 *)(var_3_1096 + 0x1C) = *(f32 *)(var_3_1096 + 0x1C) * (0.34906584f / var_f21_1004);
                            var_3_1096 += 0x10;
                        } while (var_4_1085 < temp_17_1007->unk0);
                    }
                    temp_16_1110 = &D_0013E030;
                    temp_16_1110->unk44 = func_001F9B48(temp_18_1054, temp_19_1055);
                    temp_16_1110->unk3C = 0;
                    temp_16_1110->unk40 = 0;
                    temp_16_1110->unk50 = 0;
                    temp_16_1110->unk54 = 0;
                    temp_16_1110->unk80 = *(Vec4 *)((u8 *)D_0013E030.unk0 + 0x10);
                    temp_16_1110->unk90 = *(Vec4 *)temp_17_1007;
                    temp_16_1110->unkA0 = *(Vec4 *)((u8 *)D_0013E030.unk0 + 0x40);
                    temp_16_1110->unkB4 = func_001F9E90(func_001F9B80(temp_19_1055, temp_18_1054, ((u8 *)temp_16_1110) + 0x80), temp_17_1007->unk18 - temp_17_1007->unk28);
                    temp_16_1110->unkB8 = func_001F9E90(temp_17_1007->unk20 - temp_17_1007->unk10, temp_17_1007->unk24 - temp_17_1007->unk14);
                }
                var_17_904 = &D_0013E030;
            }
            if (var_17_904->unk34 >= 0) {
                if (var_17_904->unk38 >= 0) {
                    temp_f13_1163 = D_0015ED70[0] * 0.666f;
                    func_00213F38(((u8 *)var_17_904) + 0x48, ((u8 *)var_17_904) + 0x4C, 0x3F800000, temp_f13_1163, temp_f13_1163, D_0015ED6C * 0.5f);
                    func_001F9A40(sp_slot, D_00160034 + (var_17_904->unk34 << 7) + 0x30, D_00160034 + (var_17_904->unk38 << 7) + 0x30, var_17_904->unk48);
                    func_001F99F8(sp_slot + 0x10);
                    temp_f13_1198 = func_001FA5C8(*(f32 *)((u8 *)((var_17_904->unk38 << 7) + D_00160034) + 0x74), *(f32 *)((u8 *)((var_17_904->unk34 << 7) + D_00160034) + 0x74)) * var_17_904->unk48;
                    *(f32 *)(sp_slot + 0x14) = temp_f13_1198;
                    *(f32 *)(sp_slot + 0x14) = func_001FA580(*(f32 *)((u8 *)((var_17_904->unk34 << 7) + D_00160034) + 0x74), temp_f13_1198);
                    temp_f13_1220 = func_001FA5C8(*(f32 *)((u8 *)((var_17_904->unk38 << 7) + D_00160034) + 0x78), *(f32 *)((u8 *)((var_17_904->unk34 << 7) + D_00160034) + 0x78)) * var_17_904->unk48;
                    *(f32 *)(sp_slot + 0x18) = temp_f13_1220;
                    *(f32 *)(sp_slot + 0x18) = func_001FA580(*(f32 *)((u8 *)((var_17_904->unk34 << 7) + D_00160034) + 0x78), temp_f13_1220);
                    func_001E93F8(sp_slot);
                    func_001E9400(sp_slot + 0x10);
                }
            }
            temp_17_1234 = &D_0013E030;
            if (temp_17_1234->unk30 >= 0) {
                temp_16_1239 = temp_17_1234->unk24;
                if (temp_16_1239 < func_001F96F8(0x96)) {
                    temp_f20_1249 = func_001FA6C0(temp_17_1234->unk24);
                    temp_16_1264 = temp_17_1234->unk24;
                    var_f20_1267 = (1.0f - fast_cos(temp_f20_1249 * (3.1415927f / func_001FA6C0(func_001F96F8(0x78))))) * 0.5f;
                    if (temp_16_1264 < func_001F96F8(0x78)) {
                        var_f20_1267 = 1.0f;
                    }
                    *(f32 *)((u8 *)D_0013E030.unk0 + 0x54) = var_f20_1267;
                    func_001F9A40(*(s32 *)((u8 *)&D_0013E030) + 0x10, ((u8 *)temp_17_1234) + 0x80, ((u8 *)temp_17_1234) + 0x90, var_f20_1267);
                    *(s32 *)((u8 *)D_0013E030.unk0 + 0x40) = 0;
                    *(f32 *)((u8 *)D_0013E030.unk0 + 0x44) = func_001FA580(temp_17_1234->unkA4, func_001FA5C8(temp_17_1234->unkB4, temp_17_1234->unkA4) * var_f20_1267);
                    *(f32 *)((u8 *)D_0013E030.unk0 + 0x48) = func_001FA580(temp_17_1234->unkA8, func_001FA5C8(temp_17_1234->unkB8, temp_17_1234->unkA8) * var_f20_1267);
                    if (D_0015F43C > 0.0f) {
                        temp_f0_1307 = D_0015F43C - 0.125f;
                        D_0015F43C = temp_f0_1307;
                        if (temp_f0_1307 < 0.0f) {
                            D_0015F43C = 0.0f;
                        }
                    }
                    if (var_f20_1267 < 1.0f) {
                        func_0022F5B0(*(s32 *)((u8 *)&D_0013E030), D_0015ED6C * -3.75f);
                    }
                    var_16_1502 = &D_0013E030;
                } else {
                    temp_f0_1336 = temp_17_1234->unk40 + 0.8f;
                    temp_16_1342 = *((temp_17_1234->unk30 * 4) + D_001CC3B0);
                    temp_17_1234->unk40 = temp_f0_1336;
                    if (temp_f0_1336 > 100.0f) {
                        temp_17_1234->unk40 = 100.0f;
                    }
                    if (D_0013CAE4[0] & 0x50) {
                        if (D_0015F43C < 0.0625f) {
                            D_0015F43C = 0.0625f;
                        }
                    }
                    if ((f32) (*temp_16_1342 - 6) < *(f32 *)((u8 *)(&D_0013E030) + 0x3C)) {
                        if (D_0015F43C < 0.0625f) {
                            D_0015F43C = 0.0625f;
                        }
                    }
                    temp_19_1379 = &D_0013E030;
                    if (temp_19_1379->unk26 < 2) {
                        *(u8 *)((u8 *)D_0013E030.unk0 + 0xBC) = 0x32;
                        *(s16 *)((u8 *)D_0013E030.unk0 + 0xB2) = 0xA;
                        func_001F4600(D_0022E1B0, *(s32 *)((u8 *)&D_0013E030));
                    }
                    temp_f12_1401 = temp_19_1379->unk3C + ((temp_19_1379->unk40 * *(f32 *)&D_0015ED6C) / temp_19_1379->unk44);
                    temp_19_1379->unk3C = temp_f12_1401;
                    if (((f32) (*temp_16_1342 - 1) < temp_f12_1401) || (D_0015F43C >= 1.0f)) {
                        D_0015F618 = 1;
                        D_0015F5B0 = 1;
                        var_16_1502 = &D_0013E030;
                    } else {
                        func_00214E58(temp_16_1342, 1, *(s32 *)((u8 *)&D_0013E030) + 0x10, *(s32 *)((u8 *)&D_0013E030) + 0x40, 0, temp_f12_1401);
                        *(f32 *)((u8 *)D_0013E030.unk0 + 0x40) = *(f32 *)((u8 *)D_0013E030.unk0 + 0x4C);
                        temp_4_1437 = temp_19_1379->unk54;
                        temp_19_1379->unk50 = (s32) ((temp_19_1379->unk50 + 1) & 0x1F);
                        if (temp_4_1437 < 0x20) {
                            temp_19_1379->unk54 = (s32) (temp_4_1437 + 1);
                        }
                        temp_17_1453 = ((u8 *)temp_19_1379) + 0xC0;
                        temp_18_1458 = ((u8 *)temp_19_1379) + 0x2C0;
                        func_001F9CF8((temp_19_1379->unk50 * 0x10) + temp_17_1453, (temp_19_1379->unk26 << 5) + D_001D99B0, *(s32 *)((u8 *)&D_0013E030) + 0xC0);
                        temp_4_1462 = (temp_19_1379->unk50 * 0x10) + temp_17_1453;
                        func_001F9A10(temp_4_1462, temp_4_1462, *(s32 *)((u8 *)&D_0013E030) + 0x10);
                        *(f32 *)((u8 *)(((u8 *)temp_19_1379 + ((temp_19_1379->unk50 * 0x10)))) + 0xCC) = 1.0f;
                        func_001F9CF8((temp_19_1379->unk50 * 0x10) + temp_18_1458, (temp_19_1379->unk26 << 5) + (D_001D99B0 + 0x10), *(s32 *)((u8 *)&D_0013E030) + 0xC0);
                        temp_4_1483 = (temp_19_1379->unk50 * 0x10) + temp_18_1458;
                        func_001F9A10(temp_4_1483, temp_4_1483, *(s32 *)((u8 *)&D_0013E030) + 0x10);
                        *(f32 *)((u8 *)(((u8 *)temp_19_1379 + ((temp_19_1379->unk50 * 0x10)))) + 0x2CC) = 1.0f;
                        func_001F4600(&func_0022E420, *(s32 *)((u8 *)&D_0013E030));
                        var_16_1502 = &D_0013E030;
                        if (D_0015F43C > 0.0f) {
                            temp_f0_1506 = D_0015F43C + 0.0625f;
                            D_0015F43C = temp_f0_1506;
                            if (temp_f0_1506 > 1.0f) {
                                D_0015F43C = 1.0f;
                                var_16_1502 = &D_0013E030;
                            }
                        }
                    }
                }
                temp_4_1517 = ((struct M2c_var_16_1502 *)&D_0013E030)->unk10;
                temp_4_1517->unk40 = *(Vec4 *)((u8 *)D_0013E030.unk0 + 0x40);
                temp_7_1526 = ((struct M2c_var_16_1502 *)&D_0013E030)->unk26 * 0x10;
                func_001F9CF8(((u8 *)temp_4_1517) + 0x10, temp_7_1526 + D_001D9C80, *(s32 *)((u8 *)&D_0013E030) + 0xC0);
                temp_4_1532 = (struct M2c_temp_4_1517 *)((u8 *)((struct M2c_var_16_1502 *)&D_0013E030)->unk10 + 0x10);
                func_001F9A10(temp_4_1532, temp_4_1532, *(s32 *)((u8 *)&D_0013E030) + 0x10);
                func_0020DEF8(((struct M2c_var_16_1502 *)&D_0013E030)->unk10);
                temp_4_1538 = ((struct M2c_var_16_1502 *)&D_0013E030)->unk14;
                temp_4_1538->unk40 = *(Vec4 *)((u8 *)D_0013E030.unk0 + 0x40);
                temp_7_1548 = ((struct M2c_var_16_1502 *)&D_0013E030)->unk26 * 0x10;
                func_001F9CF8(((u8 *)temp_4_1538) + 0x10, temp_7_1548 + D_001D9CB0, *(s32 *)((u8 *)&D_0013E030) + 0xC0);
                temp_4_1554 = (struct M2c_temp_4_1538 *)((u8 *)((struct M2c_var_16_1502 *)&D_0013E030)->unk14 + 0x10);
                func_001F9A10(temp_4_1554, temp_4_1554, *(s32 *)((u8 *)&D_0013E030) + 0x10);
                func_0020DEF8(((struct M2c_var_16_1502 *)&D_0013E030)->unk14);
            }
            func_001EDAA8();
block_180:
            func_0022CA50();
            func_00201A28();
            func_0020CFD0();
            func_001E9428();
            return;
        case 4:
            func_0022EAA8();
            break;
        }
    } else {
    /* case 1: */
    /* case 2: */
    /* case 5: */
    /* case 6: */
    /* case 7: */
    }
}

extern void func_0022F778(void) __attribute__((alias("FUN_0022f778")));
#endif /* NON_MATCHING */
#else
#include "types.h"
#include "sda.h"
#include "qcopy.h"

typedef unsigned int u128 __attribute__((mode(TI)));

typedef union {
    u128 quadword;
    f32 components[4];
    u8 bytes[16];
} Vector4;

typedef struct {
    u8 pad0[6];
    u8 unk6;
    u8 pad7[5];
    u8 count;          /* 0x0C */
    u8 padD[0x48 - 0xD];
    s32 frames[1];     /* 0x48 */
} RenderModel;

typedef struct ResidentRenderObject {
    u8 pad0[0x10];
    Vector4 position;          /* 0x10 */
    u8 state;          /* 0x20 */
    u8 pad21[3];
    RenderModel *model;      /* 0x24 */
    u8 pad28[0xA];
    s16 selected_index;         /* 0x32 */
    u16 flags;         /* 0x34 */
    u8 pad36[2];
    s64 unk38;         /* 0x38 */
    Vector4 rotation;          /* 0x40 */
    u8 current_frame;
    u8 next_frame;
    u8 selected_a;
    u8 selected_b;
    f32 blend;         /* 0x54 */
    u8 pad58[0x71 - 0x58];
    u8 cached_selector;
    u8 opacity;
    u8 unk73;
    u8 pad74[4];
    u8 *animation_positions;         /* 0x78 */
    u8 pad7C[3];
    u8 unk7F;
    u8 pad80[0x10];
    s32 color;         /* 0x90 */
    s32 unk94;         /* 0x94 */
    u8 pad98[0xE];
    s16 class_id;        /* 0xA6 */
    u8 padA8[0xA];
    s16 unkB2;         /* 0xB2 */
    u8 padB4[8];
    u8 unkBC;          /* 0xBC */
    u8 padBD[3];
    Vector4 transform[4];       /* 0xC0 */
} ResidentRenderObject;

typedef struct {
    Vector4 position;
    f32 rotation[4];
} CameraKeyframe;

typedef struct {
    u8 pad0[0x34];
    s32 time;          /* 0x34 */
    s32 frame;         /* 0x38 */
    s32 sequence_frame;         /* 0x3C */
    s16 end;           /* 0x40 */
    u8 pad42[2];
    s16 count;         /* 0x44 */
    u8 pad46[0x54 - 0x46];
    CameraKeyframe *frames;     /* 0x54 */
    u8 pad58[0x178 - 0x58];
    ResidentRenderObject *objects[1];     /* 0x178 */
} ResidentPlaybackState;

typedef struct {
    ResidentRenderObject *player;      /* 0x00 */
    u8 pad4[4];
    ResidentRenderObject *attachment;        /* 0x08 */
    u8 padC[4];
    ResidentRenderObject *companion_a;         /* 0x10 */
    ResidentRenderObject *companion_b;         /* 0x14 */
    u8 pad18[8];
    s32 state;         /* 0x20 */
    s16 timer;         /* 0x24 */
    s16 content_variant;           /* 0x26 */
    s16 skip;          /* 0x28 */
    u8 pad2A[2];
    s16 unk2C;         /* 0x2C */
    u8 pad2E[2];
    s32 path;          /* 0x30 */
    s32 source_camera_index;          /* 0x34 */
    s32 destination_camera_index;            /* 0x38 */
    f32 path_progress;             /* 0x3C */
    f32 speed;         /* 0x40 */
    f32 path_segment_length;           /* 0x44 */
    f32 blend;         /* 0x48 */
    f32 interpolation_velocity;      /* 0x4C */
    s32 trail;         /* 0x50 */
    s32 history_count;      /* 0x54 */
    u8 pad58[8];
    Vector4 unk60;        /* 0x60 */
    Vector4 unk70;        /* 0x70 */
    Vector4 startPos;     /* 0x80 */
    Vector4 pathPos;      /* 0x90 */
    Vector4 startRot;     /* 0xA0 */
    f32 unkB0;
    f32 rotY;          /* 0xB4 */
    f32 rotZ;          /* 0xB8 */
    f32 unkBC;
    Vector4 trailA[32];   /* 0xC0 */
    Vector4 trailB[32];   /* 0x2C0 */
} ResidentCinematicState;

typedef struct {
    s32 point_index;
    s32 pad[3];
    Vector4 p[1];         /* 0x10: x, y, z, angle */
} ScriptedPath;

typedef struct {
    u8 pad0[0x30];
    Vector4 position;          /* 0x30 */
    u8 pad40[0x34];
    f32 rotY;          /* 0x74 */
    f32 rotZ;          /* 0x78 */
    u8 pad7C[4];
} CameraBlendNode;

typedef struct {
    u8 pad0[0x140];
    Vector4 position;          /* 0x140 */
    Vector4 unk150;       /* 0x150 */
    u8 pad160[0x350 - 0x160];
    Vector4 forward;          /* 0x350 */
    Vector4 right;        /* 0x360 */
    Vector4 up;           /* 0x370 */
} ResidentCameraState;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x3A];
    u16 unk5A;
} DialoguePlaybackState;

typedef struct {
    u8 pad0[0x2080];
    ResidentRenderObject *reference_object;
} LevelObjectState;

typedef struct {
    Vector4 a;
    Vector4 b;
} PositionPair;

extern s32 D_0013CAE4[];
extern u8 D_0013D4C0[];
extern ResidentCinematicState D_0013E030;
extern LevelObjectState D_0013F350;
extern u8 D_001413F5[];
extern DialoguePlaybackState D_001516D0;
extern f32 D_0015ED60;
extern f32 D_0015ED6C;
extern f32 D_0015ED70 __attribute__((sda));
extern s32 D_0015ED80;
extern s32 D_0015ED84;
extern u8 D_0015EDB4;
extern f32 D_0015F43C;
extern f32 D_0015F440;
extern s32 D_0015F5B0;
extern s32 D_0015F604;
extern s32 D_0015F618[1];
extern ResidentRenderObject *D_0015FF1C;
extern CameraBlendNode *D_00160034;
extern s32 D_00160510[1] __attribute__((sda));
extern ResidentCameraState D_00186F40;
extern ResidentPlaybackState D_0018CB20;
extern f32 D_0018CDB0[4];
extern ScriptedPath *D_001CC3B0[];
extern s32 D_001D5BF0 NOT_SDA;
extern PositionPair D_001D99B0[];
extern Vector4 D_001D9C80[];
extern Vector4 D_001D9CB0[];
extern u8 D_0022E1B0[];

extern void CalculateDmaTransferAddress(void);
extern void InitializeRenderState(s32);
extern void UpdateRenderStateCommand(void);
extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern s32 sceGsSyncV(s32);
extern void func_001E93F0(void *, void *, s32, s32, s32);
extern void func_001E93F8(void *);
extern void func_001E9400(void *);
extern void func_001E9408(void);
extern void func_001E9410(ResidentRenderObject *);
extern void func_001E9418(ResidentRenderObject *);
extern void func_001E9420(void);
extern void func_001E9428(void);
extern void func_001E9430(void);
extern void func_001E9438(void);
extern void func_001E9440(void *, void *, s32, s32);
extern void func_001E9450(ResidentRenderObject *, ResidentRenderObject *);
extern void update_camera(void) __asm__("func_001EDAA8");
extern void update_view_context(void) __asm__("func_001F2D98");
extern void enqueue_callback_list_1(void *, ResidentRenderObject *) __asm__("FUN_001f4600");
extern void enqueue_callback_list_4(void *, ResidentRenderObject *) __asm__("FUN_001f47b8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern f32 func_001F96E8(f32);
extern s32 scale_ticks(s32) __asm__("func_001F96F8");
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern void func_001F99F8(void *);
extern void add_vectors(void *, void *, void *) __asm__("func_001F9A10");
extern void func_001F9A40(void *, void *, void *, f32);
extern void scale_vector(void *, void *, f32) __asm__("func_001F9A68");
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 func_001F9B80(void *, void *, void *);
extern void func_001F9CF8(void *, void *, void *);
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 func_001F9E90(f32, f32);
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void func_00201A28(void);
extern void parse_space_scene_chunk(s32) __asm__("FUN_002049f0");
extern ResidentRenderObject *func_0020C4F8(s32);
extern void func_0020C828(ResidentRenderObject *);
extern void update_moby_animation_state(ResidentRenderObject *) __asm__("func_0020C880");
extern void func_0020CFD0(void);
extern void refresh_resident_object_spatial_bounds(ResidentRenderObject *) __asm__("func_0020DEF8");
extern void update_visible_resident_objects(void) __asm__("func_00212E28");
extern void update_moby_shadow_range(ResidentRenderObject *) __asm__("func_00213700");
extern void advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern void sample_camera_path(ScriptedPath *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern void update_audio_stream_until_idle(s32) __asm__("FUN_002168a8");
extern void func_00217B88(void);
extern void pause_all_sounds(s32) __asm__("FUN_00218d78");
extern void func_0022CA50(void);
extern void func_0022DC50(void);
extern void update_level_gameplay_frame(void) __asm__("func_0022EAA8");
extern void func_0022F5B0(ResidentRenderObject *, f32);
extern void draw_light_flare() __asm__("func_0022DF40");
extern void build_resident_indexed_texture_warp_meshes() __asm__("func_0022E420");
extern void render_environment_mapped_object() __asm__("func_002327A0");

void update_resident_gameplay_state(void) __asm__("FUN_0022f778");

void update_resident_gameplay_state(void) {
    Vector4 scratch_vectors[4];
    s32 keyframe_flags;
    s32 ticks_per_bank;
    s32 skip;
    s32 object_index;
    s32 point_index;
    ResidentRenderObject *object;
    ResidentRenderObject *object_cursor;
    ResidentRenderObject *expired_object;
    s32 release_index;
    s32 bank_tick_limit;
    ResidentRenderObject *player;
    s32 path_point_index;
    f32 camera_blend_step;
    CameraKeyframe *keyframe;
    f32 *keyframe_angles;
    u8 *animation_positions;
    s32 current_frame;
    ScriptedPath *path;
    f32 segment_angle;
    f32 maximum_turn;
    f32 turn_scale;
    f32 blend;

    D_0013E030.timer++;
    switch (D_0013E030.state) {
    case 0:
    case 8:
        func_001E9430();
        update_visible_resident_objects();
        func_0022DC50();
        func_001E9420();
        func_00217B88();
        D_0015F43C -= 0.125f;
        D_0018CB20.frame++;
        D_0018CB20.time++;
        if (D_0015F43C < 0.0f) {
            D_0015F43C = 0.0f;
        }
        ticks_per_bank = D_0015ED80 ? 0x50 : 0x60;
        skip = D_0013E030.skip;
        if ((D_0013CAE4[0] & 0x50) && scale_ticks(30) < D_0018CB20.time && D_0015F43C == 0.0f) {
            if (D_0013E030.state == 0 && D_0018CB20.time < scale_ticks(D_00160510[D_0013E030.content_variant] - 30)) {
                skip = 1;
            }
            if (D_0013E030.state == 8 && D_0018CB20.time < D_0018CB20.end - scale_ticks(30)) {
                skip = 1;
            }
        }
        if (skip) {
            if (D_001516D0.unk5A != 6 && D_001516D0.unk5A != 7) {
                D_001516D0.unk5A = 5;
            }
            if (D_0013E030.state == 0) {
                D_0018CB20.time = scale_ticks(D_00160510[D_0013E030.content_variant]);
                D_0018CB20.sequence_frame = D_0018CB20.time / ticks_per_bank;
                parse_space_scene_chunk(D_0018CB20.sequence_frame);
                D_0018CB20.frame = D_0018CB20.time % ticks_per_bank;
                if (D_0013E030.skip) {
                    D_0013E030.skip = 0;
                } else {
                    fade_to_black(4);
                }
                D_0015F43C = 1.0f;
            } else {
                D_0018CB20.time = D_0018CB20.end;
            }
        }
        if (D_0018CB20.time >= D_0018CB20.end) {
            if (D_001516D0.unk5A != 6 && D_001516D0.unk5A != 7) {
                D_001516D0.unk5A = 5;
            }
            CalculateDmaTransferAddress();
            D_0018CDB0[0] = 0.63f;
            update_view_context();
            for (release_index = 0; release_index < D_0018CB20.count; release_index++) {
                expired_object = D_0018CB20.objects[release_index];
                if (expired_object != 0) {
                    expired_object->model->count--;
                    expired_object->model->frames[expired_object->model->count] = 0;
                    func_0020C828(expired_object);
                }
                if (D_0013E030.attachment != 0) {
                    func_0020C828(D_0013E030.attachment);
                }
            }
            D_0013E030.player->flags &= ~1;
            for (object_cursor = D_0015FF1C; object_cursor->state != 0xFF; object_cursor++) {
                if (!(object_cursor->state & 0x80) && (object_cursor->class_id == 0x4A || object_cursor->class_id == 0xCB)) {
                    object_cursor->flags &= ~0x80;
                }
            }
            if (D_0013E030.state == 0) {
                pause_all_sounds(0);
                D_001D5BF0 = 0xE;
                D_0015F618[0] = 1;
                return;
            }
            D_0015F604 = 0;
            UpdateRenderStateCommand();
            D_0015F618[0] = 1;
            D_001413F5[0] = 0;
            func_001E9438();
            if (D_0013E030.unk2C != 0) {
                func_001E9408();
                return;
            }
            func_001E9440(&D_0013E030.unk60, &D_0013E030.unk70, 0, 1);
            return;
        }
        bank_tick_limit = D_0015ED80 ? 0x50 : 0x60;
        if (D_0018CB20.frame >= bank_tick_limit) {
            parse_space_scene_chunk(++D_0018CB20.sequence_frame);
        }
        keyframe = &D_0018CB20.frames[D_0018CB20.frame];
        keyframe_angles = keyframe->rotation;
        keyframe_flags = keyframe->position.bytes[12];
        D_0018CDB0[0] = keyframe_angles[3];
        update_view_context();
        qcopy(&D_00186F40.position, &keyframe->position);
        func_001F9CF8(&D_00186F40.position, &D_00186F40.position, D_0013E030.player->transform);
        add_vectors(&D_00186F40.position, &D_00186F40.position, &D_0013E030.player->position);
        segment_angle = fast_add_rotations(keyframe_angles[2], D_0013E030.player->rotation.components[2]);
        sceVu0UnitMatrix(scratch_vectors);
        SceVu0RotMatrixX(scratch_vectors, scratch_vectors, keyframe->rotation[0]);
        SceVu0RotMatrixY(scratch_vectors, scratch_vectors, keyframe_angles[1]);
        sceVu0RotMatrixZ(scratch_vectors, scratch_vectors, segment_angle);
        D_00186F40.forward.components[0] = -scratch_vectors[2].components[0];
        D_00186F40.right.components[0] = -scratch_vectors[0].components[0];
        D_00186F40.up.components[0] = scratch_vectors[1].components[0];
        D_00186F40.forward.components[1] = -scratch_vectors[2].components[1];
        D_00186F40.right.components[1] = -scratch_vectors[0].components[1];
        D_00186F40.up.components[1] = scratch_vectors[1].components[1];
        D_00186F40.forward.components[2] = -scratch_vectors[2].components[2];
        D_00186F40.right.components[2] = -scratch_vectors[0].components[2];
        D_00186F40.up.components[2] = scratch_vectors[1].components[2];
        if (D_0015EDB4 != 0) {
            fast_vec_cross(&D_00186F40.right, &D_00186F40.up, &D_00186F40.forward);
        }
        for (object_index = 0; object_index < D_0018CB20.count; object_index++) {
            object = D_0018CB20.objects[object_index];
            current_frame = D_0018CB20.frame >> 1;
            object->current_frame = current_frame;
            object->next_frame = current_frame + 1;
            update_moby_animation_state(object);
            object->blend = convert_integer_to_float(D_0018CB20.frame & 1) * 0.5f;
            if (keyframe_flags != 0 && (D_0018CB20.frame & 1)) {
                object->blend = 1.0f;
            }
            animation_positions = object->animation_positions;
            scale_vector(&scratch_vectors[0], animation_positions + object->current_frame * 16, 1.0f - object->blend);
            scale_vector(&scratch_vectors[1], animation_positions + object->next_frame * 16, object->blend);
            add_vectors(&object->position, &scratch_vectors[0], &scratch_vectors[1]);
            func_001F9CF8(&object->position, &object->position, D_0013E030.player->transform);
            add_vectors(&object->position, &object->position, &D_0013E030.player->position);
            object->rotation.components[2] = D_0013E030.player->rotation.components[2];
            object->cached_selector = 0xFF;
            refresh_resident_object_spatial_bounds(object);
            if (object->class_id == 0) {
                if (D_0013E030.state == 0) {
                    if (D_0015ED84 == 10 && D_0013D4C0[6] == 0) {
                        object->unk7F = 0;
                    } else if (D_0018CB20.time <= scale_ticks(0x3E)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (scale_ticks(360) < D_0018CB20.time) {
                    if (D_0015ED84 == 10 && D_0013D4C0[6] == 0) {
                        object->unk7F = 0;
                    } else {
                        object->unk7F = 0x18;
                    }
                } else {
                    object->unk7F = 0;
                }
            } else if (object->class_id == 10) {
                if (D_0013E030.state == 0) {
                    if (D_0018CB20.time <= scale_ticks(350)) {
                        object->unk7F = 0x18;
                    } else {
                        object->unk7F = 0;
                    }
                } else if (D_0018CB20.time <= scale_ticks(524)) {
                    object->unk7F = 0;
                } else {
                    object->unk7F = 0x18;
                }
            }
            if (object->unk7F != 0) {
                update_moby_shadow_range(object);
            }
            if (object->class_id == 10) {
                func_001E9418(object);
            }
            if (object->class_id == 0) {
                func_001E9410(object);
                if ((D_0015ED84 == 10 && D_0013D4C0[6] != 0) || D_0015ED84 == 13) {
                    if (D_0013E030.attachment == 0) {
                        D_0013E030.attachment = func_0020C4F8(0x509);
                        D_0013E030.attachment->selected_index = 0x40;
                        D_0013E030.attachment->flags |= 0x806;
                        D_0013E030.attachment->unk38 = object->unk38;
                        if (D_0013E030.attachment->model->unk6 != 0) {
                            D_0013E030.attachment->unk73 = 0x18;
                        }
                    }
                    func_001E9450(object, D_0013E030.attachment);
                }
            }
            if ((u16)object->class_id - 0x213U < 3) {
                enqueue_callback_list_4(render_environment_mapped_object, object);
                enqueue_callback_list_1(draw_light_flare, object);
                if (D_0013E030.state == 0) {
                    if (D_0018CB20.time < scale_ticks(360)) {
                        s32 color_intensity = (s32)(fast_cos(convert_integer_to_float((D_0018CB20.time & 0x3F) - 32) * 0.09817477f) * 80.0f) + 120;
                        if (object->class_id == 0x215) {
                            object->color = (color_intensity >> 1) | (color_intensity << 8) | (color_intensity << 16);
                        } else {
                            object->color = color_intensity | (color_intensity << 8) | (color_intensity << 16);
                        }
                    } else if (D_0018CB20.time < scale_ticks(0x1F8)) {
                        s32 color_intensity = (s32)(((f32)D_0018CB20.time - func_001F96E8(360.0f)) * (D_0015ED60 * 1.25f));
                        if (color_intensity >= 256) {
                            color_intensity = 255;
                        }
                        if (object->class_id == 0x215) {
                            object->color = (color_intensity << 8) | (color_intensity << 16);
                        } else {
                            object->color = color_intensity | (color_intensity << 8) | (color_intensity << 16);
                        }
                        if (scale_ticks(0x1A4) < D_0018CB20.time) {
                            D_0013E030.player->unkBC = (s32)(((f32)D_0018CB20.time - func_001F96E8(420.0f)) * (D_0015ED60 * 1.2f));
                            D_0013E030.player->unkB2 = 0;
                            enqueue_callback_list_1(D_0022E1B0, D_0013E030.player);
                        }
                        if (scale_ticks(0x1D0) < D_0018CB20.time) {
                            D_0015F440 += 0.025f;
                            if (D_0015F440 > 1.0f) {
                                D_0015F440 = 1.0f;
                            }
                        }
                    } else {
                        object->color = 0xA0A0A0;
                        D_0015F440 -= 0.025f;
                        if (D_0015F440 < 0.0f) {
                            D_0015F440 = 0.0f;
                        }
                    }
                } else if (D_0018CB20.time < scale_ticks(0xF0)) {
                    s32 color_intensity;
                    if (D_0018CB20.time < scale_ticks(0xC8)) {
                        func_0022F5B0(object, D_0015ED6C * -4.0f);
                        func_0022F5B0(object, D_0015ED6C * -3.0f);
                    }
                    color_intensity = (s32)(fast_cos(convert_integer_to_float((D_0018CB20.time & 0x3F) - 32) * 0.09817477f) * 80.0f) + 120;
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity >> 1) | (color_intensity << 8) | (color_intensity << 16);
                    } else {
                        object->color = color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                    object->unkBC = 0x32;
                    object->unkB2 = 10;
                    enqueue_callback_list_1(D_0022E1B0, object);
                } else if (D_0018CB20.time <= scale_ticks(360)) {
                    s32 color_intensity;
                    if (D_0018CB20.time < scale_ticks(300)) {
                        D_0013E030.player->unkBC = (s32)((func_001F96E8(300.0f) - (f32)D_0018CB20.time) * (D_0015ED60 * 1.5f));
                        D_0013E030.player->unkB2 = 0;
                        enqueue_callback_list_1(D_0022E1B0, D_0013E030.player);
                    }
                    color_intensity = (s32)((func_001F96E8(360.0f) - (f32)D_0018CB20.time) * (D_0015ED60 * 1.25f));
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity << 8) | (color_intensity << 16);
                    } else {
                        object->color = color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                } else {
                    s32 color_intensity = (s32)(fast_cos(convert_integer_to_float((D_0018CB20.time & 0x3F) - 32) * 0.09817477f) * 80.0f) + 120;
                    if (object->class_id == 0x215) {
                        object->color = (color_intensity >> 1) | (color_intensity << 8) | (color_intensity << 16);
                    } else {
                        object->color = color_intensity | (color_intensity << 8) | (color_intensity << 16);
                    }
                }
            }
        }
        func_0022CA50();
        func_00201A28();
        func_0020CFD0();
        func_001E9428();
        break;

    case 3:
        func_001E9430();
        update_visible_resident_objects();
        func_0022DC50();
        func_001E9420();
        func_00217B88();
        if (D_0013E030.timer == 0) {
            InitializeRenderState(0);
            update_audio_stream_until_idle(0);
            D_001516D0.unk1C = D_0013E030.content_variant + 0x9C4F;
            fade_to_black(scale_ticks(12));
            while ((s16)D_001516D0.unk5A != 3) {
                func_0022CA50();
                sceGsSyncV(0);
            }
            continue_audio_stream_if_ready();
            D_0013E030.player->selected_a = 1;
            D_0013E030.player->selected_b = 2;
            D_0013E030.player->blend = 0.0f;
            update_moby_animation_state(D_0013E030.player);
            D_0013E030.player->opacity = 0xFF;
            D_0013E030.player->unk94 = 0;
            if (D_0013E030.path >= 0) {
                D_0013E030.companion_a = func_0020C4F8(0);
                D_0013E030.companion_a->selected_index = 0x1FF;
                D_0013E030.companion_a->opacity = 0xFF;
                D_0013E030.companion_a->unk94 = 0;
                D_0013E030.companion_a->flags |= 6;
                D_0013E030.companion_a->unk38 = D_0013F350.reference_object->unk38;
                D_0013E030.companion_b = func_0020C4F8(10);
                D_0013E030.companion_b->selected_index = 0x1FF;
                D_0013E030.companion_b->opacity = 0xFF;
                D_0013E030.companion_b->unk94 = 0;
                D_0013E030.companion_b->flags |= 6;
                D_0013E030.companion_b->unk38 = D_0013F350.reference_object->unk38;
            }
            func_001E93F0(&D_00186F40.position, &D_00186F40.unk150, 1, 0, 0);
            D_0013E030.blend = 0.0f;
            D_0013E030.interpolation_velocity = 0.0f;
            if (D_0013E030.path >= 0) {
                path = D_001CC3B0[D_0013E030.path];
                maximum_turn = D_0013E030.blend;
                path->p[0].components[3] = maximum_turn;
                for (path_point_index = 1; path_point_index < path->point_index - 1; path_point_index++) {
                    segment_angle = func_001F9E90(path->p[path_point_index].components[0] - path->p[path_point_index - 1].components[0], path->p[path_point_index].components[1] - path->p[path_point_index - 1].components[1]);
                    path->p[path_point_index].components[3] = fast_subtract_rotations(func_001F9E90(path->p[path_point_index + 1].components[0] - path->p[path_point_index].components[0], path->p[path_point_index + 1].components[1] - path->p[path_point_index].components[1]), segment_angle);
                    if (maximum_turn < AbsoluteFloat(path->p[path_point_index].components[3])) {
                        maximum_turn = AbsoluteFloat(path->p[path_point_index].components[3]);
                    }
                }
                segment_angle = func_001F9E90(path->p[path->point_index - 1].components[0] - path->p[path->point_index - 2].components[0], path->p[path->point_index - 1].components[1] - path->p[path->point_index - 2].components[1]);
                path->p[path->point_index - 1].components[3] = fast_subtract_rotations(func_001F9E90(path->p[0].components[0] - path->p[path->point_index - 1].components[0], path->p[0].components[1] - path->p[path->point_index - 1].components[1]), segment_angle);
                turn_scale = 0.34906584f / maximum_turn;
                for (point_index = 0; point_index < path->point_index; point_index++) {
                    path->p[point_index].components[3] *= turn_scale;
                }
                D_0013E030.path_segment_length = distance_xyz(&path->p[0], &path->p[1]);
                D_0013E030.path_progress = 0.0f;
                D_0013E030.speed = 0.0f;
                D_0013E030.trail = 0;
                D_0013E030.history_count = 0;
                player = D_0013E030.player;
                qcopy(&D_0013E030.startPos, &player->position);
                qcopy(&D_0013E030.pathPos, &path->p[0]);
                qcopy(&D_0013E030.startRot, &player->rotation);
                D_0013E030.rotY = func_001F9E90(func_001F9B80(&path->p[1], &path->p[0], &D_0013E030.startPos), path->p[0].components[2] - path->p[1].components[2]);
                D_0013E030.rotZ = func_001F9E90(path->p[1].components[0] - path->p[0].components[0], path->p[1].components[1] - path->p[0].components[1]);
            }
        }
        if (D_0013E030.source_camera_index >= 0 && D_0013E030.destination_camera_index >= 0) {
            camera_blend_step = D_0015ED70 * 0.666f;
            advance_accelerated_scalar(&D_0013E030.blend, &D_0013E030.interpolation_velocity, 1.0f, camera_blend_step, camera_blend_step, D_0015ED6C * 0.5f);
            func_001F9A40(&scratch_vectors[0], &D_00160034[D_0013E030.source_camera_index].position, &D_00160034[D_0013E030.destination_camera_index].position, D_0013E030.blend);
            func_001F99F8(&scratch_vectors[1]);
            scratch_vectors[1].components[1] = fast_subtract_rotations(D_00160034[D_0013E030.destination_camera_index].rotY, D_00160034[D_0013E030.source_camera_index].rotY) * D_0013E030.blend;
            scratch_vectors[1].components[1] = fast_add_rotations(D_00160034[D_0013E030.source_camera_index].rotY, scratch_vectors[1].components[1]);
            scratch_vectors[1].components[2] = fast_subtract_rotations(D_00160034[D_0013E030.destination_camera_index].rotZ, D_00160034[D_0013E030.source_camera_index].rotZ) * D_0013E030.blend;
            scratch_vectors[1].components[2] = fast_add_rotations(D_00160034[D_0013E030.source_camera_index].rotZ, scratch_vectors[1].components[2]);
            func_001E93F8(&scratch_vectors[0]);
            func_001E9400(&scratch_vectors[1]);
        }
        if (D_0013E030.path >= 0) {
            if (D_0013E030.timer < scale_ticks(150)) {
                blend = (1.0f - fast_cos(convert_integer_to_float(D_0013E030.timer) * (3.1415927f / convert_integer_to_float(scale_ticks(120))))) * 0.5f;
                if (D_0013E030.timer >= scale_ticks(120)) {
                    blend = 1.0f;
                }
                D_0013E030.player->blend = blend;
                func_001F9A40(&D_0013E030.player->position, &D_0013E030.startPos, &D_0013E030.pathPos, blend);
                D_0013E030.player->rotation.components[0] = 0.0f;
                D_0013E030.player->rotation.components[1] = fast_add_rotations(D_0013E030.startRot.components[1], fast_subtract_rotations(D_0013E030.rotY, D_0013E030.startRot.components[1]) * blend);
                D_0013E030.player->rotation.components[2] = fast_add_rotations(D_0013E030.startRot.components[2], fast_subtract_rotations(D_0013E030.rotZ, D_0013E030.startRot.components[2]) * blend);
                if (D_0015F43C > 0.0f) {
                    D_0015F43C -= 0.125f;
                    if (D_0015F43C < 0.0f) {
                        D_0015F43C = 0.0f;
                    }
                }
                if (blend < 1.0f) {
                    func_0022F5B0(D_0013E030.player, D_0015ED6C * -3.75f);
                }
            } else {
                ScriptedPath *active_path;
                D_0013E030.speed += 0.8f;
                active_path = D_001CC3B0[D_0013E030.path];
                if (D_0013E030.speed > 100.0f) {
                    D_0013E030.speed = 100.0f;
                }
                if ((D_0013CAE4[0] & 0x50) && D_0015F43C < 0.0625f) {
                    D_0015F43C = 0.0625f;
                }
                if ((f32)(active_path->point_index - 6) < D_0013E030.path_progress && D_0015F43C < 0.0625f) {
                    D_0015F43C = 0.0625f;
                }
                if (D_0013E030.content_variant < 2) {
                    D_0013E030.player->unkBC = 0x32;
                    D_0013E030.player->unkB2 = 10;
                    enqueue_callback_list_1(D_0022E1B0, D_0013E030.player);
                }
                D_0013E030.path_progress += D_0013E030.speed * D_0015ED6C / D_0013E030.path_segment_length;
                if ((f32)(active_path->point_index - 1) < D_0013E030.path_progress || D_0015F43C >= 1.0f) {
                    D_0015F5B0 = 1;
                    D_0015F618[0] = 1;
                } else {
                    sample_camera_path(active_path, 1, &D_0013E030.player->position, &D_0013E030.player->rotation, 0, D_0013E030.path_progress);
                    D_0013E030.player->rotation.components[0] = D_0013E030.player->rotation.components[3];
                    D_0013E030.trail = (D_0013E030.trail + 1) & 0x1F;
                    if (D_0013E030.history_count < 0x20) {
                        D_0013E030.history_count++;
                    }
                    func_001F9CF8(&D_0013E030.trailA[D_0013E030.trail], &D_001D99B0[D_0013E030.content_variant].a, D_0013E030.player->transform);
                    add_vectors(&D_0013E030.trailA[D_0013E030.trail], &D_0013E030.trailA[D_0013E030.trail], &D_0013E030.player->position);
                    D_0013E030.trailA[D_0013E030.trail].components[3] = 1.0f;
                    func_001F9CF8(&D_0013E030.trailB[D_0013E030.trail], &D_001D99B0[D_0013E030.content_variant].b, D_0013E030.player->transform);
                    add_vectors(&D_0013E030.trailB[D_0013E030.trail], &D_0013E030.trailB[D_0013E030.trail], &D_0013E030.player->position);
                    D_0013E030.trailB[D_0013E030.trail].components[3] = 1.0f;
                    enqueue_callback_list_1(build_resident_indexed_texture_warp_meshes, D_0013E030.player);
                    if (D_0015F43C > 0.0f) {
                        D_0015F43C += 0.0625f;
                        if (D_0015F43C > 1.0f) {
                            D_0015F43C = 1.0f;
                        }
                    }
                }
            }
            {
                ResidentRenderObject *p = D_0013E030.player;
                ResidentRenderObject *scratch_vectors = D_0013E030.companion_a;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                func_001F9CF8(&scratch_vectors->position, &D_001D9C80[D_0013E030.content_variant], p->transform);
            }
            add_vectors(&D_0013E030.companion_a->position, &D_0013E030.companion_a->position, &D_0013E030.player->position);
            refresh_resident_object_spatial_bounds(D_0013E030.companion_a);
            {
                ResidentRenderObject *scratch_vectors = D_0013E030.companion_b;
                ResidentRenderObject *p = D_0013E030.player;
                qcopy(&scratch_vectors->rotation, &p->rotation);
                func_001F9CF8(&scratch_vectors->position, &D_001D9CB0[D_0013E030.content_variant], p->transform);
            }
            add_vectors(&D_0013E030.companion_b->position, &D_0013E030.companion_b->position, &D_0013E030.player->position);
            refresh_resident_object_spatial_bounds(D_0013E030.companion_b);
        }
        update_camera();
        func_0022CA50();
        func_00201A28();
        func_0020CFD0();
        func_001E9428();
        break;

    case 4:
        update_level_gameplay_frame();
        break;
    case 1:
    case 2:
    case 5:
    case 6:
    case 7:
        break;
    }
}
#endif /* NON_MATCHING */
