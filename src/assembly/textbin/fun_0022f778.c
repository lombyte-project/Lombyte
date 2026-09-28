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
extern f32 func_001F9DC8(f32);
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
extern s32 func_00218D78();
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
                    func_00218D78(0, var_5_227, 0x4A);
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
                            var_4_628 = (s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
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
                        temp_4_775 = (s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
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
                        var_4_628 = (s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f) + 0x78;
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
                    var_f20_1267 = (1.0f - func_001F9DC8(temp_f20_1249 * (3.1415927f / func_001FA6C0(func_001F96F8(0x78))))) * 0.5f;
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
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct M2c_D_0013D4C0
{
  u8 pad_0[0x6];
  u8 unk6;
};
struct M2c_D_0013E030
{
  struct M2c_D_0013E030_unk0 *unk0;
  u8 pad_4[0x4];
  struct M2c_D_0013E030_unk8 *unk8;
  u8 pad_C[0x14];
  s32 unk20;
  u16 unk24;
  s16 unk26;
  s16 unk28;
  u8 pad_2A[0x2];
  s16 unk2C;
  u8 pad_2E[0x2];
};
struct M2c_D_0013E030_unk0
{
  u8 pad_0[0x34];
  u16 unk34;
  u8 pad_36[2];
  u8 pad_38[0x10];
  f32 unk48;
  u8 pad_4C[0x66];
  s16 unkB2;
  u8 pad_B4[0x8];
  s8 unkBC;
};
struct M2c_D_0013E030_unk8
{
  u8 pad_0[0x24];
  struct M2c_D_0013E030_unk8_unk24 *unk24;
  u8 pad_28[0xA];
  u16 unk32;
  u16 unk34;
  s64 unk38;
  u8 pad_40[0x33];
  u8 unk73;
};
struct M2c_D_0013E030_unk8_unk24
{
  u8 pad_0[0x6];
  u8 unk6;
};
struct M2c_D_0013F350
{
  u8 pad_0[0x2080];
  struct M2c_D_0013F350_unk2080 *unk2080;
};
struct M2c_D_0013F350_unk2080
{
  u8 pad_0[0x38];
  s64 unk38;
};
struct M2c_D_001516D0
{
  u8 pad_0[0x1C];
  s32 unk1C;
  u8 pad_20[0x3A];
  u16 unk5A;
};
struct M2c_D_0018CB20
{
  u8 pad_0[0x34];
  s32 unk34;
  s32 unk38;
  s32 unk3C;
  s16 unk40;
  u8 pad_42[2];
  s16 unk44;
  u8 pad_46[0xE];
  s32 unk54;
};
struct M2c_temp_16_1014
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
  u8 pad_18[0x4];
  f32 unk1C;
};
typedef struct __attribute__((aligned(16))) 
{
  f32 x;
  f32 y;
  f32 z;
  f32 w;
} Vec4;
struct M2c_temp_16_1110
{
  u8 pad_0[0x3C];
  s32 unk3C;
  s32 unk40;
  f32 unk44;
  u8 pad_48[0x8];
  s32 unk50;
  s32 unk54;
  u8 pad_58[0x28];
  Vec4 unk80;
  Vec4 unk90;
  Vec4 unkA0;
  u8 pad_A8[0x4];
  f32 unkB4;
  f32 unkB8;
};
struct M2c_temp_16_308
{
  s32 unk0;
  u8 pad_4[0x8];
  s32 unkC;
  f32 unk10;
};
struct M2c_temp_16_893
{
  u8 pad_0[0x24];
  s16 unk24;
  s16 unk26;
};
struct M2c_temp_17_1007
{
  s32 unk0;
  u8 pad_4[0xC];
  f32 unk10;
  f32 unk14;
  f32 unk18;
  f32 unk1C;
  f32 unk20;
  f32 unk24;
  f32 unk28;
};
struct M2c_temp_17_1234
{
  u8 pad_0[0x24];
  s16 unk24;
  u8 pad_26[0xA];
  s32 unk30;
  u8 pad_34[0xC];
  f32 unk40;
  u8 pad_44[0x60];
  f32 unkA4;
  f32 unkA8;
  u8 pad_AC[0x8];
  f32 unkB4;
  f32 unkB8;
};
struct M2c_temp_17_310
{
  u8 pad_0[0x4];
  f32 unk4;
  f32 unk8;
  f32 unkC;
};
struct M2c_temp_18_399
{
  u8 pad_0[0x38];
  s64 unk38;
  u8 pad_40[0x8];
  f32 unk48;
  u8 pad_4C[0x4];
  u8 unk50;
  u8 unk51;
  u8 pad_52[0x2];
  f32 unk54;
  u8 pad_58[0x19];
  u8 unk71;
  u8 pad_72[0x6];
  s32 unk78;
  u8 pad_7C[0x3];
  u8 unk7F;
  u8 pad_80[0x10];
  s32 unk90;
  u8 pad_94[0x12];
  s16 unkA6;
  u8 pad_A8[0xA];
  s16 unkB2;
  u8 pad_B4[0x8];
  u8 unkBC;
};
struct M2c_temp_18_940
{
  u8 pad_0[0x10];
  struct M2c_temp_18_940_unk10 *unk10;
  struct M2c_temp_18_940_unk14 *unk14;
  u8 pad_18[0x18];
  s32 unk30;
  u8 pad_34[0x14];
  s32 unk48;
  s32 unk4C;
};
struct M2c_temp_18_940_unk10
{
  u8 pad_0[0x72];
  u8 unk72;
  u8 pad_73[0x21];
  s32 unk94;
};
struct M2c_temp_18_940_unk14
{
  u8 pad_0[0x38];
  s64 unk38;
  u8 pad_40[0x32];
  u8 unk72;
  u8 pad_73[0x21];
  s32 unk94;
};
struct M2c_temp_19_1379
{
  u8 pad_0[0x26];
  s16 unk26;
  u8 pad_2A[0x12];
  f32 unk3C;
  f32 unk40;
  f32 unk44;
  u8 pad_48[0x8];
  s32 unk50;
  s32 unk54;
};
struct M2c_temp_2_1026
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
};
struct M2c_temp_2_1061
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
};
struct M2c_temp_2_1075
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
};
struct M2c_temp_2_349
{
  u8 pad_0[0x350];
  f32 unk350;
  f32 unk354;
  f32 unk358;
  u8 pad_35C[0x4];
  f32 unk360;
  f32 unk364;
  f32 unk368;
  u8 pad_36C[0x4];
  f32 unk370;
  f32 unk374;
  f32 unk378;
};
struct M2c_temp_2_570
{
  u8 pad_0[0x32];
  u16 unk32;
};
struct M2c_temp_2_955
{
  u8 pad_0[0x32];
  u16 unk32;
};
struct M2c_temp_2_973
{
  u8 pad_0[0x32];
  u16 unk32;
};
struct M2c_temp_3_1015
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
};
struct M2c_temp_3_1060
{
  u8 pad_0[0x10];
  f32 unk10;
  f32 unk14;
};
struct M2c_temp_3_198
{
  u8 pad_0[0x24];
  struct M2c_temp_3_205 *unk24;
};
struct M2c_temp_3_205
{
  u8 pad_0[0xC];
  u8 unkC;
};
struct M2c_temp_4_1517
{
  u8 pad_0[0x40];
  Vec4 unk40;
};
struct M2c_temp_4_1538
{
  u8 pad_0[0x40];
  Vec4 unk40;
};
struct M2c_temp_4_981
{
  u8 pad_0[0x34];
  u16 unk34;
};
struct M2c_temp_5_201
{
  u8 pad_0[0xC];
  u8 unkC;
};
struct M2c_temp_5_966
{
  u8 pad_0[0x34];
  u16 unk34;
};
struct M2c_temp_5_971
{
  u8 pad_0[0x38];
  s64 unk38;
};
struct M2c_unk24
{
  u8 pad_0[0x6];
  u8 unk6;
};
struct M2c_var_16_1502
{
  u8 pad_0[0x10];
  struct M2c_temp_4_1517 *unk10;
  struct M2c_temp_4_1538 *unk14;
  u8 pad_18[0xE];
  s16 unk26;
};
struct M2c_var_17_904
{
  u8 pad_0[0x34];
  s32 unk34;
  s32 unk38;
  u8 pad_3C[0xC];
  f32 unk48;
};
struct M2c_var_3_1096
{
  u8 pad_0[0x1C];
  f32 unk1C;
};
struct M2c_var_5_227
{
  u8 pad_0[0x20];
  u8 unk20;
  u8 pad_21[0x13];
  u16 unk34;
  u8 pad_36[0x70];
  s16 unkA6;
};
extern s32 D_0013CAE4[];
extern struct M2c_D_0013D4C0 D_0013D4C0;
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_0013F350 D_0013F350;
extern u8 D_001413F5[];
extern struct M2c_D_001516D0 D_001516D0;
extern f32 D_0015ED60;
extern f32 D_0015ED6C;
extern f32 D_0015ED70[1];
extern volatile int D_0015ED80;
extern s32 D_0015ED84;
extern u8 D_0015EDB4;
extern f32 D_0015F43C;
extern f32 D_0015F440;
extern s32 D_0015F5B0;
extern s32 D_0015F604;
extern s32 D_0015F618[1];
extern s32 D_0015FF1C[];
extern s32 D_00160034;
extern s32 D_00160510[1];
extern u8 D_00187080[];
extern struct M2c_D_0018CB20 D_0018CB20;
extern f32 D_0018CDB0[4];
extern u8 D_001CC3B0[];
extern s32 D_001D5BF0[1];
extern u8 D_001D99B0[];
extern unsigned int D_001D9C80[];
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
extern void func_001F4600();
extern s32 func_001F47B8();
extern s32 func_001F4A58();
extern f32 func_001F96E8(f32);
extern s32 func_001F96F8();
extern f32 func_001F99C0(f32);
extern void func_001F99F8();
extern void func_001F9A10(void *, void *, void *);
extern void func_001F9A40(void *, void *, void *, f32);
extern void func_001F9A68(void *, void *, f32);
extern s32 func_001F9AD8();
extern f32 func_001F9B48(void *, void *);
extern f32 func_001F9B80(void *, void *, void *);
extern void func_001F9CF8(void *, void *, void *);
extern f32 func_001F9DC8(f32);
extern f32 func_001F9E90(f32, f32);
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA5C8(f32, f32);
extern f32 func_001FA6C0(s32);
extern s32 func_00201A28();
extern s32 func_002049F0();
extern s32 func_0020C4F8();
extern s32 func_0020C828();
extern void func_0020C880();
extern s32 func_0020CFD0();
extern s32 func_0020DEF8();
extern s32 func_00212E28();
extern s32 func_00213700();
extern void func_00213F38(void *, void *, f32, f32, f32, f32);
extern void func_00214E58(void *, s32, void *, void *, s32, f32);
extern s32 func_00215B10();
extern s32 func_002168A8();
extern void func_00217B88();
extern s32 func_00218D78();
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
inline int inline_fn(s32 arg0, int arg1, s32 arg2)
{
  return (arg0 << arg1) | (arg2 << 0x10);
}

inline u8 *inline_fn2(struct M2c_D_0013E030 *arg0)
{
  return (u8 *) arg0;
}

void FUN_0022f778(void)
{
  u8 sp_slot[0x40];
  s32 sp40;
  f32 *sp44;
  s32 var_4_225;
  f32 *temp_17_430;
  struct M2c_temp_2_349 *temp_2_349;
  s16 new_var8;
  u8 *temp_4_1462;
  u8 *temp_4_1483;
  f32 *temp_4_1532;
  f32 *temp_4_1554;
  s32 temp_5_661;
  f32 temp_f0_1016;
  f32 temp_f0_1035;
  s32 new_var5;
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
  register f32 temp_f20_333 asm("$21");
  unsigned long pad;
  f32 var_f20_1267;
  f32 var_f21_1004;
  f32 temp_f1_1084;
  f32 new_var12;
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
  int new_var14;
  u8 *new_var9;
  s32 temp_3_997;
  s32 temp_4_1437;
  s32 temp_4_29;
  register s32 temp_4_775;
  s32 temp_5_913;
  register s32 temp_7_1526 asm("s2");
  struct M2c_var_17_904 *new_var10;
  register long long temp_7_1548 asm("s1");
  s32 temp_lo_134;
  register s32 var_16_184 asm("s0");
  register s32 var_18_1006 asm("s2");
  register s32 var_22_383 asm("s6");
  register unsigned short var_2_236 asm("s0");
  s32 var_2_634;
  s32 var_2_664;
  register s32 var_2_781 asm("v0");
  s32 var_2_853;
  register u8 *new_var6 asm("v0");
  s32 var_3_662;
  s32 var_3_855;
  register s32 var_4_1085 asm("s3");
  s32 var_4_628;
  struct M2c_D_0013E030 *new_var4;
  s32 var_condition_bit_717;
  f32 new_var3;
  u8 temp_2_231;
  u8 temp_2_251;
  volatile unsigned char pad2;
  u8 temp_4_309;
  u8 *var_17_196;
  struct M2c_temp_16_1014 *temp_16_1014;
  struct M2c_temp_16_1110 *temp_16_1110;
  int new_var11;
  register struct M2c_temp_16_308 *temp_16_308 asm("s6");
  register struct M2c_temp_16_893 *temp_16_893 asm("s1");
  struct M2c_temp_17_1007 *temp_17_1007;
  struct M2c_temp_17_1234 *temp_17_1234;
  u8 *temp_17_1453;
  struct M2c_temp_17_310 *temp_17_310;
  s32 *temp_18_1054;
  u8 *temp_18_1458;
  struct M2c_temp_18_399 *temp_18_399;
  struct M2c_temp_18_940 *temp_18_940;
  s32 *temp_19_1055;
  struct M2c_temp_19_1379 *temp_19_1379;
  struct M2c_temp_2_1026 *temp_2_1026;
  struct M2c_temp_2_1061 *temp_2_1061;
  struct M2c_temp_16_308 *new_var;
  register struct M2c_temp_2_1075 *temp_2_1075 asm("s3");
  struct M2c_temp_2_570 *temp_2_570;
  struct M2c_temp_2_955 *temp_2_955;
  struct M2c_temp_2_973 *temp_2_973;
  int new_var2;
  u8 *new_var15;
  struct M2c_temp_3_1015 *temp_3_1015;
  float new_var7;
  register u8 *new_var16 asm("a0");
  struct M2c_temp_3_1060 *temp_3_1060;
  struct M2c_temp_3_198 *temp_3_198;
  struct M2c_temp_3_205 *temp_3_205;
  struct M2c_temp_4_1517 *temp_4_1517;
  register struct M2c_temp_4_1538 *temp_4_1538 asm("s1");
  struct M2c_temp_4_981 *temp_4_981;
  register struct M2c_temp_5_201 *temp_5_201 asm("a1");
  struct M2c_temp_5_966 *temp_5_966;
  register struct M2c_temp_5_971 *temp_5_971 asm("a1");
  struct M2c_var_16_1502 *var_16_1502;
  struct M2c_var_17_904 *var_17_904;
  register u8 *var_3_1096 asm("v1");
  struct M2c_var_5_227 *var_5_227;
  s32 new_var13;
  D_0013E030.unk24 = (u16) (D_0013E030.unk24 + 1);
  if (((u32) D_0013E030.unk20) < 9U)
  {
    switch (D_0013E030.unk20)
    {
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
        if (temp_f0_51 < 0.0f)
      {
        D_0015F43C = 0.0f;
      }
        var_16_73 = D_0013E030.unk28;
        temp_18_70 = ((*((s32 *) 0x15ED80)) == 0) ? (0x60) : (0x50);
        if (D_0013CAE4[0] & 0x50)
      {
        if (func_001F96F8(0x1E) < D_0018CB20.unk34)
        {
          if (D_0015F43C == 0.0f)
          {
            if (D_0013E030.unk20 == 0)
            {
              var_16_73 = (D_0018CB20.unk34 < func_001F96F8(D_00160510[D_0013E030.unk26] - 0x1E)) ? (1) : (var_16_73);
            }
            if (D_0013E030.unk20 == 8)
            {
              var_16_73 = (D_0018CB20.unk34 < (D_0018CB20.unk40 - func_001F96F8(0x1E))) ? (1) : (var_16_73);
            }
          }
        }
      }
        if (var_16_73 != 0)
      {
        if (((u32) (D_001516D0.unk5A - 6)) >= 2U)
        {
          D_001516D0.unk5A = 5U;
        }
        if (D_0013E030.unk20 == 0)
        {
          temp_2_132 = func_001F96F8(D_00160510[D_0013E030.unk26]);
          D_0018CB20.unk34 = temp_2_132;
          temp_lo_134 = temp_2_132 / temp_18_70;
          D_0018CB20.unk3C = temp_lo_134;
          func_002049F0(temp_lo_134);
          D_0018CB20.unk38 = (s32) (((s32) D_0018CB20.unk34) % temp_18_70);
          if (D_0013E030.unk28 != 0)
          {
            D_0013E030.unk28 = 0;
          }
          else
          {
            func_001F4A58(4);
          }
          D_0015F43C = 1.0f;
        }
        else
        {
          D_0018CB20.unk34 = (s32) D_0018CB20.unk40;
        }
      }
        if (D_0018CB20.unk34 >= D_0018CB20.unk40)
      {
        if (((u32) (D_001516D0.unk5A - 6)) >= 2U)
        {
          D_001516D0.unk5A = 5U;
        }
        var_16_184 = 0;
        CalculateDmaTransferAddress();
        D_0018CDB0[0] = 0.63f;
        func_001F2D98();
        if (D_0018CB20.unk44 > 0)
        {
          var_17_196 = ((u8 *) (&D_0018CB20)) + 0x178;
          do
          {
            temp_3_198 = *((struct M2c_temp_3_198 **) var_17_196);
            D_0018CB20.unk54 += 0;
            if (temp_3_198 != 0)
            {
              temp_5_201 = temp_3_198->unk24;
              temp_5_201->unkC = (u8) (temp_5_201->unkC - 1);
              temp_3_205 = temp_3_198->unk24;
              new_var11 = temp_3_205->unkC * 2;
              *((s32 *) (((u8 *) (((u8 *) temp_3_205) - (-(2 * new_var11)))) + 0x48)) = 0;
              func_0020C828(temp_3_198, temp_5_201);
            }
            if (D_0013E030.unk8 != 0)
            {
              func_0020C828(D_0013E030.unk8);
            }
            var_16_184 += 1;
            var_17_196 = var_17_196 + 4;
          }
          while (var_16_184 < D_0018CB20.unk44);
        }
        var_4_225 = 0xFF;
        var_5_227 = ((s32 *) 0x15FF1C)[0];
        D_0013E030.unk0->unk34 = (u16) (D_0013E030.unk0->unk34 & 0xFFFE);
        temp_2_231 = var_5_227->unk20;
        if (temp_2_231 != 0xFF)
        {
          var_4_225 = 0xCB;
          var_2_236 = temp_2_231;
          do
          {
            if (var_2_236 == 0)
            {
              temp_2_240 = var_5_227->unkA6;
              if (temp_2_240 != 0x4A)
              {
                temp_2_465 = temp_2_240 == 0xCB;
                if (temp_2_465)
                {
                  goto block_35;
                }
              }
              else
              {
                block_35:
                var_5_227->unk34 = (u16) (var_5_227->unk34 & 0xFF7F);

              }
            }
            var_5_227 = (struct M2c_var_5_227 *) (((u8 *) var_5_227) + 0x100);
            temp_2_251 = var_5_227->unk20;
          }
          while (temp_2_251 != 0xFF);
        }
        if (D_0013E030.unk20 == 0)
        {
          func_00218D78(0, (struct M2c_var_5_227 *) (((u8 *) var_5_227) + 0x100), 0x4A);
          D_001D5BF0[0] = 0xE;
          D_0015F618[0] = 1;
          return;
        }
        D_0015F604 = 0;
        UpdateRenderStateCommand(var_4_225, var_5_227, 0x4A);
        D_0015F618[0] = 1;
        D_001413F5[0] = 0;
        func_001E9438();
        if (D_0013E030.unk2C != 0)
        {
          func_001E9408();
          return;
        }
        func_001E9440(inline_fn2(&D_0013E030) + 0x60, inline_fn2(&D_0013E030) + 0x70, 0, 1);
        return;
      }
        new_var5 = D_0015ED80;
        if ((D_0018CB20.unk38 + 1) > temp_18_70)
      {
        temp_2_299 = D_0018CB20.unk3C + 1;
        D_0018CB20.unk3C = temp_2_299;
        func_002049F0(temp_2_299, new_var5);
      }
        temp_16_308 = D_0018CB20.unk54 + (D_0018CB20.unk38 << 5);
        temp_4_309 = temp_16_308->unkC;
        temp_17_310 = ((u8 *) temp_16_308) + 0x10;
        sp40 = (s32) temp_4_309;
        new_var = temp_16_308;
        D_0018CDB0[0] = temp_17_310->unkC;
        func_001F2D98(temp_4_309);
        *((Vec4 *) D_00187080) = *((Vec4 *) new_var);
        func_001F9CF8(D_00187080, D_00187080, (void *) (((u8 *) D_0013E030.unk0) + 0xC0));
        func_001F9A10(D_00187080, D_00187080, (void *) (((u8 *) D_0013E030.unk0) + 0x10));
        temp_f20_333 = func_001FA580(temp_17_310->unk8, D_0013E030.unk0->unk48);
        func_00125218(sp_slot);
        func_00125360(sp_slot, sp_slot, new_var->unk10);
        func_00125408(sp_slot, sp_slot, temp_17_310->unk4);
        func_001252B8(sp_slot, sp_slot, temp_f20_333);
        temp_2_349 = ((u8 *) D_00187080) - ((u8 *) 0x140);
        if ((!temp_16_1110) && (!temp_16_1110))
      {
      }
        temp_2_349->unk350 = -(*((f32 *) (sp_slot + 0x20)));
        temp_2_349->unk360 = -(*((f32 *) (sp_slot + 0x0)));
        temp_2_349->unk370 = *((f32 *) (sp_slot + 0x10));
        new_var2 = 0;
        temp_2_349->unk354 = -(*((f32 *) (sp_slot + 0x24)));
        temp_2_349->unk364 = -(*((f32 *) (sp_slot + 0x4)));
        temp_2_349->unk374 = *((f32 *) (sp_slot + 0x14));
        temp_2_349->unk358 = -(*((f32 *) (sp_slot + 0x28)));
        temp_2_349->unk368 = -(*((f32 *) (sp_slot + 0x8)));
        temp_2_349->unk378 = *((f32 *) (sp_slot + 0x18));
        if ((*((u8 *) 0x15EDB4)) != 0)
      {
        func_001F9AD8(D_00187080 + 0x220, D_00187080 + 0x230, D_00187080 + 0x210);
      }
        var_22_383 = 0;
        if (D_0018CB20.unk44 > 0)
      {
        sp44 = (f32 *) (sp_slot + 0x10);
        do
        {
          temp_3_400 = D_0018CB20.unk38 >> 1;
          temp_18_399 = *((s32 *) (((u8 *) (((u8 *) (&D_0018CB20)) + (var_22_383 * 4))) + 0x178));
          temp_18_399->unk50 = (u8) temp_3_400;
          temp_18_399->unk51 = (u8) (temp_3_400 + 1);
          func_0020C880(temp_18_399);
          temp_18_399->unk54 = (f32) (func_001FA6C0(D_0018CB20.unk38 & 1) * 0.5f);
          if (sp40 != 0)
          {
            if (D_0018CB20.unk38 & 1)
            {
              temp_18_399->unk54 = 1.0f;
            }
          }
          temp_17_430 = ((u8 *) temp_18_399) + 0x10;
          temp_16_434 = temp_18_399->unk78;
          func_001F9A68(sp_slot, temp_16_434 + (temp_18_399->unk50 * 0x10), 1.0f - temp_18_399->unk54);
          func_001F9A68(sp44, temp_16_434 + (temp_18_399->unk51 * 0x10), temp_18_399->unk54);
          func_001F9A10(temp_17_430, sp_slot, sp44);
          func_001F9CF8(temp_17_430, temp_17_430, (void *) (((u8 *) D_0013E030.unk0) + 0xC0));
          func_001F9A10(temp_17_430, temp_17_430, (void *) (((u8 *) D_0013E030.unk0) + 0x10));
          temp_18_399->unk48 = (f32) D_0013E030.unk0->unk48;
          temp_18_399->unk71 = 0xFF;
          func_0020DEF8(temp_18_399);
          temp_2_465 = temp_18_399->unkA6;
          if (temp_2_465 == 0)
          {
            if (D_0013E030.unk20 == 0)
            {
              if (D_0015ED84 == 0xA)
              {
                if ((*((volatile struct M2c_D_0013D4C0 *) 0x13D4C0)).unk6 == 0)
                {
                  temp_18_399->unk7F = 0U;
                }
                else
                {
                  goto block_57;
                }
              }
              else
              {
                block_57:
                if (func_001F96F8(0x3E) < D_0018CB20.unk34)
                {
                }
                else
                {
                  temp_18_399->unk7F = 0U;
                  goto block_69;
                }

              }
            }
            else
            {
              if (func_001F96F8(0x168) < D_0018CB20.unk34)
              {
                if (D_0015ED84 == 0xA)
                {
                  if (D_0013D4C0.unk6 != new_var2)
                  {
                    goto block_72;
                  }
                  goto block_71;
                }
                goto block_72;
              }
              goto block_71;
            }
          }
          else
            if (temp_2_465 == 0xA)
          {
            if (D_0013E030.unk20 == 0)
            {
              if (func_001F96F8(0x15E) <= (D_0018CB20.unk34 - 1))
              {
                temp_18_399->unk7F = 0U;
                block_69:
                temp_18_399->unk7F = 0x18U;

              }
              else
              {
              }
            }
            else
              if (func_001F96F8(0x20C) >= D_0018CB20.unk34)
            {
              block_71:
              temp_18_399->unk7F = 0U;

            }
            else
            {
              block_72:
              temp_18_399->unk7F = 0x18U;

            }
          }
          if (temp_18_399->unk7F != 0)
          {
            func_00213700(temp_18_399);
          }
          if (temp_18_399->unkA6 == 0xA)
          {
            func_001E9418(temp_18_399);
          }
          if (temp_18_399->unkA6 == 0)
          {
            func_001E9410(temp_18_399);
            if (D_0015ED84 == 0xA)
            {
              if (D_0013D4C0.unk6 == 0)
              {
                goto block_81;
              }
              goto block_83;
            }
            block_81:
            if (D_0015ED84 == 0xD)
            {
              block_83:
              if (D_0013E030.unk8 == 0)
              {
                temp_2_570 = func_0020C4F8(0x509);
                D_0013E030.unk8 = temp_2_570;
                temp_2_570->unk32 = 0x40;
                D_0013E030.unk8->unk34 = (u16) (D_0013E030.unk8->unk34 | 0x806);
                D_0013E030.unk8->unk38 = (s64) temp_18_399->unk38;
                if (D_0013E030.unk8->unk24->unk6 != 0)
                {
                  D_0013E030.unk8->unk73 = 0x18;
                }
              }

              func_001E9450(temp_18_399, D_0013E030.unk8);
            }

          }
          if (((u32) (((u16) temp_18_399->unkA6) - 0x213)) < 3U)
          {
            func_001F47B8(&func_002327A0, temp_18_399);
            func_001F4600(&func_0022DF40, temp_18_399);
            if (D_0013E030.unk20 == 0)
            {
              if (D_0018CB20.unk34 < func_001F96F8(0x168))
              {
                var_4_628 = ((s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f)) + 0x78;
                if (temp_18_399->unkA6 == 0x215)
                {
                  var_2_634 = inline_fn(var_4_628, 8, var_4_628) | (var_4_628 >> 1);
                }
                else
                {
                  goto block_120;
                }
                goto block_123;
              }
              if (D_0018CB20.unk34 < func_001F96F8(0x1F8))
              {
                temp_f0_644 = func_001F96E8(360.0f);
                temp_5_661 = (s32) ((((f32) D_0018CB20.unk34) - temp_f0_644) * ((*((f32 *) 0x15ED60)) * 1.25f));
                if (temp_5_661 >= 256)
                {
                  temp_5_661 = 255;
                }
                if (temp_18_399->unkA6 == 0x215)
                {
                  var_2_664 = temp_5_661 << 8;
                  var_3_662 = temp_5_661 << 0x10;
                }
                else
                {
                  var_3_662 = 0x10;
                  var_3_662 = temp_5_661;
                  var_3_662 = var_3_662 << var_3_662;
                  var_2_664 = temp_5_661 | (temp_5_661 << 8);
                }
                temp_18_399->unk90 = (s32) (var_2_664 | var_3_662);
                temp_3_400 = 0x1A4;
                do
                {
                  if (func_001F96F8(temp_3_400) < D_0018CB20.unk34)
                  {
                    temp_5_661 = (s32) ((((f32) D_0018CB20.unk34) - func_001F96E8(420.0f)) * ((*((f32 *) 0x15ED60)) * 1.2f));
                    D_0013E030.unk0->unkBC = (s8) ((u8) temp_5_661);
                    D_0013E030.unk0->unkB2 = 0;
                    func_001F4600(D_0022E1B0, D_0013E030.unk0);
                  }
                  if (func_001F96F8(0x1D0) < D_0018CB20.unk34)
                  {
                    temp_f0_716 = D_0015F440;
                    temp_f0_716 = temp_f0_716 + 0.025f;
                    D_0015F440 = temp_f0_716;
                    if (temp_f0_716 > 1.0f)
                    {
                      D_0015F440 = 1.0f;
                    }
                  }
                  else
                  {
                    temp_f0_727 = D_0015F440 - 0.025f;
                    temp_18_399->unk90 = 0xA0A0A0;
                    D_0015F440 = temp_f0_727;
                    if (temp_f0_727 < 0.0f)
                    {
                      D_0015F440 = 0.0f;
                    }
                  }
                }
                while (0);
              }
            }
            else
              if (D_0018CB20.unk34 < func_001F96F8(0xF0))
            {
              if (D_0018CB20.unk34 < func_001F96F8(0xC8))
              {
                func_0022F5B0(temp_18_399, D_0015ED6C * (-4.0f));
                func_0022F5B0(temp_18_399, D_0015ED6C * (-3.0f));
              }
              temp_4_775 = ((s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f)) + 0x78;
              if (temp_18_399->unkA6 == 0x215)
              {
                var_2_781 = (temp_4_775 >> 1) | (temp_4_775 << 8);
                var_2_781 = var_2_781 | (temp_4_775 << 0x10);
              }
              else
              {
                var_2_781 = temp_4_775 | (temp_4_775 << 8);
                var_2_781 = var_2_781 | (temp_4_775 << 0x10);
              }
              temp_18_399->unkBC = 0x32;
              temp_18_399->unkB2 = 0xA;
              do
              {
                temp_18_399->unk90 = var_2_781;
              }
              while (0);
              func_001F4600(D_0022E1B0, temp_18_399);
            }
            else
            {
              if (func_001F96F8(0x168) >= D_0018CB20.unk34)
              {
                if (D_0018CB20.unk34 < func_001F96F8(0x12C))
                {
                  ;
                  D_0013E030.unk0->unkBC = (s8) ((u8) ((s32) ((func_001F96E8(300.0f) - ((f32) D_0018CB20.unk34)) * ((*((f32 *) 0x15ED60)) * 1.5f))));
                  D_0013E030.unk0->unkB2 = 0;
                  func_001F4600(D_0022E1B0, D_0013E030.unk0);
                }
                var_4_628 = (s32) ((func_001F96E8(360.0f) - ((f32) D_0018CB20.unk34)) * ((*((f32 *) 0x15ED60)) * 1.25f));
                var_2_853 = var_4_628 << 8;
                if (temp_18_399->unkA6 == 0x215)
                {
                  var_3_855 = var_4_628 << 0x10;
                }
                else
                {
                  goto block_121;
                }
                goto block_122;
              }
              var_4_628 = ((s32) (func_001F9DC8(func_001FA6C0((D_0018CB20.unk34 & 0x3F) - 0x20) * 0.09817477f) * 80.0f)) + 0x78;
              if (temp_18_399->unkA6 == 0x215)
              {
                var_2_634 = ((var_4_628 >> 1) | (var_4_628 << 8)) | (var_4_628 << 0x10);
              }
              else
              {
                block_120:
                var_2_853 = var_4_628 << 8;

                block_121:
                var_2_853 |= var_4_628;

                var_3_855 = var_4_628 << 0x10;
                block_122:
                var_2_634 = var_3_855 | var_2_853;

              }
              block_123:
              temp_18_399->unk90 = var_2_634;

            }
          }
          var_22_383 += 1;
        }
        while (var_22_383 < D_0018CB20.unk44);
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
        if (0 == ((struct M2c_temp_16_893 *) (&D_0013E030))->unk24)
      {
        InitializeRenderState(0);
        func_002168A8(0);
        temp_5_913 = (D_001516D0.unk1C = ((struct M2c_temp_16_893 *) (&D_0013E030))->unk26 + 0x9C4F);
        func_001F4A58(func_001F96F8(0xC, temp_5_913));
        if (((s16) D_001516D0.unk5A) != 3)
        {
          do
          {
            func_0022CA50();
            sceGsSyncV(0);
          }
          while (((s16) D_001516D0.unk5A) != 3);
          temp_18_940 = &D_0013E030;
        }
        func_00215B10();
        *((u8 *) (((u8 *) D_0013E030.unk0) + 0x52)) = 1;
        *((u8 *) (((u8 *) D_0013E030.unk0) + 0x53)) = 2;
        *((f32 *) (((u8 *) D_0013E030.unk0) + 0x54)) = 0.0f;
        func_0020C880(*((s32 *) inline_fn2(&D_0013E030)), 2);
        do
        {
          *((u8 *) (((u8 *) D_0013E030.unk0) + 0x72)) = 0xFF;
        }
        while (0);
        *((s32 *) (((u8 *) D_0013E030.unk0) + 0x94)) = 0;
        if (temp_18_940->unk30 >= 0)
        {
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
        var_17_904 = &D_0013E030;
        func_001E93F0(D_00187080, D_00187080 + 0x10, 1, 0, 0);
        temp_3_997 = temp_18_940->unk30;
        temp_18_940->unk48 = 0.0f;
        temp_18_940->unk4C = 0;
        if (temp_3_997 >= 0)
        {
          var_f21_1004 = *((f32 *) (&temp_18_940->unk48));
          var_18_1006 = 1;
          temp_17_1007 = *((struct M2c_temp_17_1007 **) (((u8 *) D_001CC3B0) + (temp_3_997 * 4)));
          new_var14 = 1;
          temp_17_1007->unk1C = var_f21_1004;
          while (var_18_1006 < (temp_17_1007->unk0 - 1))
          {
            temp_16_1014 = ((u8 *) temp_17_1007) + (var_18_1006 * 0x10);
            temp_3_1015 = ((u8 *) temp_17_1007) + ((var_18_1006 - 1) * 0x10);
            temp_f0_1016 = temp_16_1014->unk10;
            new_var12 = temp_3_1015->unk10;
            temp_f1_1017 = temp_3_1015->unk14;
            var_18_1006 += 1;
            temp_f20_1029 = func_001F9E90(temp_f0_1016 - new_var12, temp_16_1014->unk14 - temp_f1_1017);
            temp_2_1026 = ((u8 *) temp_17_1007) + (var_18_1006 * 0x10);
            new_var7 = temp_2_1026->unk14 - temp_16_1014->unk14;
            temp_f0_1035 = func_001FA5C8(func_001F9E90(temp_2_1026->unk10 - temp_16_1014->unk10, new_var7), temp_f20_1029);
            temp_16_1014->unk1C = temp_f0_1035;
            if (var_f21_1004 < func_001F99C0(temp_f0_1035))
            {
              var_f21_1004 = func_001F99C0(temp_16_1014->unk1C);
            }
          }

          temp_2_1053 = temp_17_1007->unk0;
          var_4_1085 = 0;
          temp_2_1061 = ((u8 *) temp_17_1007) + ((temp_2_1053 - 1) * 0x10);
          temp_3_1060 = ((u8 *) temp_17_1007) + ((temp_2_1053 - 2) * 0x10);
          temp_f20_1070 = func_001F9E90(temp_2_1061->unk10 - temp_3_1060->unk10, temp_2_1061->unk14 - temp_3_1060->unk14);
          temp_2_1075 = ((u8 *) temp_17_1007) + ((temp_17_1007->unk0 - 1) * 0x10);
          new_var3 = temp_2_1075->unk14;
          *((f32 *) ((((u8 *) temp_17_1007) + ((temp_17_1007->unk0 - new_var14) * 0x10)) + 0x1C)) = func_001FA5C8(func_001F9E90(temp_17_1007->unk10 - temp_2_1075->unk10, temp_17_1007->unk14 - new_var3), temp_f20_1070);
          var_3_1096 = temp_17_1007;
          temp_f1_1084 = 0.34906584f / var_f21_1004;
          if (temp_17_1007->unk0 > 0)
          {
            do
            {
              var_4_1085 += 1;
              *((f32 *) (var_3_1096 + 0x1C)) = (*((f32 *) (var_3_1096 + 0x1C))) * temp_f1_1084;
              var_3_1096 += 0x10;
            }
            while (var_4_1085 < temp_17_1007->unk0);
          }
          ;
          temp_19_1055 = ((u8 *) temp_17_1007) + 0x20;
          temp_16_1110 = &D_0013E030;
          temp_16_1110->unk44 = func_001F9B48(((u8 *) temp_17_1007) + 0x10, temp_19_1055);
          temp_16_1110->unk54 = (temp_16_1110->unk50 = (temp_16_1110->unk40 = (temp_16_1110->unk3C = 0)));
          temp_16_1110->unk80 = *((Vec4 *) (((u8 *) D_0013E030.unk0) + 0x10));
          new_var9 = ((u8 *) D_0013E030.unk0) + 0x40;
          temp_16_1110->unkA0 = *((Vec4 *) new_var9);
          temp_16_1110->unk90 = *((Vec4 *) temp_17_1007);
          temp_16_1110->unkB4 = func_001F9E90(func_001F9B80(temp_19_1055, ((u8 *) temp_17_1007) + 0x10, ((u8 *) temp_16_1110) + 0x80), temp_17_1007->unk18 - temp_17_1007->unk28);
          temp_16_1110->unkB8 = func_001F9E90(temp_17_1007->unk20 - temp_17_1007->unk10, temp_17_1007->unk24 - temp_17_1007->unk14);
        }
        var_17_904 = &D_0013E030;
      }
        if (((struct M2c_var_17_904 *) (&D_0013E030))->unk34 >= 0)
      {
        new_var15 = ((u8 *) ((struct M2c_var_17_904 *) (&D_0013E030))) + 0x4C;
        if (((struct M2c_var_17_904 *) (&D_0013E030))->unk38 >= 0)
        {
          temp_f13_1163 = var_f21_1004 * 0.666f;
          var_f21_1004 = ((f32 *) 0x15ED70)[0];
          new_var10 = (struct M2c_var_17_904 *) (&D_0013E030);
          func_00213F38(((u8 *) new_var10) + 0x48, new_var15, 1.0f, temp_f13_1163, temp_f13_1163, D_0015ED6C * 0.5f);
          func_001F9A40(sp_slot, 0x30 + (D_00160034 + (((struct M2c_var_17_904 *) (&D_0013E030))->unk34 << 7)), (D_00160034 + (new_var10->unk38 << 7)) + 0x30, ((struct M2c_var_17_904 *) (&D_0013E030))->unk48);
          func_001F99F8(sp_slot + 0x10);
          temp_f13_1198 = func_001FA5C8(*((f32 *) (((u8 *) ((((struct M2c_var_17_904 *) (&D_0013E030))->unk38 << 7) + D_00160034)) + 0x74)), *((f32 *) (((u8 *) ((new_var10->unk34 << 7) + D_00160034)) + 0x74))) * ((struct M2c_var_17_904 *) (&D_0013E030))->unk48;
          *((f32 *) (sp_slot + 0x14)) = temp_f13_1198;
          *((f32 *) (sp_slot + 0x14)) = func_001FA580(*((f32 *) (((u8 *) ((((struct M2c_var_17_904 *) (&D_0013E030))->unk34 << 7) + D_00160034)) + 0x74)), temp_f13_1198);
          temp_f13_1220 = func_001FA5C8(*((f32 *) (((u8 *) ((new_var10->unk38 << 7) + D_00160034)) + 0x78)), *((f32 *) (((u8 *) ((new_var10->unk34 << 7) + D_00160034)) + 0x78))) * ((struct M2c_var_17_904 *) (&D_0013E030))->unk48;
          *((f32 *) (sp_slot + 0x18)) = temp_f13_1220;
          *((f32 *) (sp_slot + 0x18)) = func_001FA580(*((f32 *) (((u8 *) ((new_var10->unk34 << 7) + D_00160034)) + 0x78)), temp_f13_1220);
          func_001E93F8(sp_slot);
          func_001E9400(sp_slot + 0x10);
        }
      }
        temp_17_1234 = &D_0013E030;
        if (temp_17_1234->unk30 >= 0)
      {
        temp_16_1239 = temp_17_1234->unk24;
        if (temp_16_1239 < func_001F96F8(0x96))
        {
          temp_f20_1249 = func_001FA6C0(temp_17_1234->unk24);
          var_f20_1267 = (1.0f - func_001F9DC8(temp_f20_1249 * (3.1415927f / func_001FA6C0(func_001F96F8(0x78))))) * 0.5f;
          temp_16_1264 = temp_17_1234->unk24;
          if (temp_16_1264 < func_001F96F8(0x78))
          {
            var_f20_1267 = 1.0f;
          }
          *((f32 *) (((u8 *) D_0013E030.unk0) + 0x54)) = var_f20_1267;
          new_var16 = ((u8 *) D_0013E030.unk0) + 0x48;
          func_001F9A40((*((s32 *) inline_fn2(&D_0013E030))) + 0x10, ((u8 *) temp_17_1234) + 0x80, ((u8 *) temp_17_1234) + 0x90, var_f20_1267);
          *((s32 *) (((u8 *) D_0013E030.unk0) + 0x40)) = 0;
          *((f32 *) (((u8 *) D_0013E030.unk0) + 0x44)) = func_001FA580(temp_17_1234->unkA4, func_001FA5C8(temp_17_1234->unkB4, temp_17_1234->unkA4) * var_f20_1267);
          *((f32 *) new_var16) = func_001FA580(temp_17_1234->unkA8, func_001FA5C8(temp_17_1234->unkB8, temp_17_1234->unkA8) * var_f20_1267);
          if ((*((volatile f32 *) 0x15F43C)) > 0.0f)
          {
            temp_f0_1307 = 0.125f;
            temp_f0_1307 = D_0015F43C - temp_f0_1307;
            D_0015F43C = temp_f0_1307;
            if (temp_f0_1307 < 0.0f)
            {
              D_0015F43C = 0.0f;
            }
          }
          if (var_f20_1267 < 1.0f)
          {
            func_0022F5B0(*((s32 *) inline_fn2(&D_0013E030)), D_0015ED6C * (-3.75f));
          }
          var_16_1502 = &D_0013E030;
        }
        else
        {
          temp_f0_1336 = temp_17_1234->unk40 + 0.8f;
          temp_16_1342 = *((s32 **) (((u8 *) D_001CC3B0) + (temp_17_1234->unk30 * 4)));
          temp_17_1234->unk40 = temp_f0_1336;
          if (temp_f0_1336 > 100.0f)
          {
            temp_17_1234->unk40 = 100.0f;
          }
          if (D_0013CAE4[0] & 0x50)
          {
            if (1)
            {
              if (D_0015F43C < 0.0625f)
              {
                D_0015F43C = 0.0625f;
              }
            }
          }
          if (((f32) ((*temp_16_1342) - 6)) < (*((f32 *) (inline_fn2(&D_0013E030) + 0x3C))))
          {
            if (D_0015F43C < 0.0625f)
            {
              D_0015F43C = 0.0625f;
            }
          }
          temp_19_1379 = &D_0013E030;
          if (temp_19_1379->unk26 < 2)
          {
            *((u8 *) (((u8 *) D_0013E030.unk0) + 0xBC)) = 0x32;
            *((s16 *) (((u8 *) D_0013E030.unk0) + 0xB2)) = 0xA;
            func_001F4600(D_0022E1B0, *((s32 *) inline_fn2(&D_0013E030)));
          }
          temp_f12_1401 = temp_19_1379->unk3C + ((temp_19_1379->unk40 * (*((f32 *) (&D_0015ED6C)))) / temp_19_1379->unk44);
          temp_19_1379->unk3C = temp_f12_1401;
          if ((((f32) ((*temp_16_1342) - 1)) < temp_f12_1401) || (D_0015F43C >= 1.0f))
          {
            D_0015F618[0] = 1;
            D_0015F5B0 = 1;
            var_16_1502 = &D_0013E030;
            var_2_236 = temp_2_251 & 0x80;
          }
          else
          {
            func_00214E58(temp_16_1342, 1, (*((s32 *) inline_fn2(&D_0013E030))) + 0x10, (*((s32 *) inline_fn2(&D_0013E030))) + 0x40, 0, temp_f12_1401);
            *((f32 *) (((u8 *) D_0013E030.unk0) + 0x40)) = *((f32 *) (((u8 *) D_0013E030.unk0) + 0x4C));
            temp_4_1437 = temp_19_1379->unk54;
            temp_19_1379->unk50 = (s32) ((temp_19_1379->unk50 + 1) & 0x1F);
            if (temp_4_1437 < 0x20)
            {
              temp_19_1379->unk54 = (s32) (temp_4_1437 + 1);
            }
            temp_17_1453 = ((u8 *) temp_19_1379) + 0xC0;
            temp_18_1458 = ((u8 *) temp_19_1379) + 0x2C0;
            func_001F9CF8((temp_16_434 = temp_19_1379->unk50 * 0x10) + temp_17_1453, (temp_19_1379->unk26 << 5) + D_001D99B0, (*((s32 *) inline_fn2(&D_0013E030))) + 0xC0);
            temp_4_1462 = (temp_19_1379->unk50 * 0x10) + temp_17_1453;
            func_001F9A10(temp_4_1462, temp_4_1462, (*((s32 *) inline_fn2(&D_0013E030))) + 0x10);
            *((f32 *) (((u8 *) (((u8 *) temp_19_1379) + (temp_19_1379->unk50 * 0x10))) + 0xCC)) = 1.0f;
            func_001F9CF8((temp_19_1379->unk50 * 0x10) + temp_18_1458, (temp_19_1379->unk26 << 5) + (D_001D99B0 + 0x10), (*((s32 *) inline_fn2(&D_0013E030))) + 0xC0);
            temp_4_1483 = (temp_19_1379->unk50 * 0x10) + temp_18_1458;
            func_001F9A10(temp_4_1483, temp_4_1483, (*((s32 *) inline_fn2(&D_0013E030))) + 0x10);
            *((f32 *) (((u8 *) (((u8 *) temp_19_1379) + (temp_19_1379->unk50 * 0x10))) + 0x2CC)) = 1.0f;
            func_001F4600(&func_0022E420, *((s32 *) inline_fn2(&D_0013E030)));
            var_16_1502 = &D_0013E030;
            if (D_0015F43C > 0.0f)
            {
              temp_f0_1506 = D_0015F43C + 0.0625f;
              D_0015F43C = temp_f0_1506;
              if (temp_f0_1506 > 1.0f)
              {
                D_0015F43C = 1.0f;
                var_16_1502 = &D_0013E030;
              }
            }
          }
        }
        temp_4_1517 = ((struct M2c_var_16_1502 *) (&D_0013E030))->unk10;
        new_var6 = (u8 *) D_0013E030.unk0;
        new_var6 = new_var6 + 0x40;
        temp_4_1517->unk40 = *((Vec4 *) new_var6);
        temp_7_1526 = ((struct M2c_var_16_1502 *) (&D_0013E030))->unk26 * 0x10;
        new_var8 = ((struct M2c_var_16_1502 *) (&D_0013E030))->unk26;
        func_001F9CF8(((u8 *) temp_4_1517) + 0x10, temp_7_1526 + D_001D9C80, (*((s32 *) inline_fn2((volatile struct M2c_D_0013E030 *) 0x13E030))) + 0xC0);
        temp_4_1532 = (struct M2c_temp_4_1517 *) (((u8 *) ((struct M2c_var_16_1502 *) (&D_0013E030))->unk10) + 0x10);
        func_001F9A10(temp_4_1532, temp_4_1532, (*((s32 *) inline_fn2(&D_0013E030))) + 0x10);
        func_0020DEF8(((struct M2c_var_16_1502 *) (&D_0013E030))->unk10);
        new_var13 = *((s32 *) ((u8 *) (&D_0013E030)));
        temp_4_1538 = ((struct M2c_var_16_1502 *) (&D_0013E030))->unk14;
        temp_4_1538->unk40 = *((Vec4 *) new_var6);
        temp_7_1548 = new_var8 * 0x10;
        func_001F9CF8(((u8 *) temp_4_1538) + 0x10, temp_7_1548 + D_001D9CB0, new_var13 + 0xC0);
        new_var4 = &D_0013E030;
        temp_4_1554 = (struct M2c_temp_4_1538 *) (((u8 *) ((struct M2c_var_16_1502 *) new_var4)->unk14) + 0x10);
        func_001F9A10(temp_4_1554, temp_4_1554, (*((s32 *) ((u8 *) (&D_0013E030)))) + 0x10);
        func_0020DEF8(((struct M2c_var_16_1502 *) new_var4)->unk14);
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

  }
  else
  {
  }
}

__attribute__((alias("FUN_0022f778"))) extern void func_0022F778(void);
#endif /* NON_MATCHING */
