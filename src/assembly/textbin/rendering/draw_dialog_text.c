#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_dialog_text/FUN_001fbc50.s", FUN_001fbc50);
#else
#include "rnc/rendering_draw_dialog_text_types.h"
#include "types.h"











extern u8 D_0013E500[];
extern struct M2c_D_0013F350 D_0013F350;
extern s32 D_0015ED80[];
extern s32 D_0015ED84;
extern u8 D_0015EE58[];
extern u8 D_0015EE68[];
extern s32 D_0015EEB0[];
extern s32 D_0015EEB4[];
extern s32 D_0015F438[];
extern s32 D_0015F4E8;
extern s32 D_0015F4EC;
extern s32 D_0015F4F0;
extern s32 D_0015F4F4;
extern s32 D_0015F4F8;
extern s32 D_0015F4FC;
extern s32 D_0015F500;
extern f32 D_0015F504;
extern s32 D_0015F508;
extern s32 D_0015F50C;
extern s32 D_0015F510;
extern s32 D_0015F514;
extern f32 D_0015F518;
extern s32 D_0015F51C;
extern s32 D_0015F520;
extern s32 D_0015F524;
extern s32 D_0015F528;
extern s32 D_0015F52C;
extern s32 D_0015F530;
extern s32 D_0015F534;
extern s32 D_0015F538;
extern s32 D_0015F53C;
extern s32 D_0015F540;
extern u8 D_0015F548[];
extern u8 D_0015F550[];
extern u8 D_0015F560[];
extern u8 D_0015F568[];
extern u8 D_0015F570[];
extern u8 D_0015F578[];
extern u8 D_0015F580[];
extern u8 D_0015F588[];
extern u8 D_0015F590[];
extern u8 D_0015F5A0[];
extern s32 D_0015F5E8[];
extern struct M2c_D_00193300 D_00193300;
extern u8 D_001DF050[];
extern u8 D_001E78F0[];
extern u8 D_001E7908[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern u8 *func_001FDD10(s32);
extern s32 func_001F96F8(s32);
extern void func_001F5F18(s32, s32, s32, s32, s32);
extern s32 func_001FA6E0(s32, s32, f32);
extern void func_001F6AF0(s32, s32, s32, s32, s32);
extern void func_001F7090(struct M2c_temp_18_388 *, s32, u8 *, s32, s32, u8 *);
extern void func_001F7580(struct M2c_temp_18_388 *, s32, u8 *, s32);
extern void func_001F6060(s32, s32, s32, s32, s32);
extern s32 func_001F44B8(s32);
extern f32 func_001F9DE0(s32, f32);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern s32 func_001FF960();
extern s32 func_001FFA10(s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200600(s32, s32, s32, f32, f32, f32, f32, f32);
extern void func_00233980(s32, s32);
extern s32 memset(void *, s32, s32);
extern s32 sprintf(u8 *, u8 *, ...);
extern s32 strncpy(u8 *, u8 *, s32);

void draw_dialog_text(void) __asm__("FUN_001fbc50");

void draw_dialog_text(void) {
    struct M2c_Frame f;
    f32 temp_f0_48;
    f32 temp_f0_548;
    f32 temp_f1_439;
    f32 temp_f20_448;
    f32 temp_f20_549;
    f32 var_f2_661;
    f32 var_f2_1089;
    f32 var_f2_1140;
    f32 var_f20_719;
    f32 var_f21_688;
    s16 temp_5_432;
    s32 temp_16_75;
    s32 temp_16_172;
    s32 temp_16_535_unk1C;
    s32 temp_16_535_unk4;
    s32 temp_18_481;
    s32 temp_18_764;
    s32 temp_17_770;
    s32 temp_16_776;
    s32 temp_18_821;
    s32 temp_17_827;
    s32 temp_16_833;
    s32 temp_20_680;
    s32 temp_2_430;
    s32 temp_2_709;
    s32 *temp_2_922;
    u8 *var_p500;
    s32 var_4_922;
    s32 var_4_931;
    s32 var_4_1005;
    s32 temp_6_923;
    s32 temp_16_931;
    s32 temp_19_941;
    s32 temp_lo_942;
    s32 temp_18_947;
    s32 temp_16_952;
    s32 temp_3_1005;
    s32 temp_hi_206;
    s32 temp_hi_661;
    s32 temp_hi_1089;
    s32 temp_hi_1140;
    s32 temp_17_131;
    s32 temp_17_187;
    s32 temp_17_562;
    s32 temp_4_566;
    s32 temp_3_567;
    s32 temp_17_636;
    s32 var_20_245;
    s32 var_21_248;
    s32 var_22_243;
    s32 var_23_707;
    s32 var_30_187;
    s32 var_30_711;
    s32 var_4_257;
    s32 var_4_265;
    s32 var_4_313;
    s32 var_4_597;
    s32 var_5_513;
    s32 var_6_514;
    u8 *var_15_fmt;
    u8 *var_19_242;
    u8 *var_17_311;
    u8 *var_19_34;
    u8 *var_7_515;

    func_001F4280(0);
    switch (D_00193300.unk0) {                      /* switch 1 */
    case 5:                                         /* switch 1 */
        temp_f0_48 = (f32) D_00193300.unk4 / (f32) func_001F96F8(0x1E);
        func_001F5F18(0x50, 0x154, 0x60, 0x1A0, (s32) ((1.0f - temp_f0_48) * 80.0f));
        *(struct Blob18 *) &f.box = *(struct Blob18 *) D_001E78F0;
        temp_16_75 = func_001FA6E0(D_0015F4F0, D_0015F4F4, 1.0f - temp_f0_48);
        strncpy((u8 *) 0x70000000, func_001FDD10(0x4E2B), 0x400);
        var_19_34 = (u8 *) 0x70000000;
        while (*var_19_34 >= 2U) {
            var_19_34 += 1;
        }
        while (*var_19_34 == 1) {
            *var_19_34 = 0;
            var_19_34 += 1;
        }
        func_001F7090(&f.box, temp_16_75, (u8 *) 0x70000000, -1, func_001F44B8(1), D_001DF050);
        f.box.unk12 |= 4;
        temp_17_131 = f.box.unkA + f.box.unkE;
        func_001F7090(&f.box, temp_16_75, var_19_34, -1, func_001F44B8(1), D_001DF050);
        f.box.unk12 ^= 4;
        f.box.unkA = 0x136 - (u16) f.box.unkE;
        func_001F7090(&f.box, temp_16_75, var_19_34, -1, func_001F44B8(1), D_001DF050);
        temp_16_172 = func_001FA6E0(0x20FFFF, 0x8020FFFF, 1.0f - ((f32) D_00193300.unk24 / (f32) func_001F96F8(0x1E)));
        func_001F6AF0(0x100, 0x140, temp_16_172, func_001FDD10(0x524A), -1);
        temp_17_187 = (s32) (temp_17_131 + f.box.unkA) >> 1;
        func_00233980(0x47, 0x3004B);
        var_30_187 = temp_17_187 - 0x20;
        func_001FFC30(func_001FF960(0x755D, 0), 0xE0, var_30_187, 0x40, 0x40, 0x80);
        temp_hi_206 = (s32) D_0015F438[0] % 55;
        func_00200600(0x40, 0x40, func_001FFA10(func_001FF960(0x755D, 1, temp_hi_206)), 4096.0f, (f32) (temp_17_187 * 0x10), 272.0f, 272.0f, ((f32) temp_hi_206 * -6.2831855f) / 55.0f);
        break;
    case 3:                                         /* switch 1 */
        var_19_242 = D_0015F548;
        var_22_243 = 0;
        var_20_245 = 0;
        var_21_248 = 0;
        switch (D_0015EEB0[0]) {                       /* switch 2 */
        case 6:                                     /* switch 2 */
            var_4_257 = 0x4FAF;
            var_22_243 = 0x524F;
            goto block_36;
        case 12:                                    /* switch 2 */
            var_4_265 = 0x4FA7;
            if (D_0015EEB4[0] & 2) {
            case 13:                                /* switch 2 */
                var_4_257 = 0x4FAD;
                var_22_243 = 0x524F;
                goto block_36;
            }
            goto block_38;
        case 10:                                    /* switch 2 */
        case 11:                                    /* switch 2 */
            var_4_265 = 0x4FC1;
            goto block_39;
        case 7:                                     /* switch 2 */
        case 8:                                     /* switch 2 */
            var_4_265 = 0x4FB7;
            goto block_39;
        case 14:                                    /* switch 2 */
        case 15:                                    /* switch 2 */
            var_4_265 = 0x4FB8;
            goto block_39;
        case 17:                                    /* switch 2 */
            var_4_265 = 0x4FBA;
            var_20_245 = 0x524A;
            goto block_39;
        case 18:                                    /* switch 2 */
            var_4_265 = 0x4FBC;
            var_20_245 = 0x524A;
            goto block_39;
        case 2:                                     /* switch 2 */
            var_19_242 = func_001FDD10(0x4FA6);
            var_20_245 = (D_00193300.unk4 != 0) ? 0 : 0x524A;
            break;
        case 19:                                    /* switch 2 */
            var_4_265 = 0x4FA7;
            if (D_0015EEB4[0] & 4) {
                goto block_38;
            }
            if (D_0015F5E8[0] == 0) {
                var_4_265 = 0x4FA9;
                goto block_38;
            }
            var_22_243 = 0x524E;
            var_21_248 = 0x524B;
            var_15_fmt = D_0015F550;
            var_17_311 = func_001FDD10(0x4FA9);
            var_4_313 = 0x4FAA;
            goto block_41;
        case 21:                                    /* switch 2 */
            var_4_265 = 0x4FBD;
            var_20_245 = 0x524A;
            goto block_39;
        case 20:                                    /* switch 2 */
            var_4_265 = 0x4FBB;
            var_20_245 = 0x524A;
            goto block_39;
        case 24:                                    /* switch 2 */
            var_22_243 = 0x524E;
            var_21_248 = 0x524B;
            var_15_fmt = D_0015F550;
            var_17_311 = func_001FDD10(0x4FAE);
            var_4_313 = 0x4FAA;
            goto block_41;
        case 23:                                    /* switch 2 */
            var_4_257 = 0x4FB1;
block_35:
            var_22_243 = 0x524E;
            goto block_36;
        case 3:                                     /* switch 2 */
        case 4:                                     /* switch 2 */
            if ((D_0015F5E8[0] != 0) && (D_0015EEB4[0] & 2)) {
                var_4_257 = 0x4FAB;
                var_22_243 = 0x524E;
block_36:
                var_21_248 = 0x524B;
                var_19_242 = func_001FDD10(var_4_257);
                break;
            }
            var_4_265 = 0x4FAC;
block_38:
            var_20_245 = 0x4FA8;
block_39:
            var_19_242 = func_001FDD10(var_4_265);
            break;
        case 5:                                     /* switch 2 */
            var_20_245 = 0x4FA8;
            var_15_fmt = D_0015F550;
            var_17_311 = func_001FDD10(0x4FB0);
            var_4_313 = 0x4FA7;
block_41:
            sprintf(f.buf, var_15_fmt, var_17_311, 1, 1, func_001FDD10(var_4_313));
            var_19_242 = f.buf;
            break;
        }
        memset(&f.box2, 0, 0x18);
        var_p500 = D_0013E500;
        f.box2.unk2 = *(u16 *) (var_p500 + 0x4);
        f.box2.unk4 = 0x60;
        f.box2.unk6 = 0x1A0;
        f.box2.unk8 = 0x100;
        f.box2.unkA = 0x68;
        f.box2.unk10 = 0x10;
        f.box2.unk12 = 5;
        *(struct Blob18 *) &f.box = *(struct Blob18 *) &f.box2;
        func_001F7580(&f.box, 0, var_19_242, -1);
        temp_5_432 = (f.box.unkE + 0x28);
        temp_2_430 = (s32) (*(s32 *) (var_p500 + 0x4) - temp_5_432) >> 1;
        f.box.unkA = temp_2_430 + 4;
        f.box.unk2 = temp_2_430 + temp_5_432;
        f.box.unk0 = (s16) temp_2_430;
        temp_f1_439 = (f32) func_001F96F8(0x1E);
        temp_f20_448 = 1.0f - ((f32) D_00193300.unk4 / temp_f1_439);
        func_001F5F18(f.box.unk0, f.box.unk2, 0x60, 0x1A0, (s32) (temp_f20_448 * 80.0f));
        f.box.unk12 ^= 4;
        func_001F7580(&f.box, func_001FA6E0(D_0015F4F0, D_0015F4F4, temp_f20_448), var_19_242, -1);
        temp_18_481 = func_001FA6E0(0x20FFFF, 0x8020FFFF, 1.0f - ((f32) D_00193300.unk24 / (f32) func_001F96F8(0x1E)));
        if (var_22_243 != 0) {
            func_001F6AF0(0xCA, f.box.unk2 - 0x14, temp_18_481, func_001FDD10(var_22_243), -1);
        }
        if (var_21_248 != 0) {
            func_001F6AF0(0x135, f.box.unk2 - 0x14, temp_18_481, func_001FDD10(var_21_248), -1);
        }
        if (var_20_245 != 0) {
            var_5_513 = f.box.unk2 - 0x14;
            var_6_514 = temp_18_481;
            var_7_515 = func_001FDD10(var_20_245);
            goto block_91;
        }
        break;
    case 6:                                         /* switch 1 */
        *(struct Blob18 *) &f.box = *(struct Blob18 *) D_001E7908;
        temp_f0_548 = (f32) D_00193300.unk4 / (f32) func_001F96F8(0x1E);
        temp_f20_549 = 1.0f - temp_f0_548;
        func_001F5F18(0x64, 0x12C, 0x60, 0x1A0, (s32) (temp_f20_549 * 80.0f));
        temp_17_562 = func_001FA6E0(D_0015F4F0, D_0015F4F4, temp_f20_549);
        temp_4_566 = func_001FA6E0(0x20FFFF, 0x8020FFFF, temp_f20_549);
        temp_3_567 = D_00193300.unk1C;
        if (temp_3_567 != 2) {
            if (temp_3_567 >= 3) {
                if (temp_3_567 != 3) {
                    goto block_56;
                }
                func_001F6AF0(0x100, 0x118, temp_4_566, func_001FDD10(0x524A), -1);
                var_4_597 = 0x522C;
            } else if (temp_3_567 >= 0) {
                func_001F6AF0(0xCA, 0x118, temp_4_566, func_001FDD10(0x524E), -1);
                func_001F6AF0(0x135, 0x118, temp_4_566, func_001FDD10(0x524B), -1);
                var_4_597 = 0x522A;
            } else {
block_56:
                var_4_597 = 0;
            }
        } else {
            func_001F6AF0(0xCA, 0x118, temp_4_566, func_001FDD10(0x524E), -1);
            func_001F6AF0(0x135, 0x118, temp_4_566, func_001FDD10(0x524B), -1);
            var_4_597 = 0x522B;
        }
        if (var_4_597 != 0) {
            temp_17_636 = func_001FDD10(var_4_597);
            func_001F7090(&f.box, temp_17_562, temp_17_636, -1, func_001F44B8(1), D_001DF050);
        }
        break;
    case 0:                                         /* switch 1 */
        var_f2_661 = func_001FA6C0(D_0015F520);
        temp_hi_661 = (s32) D_0015F438[0] % (s32) D_0015F520;
        temp_20_680 = func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0(temp_hi_661, (((f32) temp_hi_661 / var_f2_661) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f);
        var_f21_688 = (f32) D_00193300.unk20 * 0.125f;
        if (var_f21_688 > 1.0f) {
            var_f21_688 = 1.0f;
        } else if (var_f21_688 < 0.1f) {
            var_f21_688 = 0.1f;
        }
        f.sp280 = D_0015F52C;
        var_23_707 = D_0015F534;
        temp_2_709 = D_00193300.unk24;
        var_30_711 = D_0015F53C;
        if (temp_2_709 != 0) {
            var_f20_719 = (f32) temp_2_709 * 0.125f;
            if (var_f20_719 > 1.0f) {
                var_f20_719 = 1.0f;
            } else if (var_f20_719 < 0.0f) {
                var_f20_719 = 0.0f;
            }
            f.sp280 = func_001FA6E0(D_0015F52C, D_0015F530, var_f20_719);
            var_23_707 = func_001FA6E0(D_0015F534, D_0015F538, var_f20_719);
            var_30_711 = func_001FA6E0(D_0015F53C, D_0015F540, var_f20_719);
        }
        if (D_0013F350.unk89A < 3) {
            temp_18_764 = D_0015F4FC - func_001FA6D0((f32) D_0015F504 * var_f21_688);
            temp_17_770 = D_0015F4FC + func_001FA6D0((f32) D_0015F4FC * var_f21_688);
            temp_16_776 = D_0015F4F8 - func_001FA6D0((f32) D_0015F500 * var_f21_688);
            func_001F6060(temp_18_764, temp_17_770, temp_16_776, D_0015F4F8 + func_001FA6D0((f32) D_0015F500 * var_f21_688), temp_20_680);
            func_001F6AF0(0x100, D_0015F508, f.sp280, func_001FDD10(0x4F6E), -1);
            func_001F6AF0(0x100, D_0015F508 + 0x18, f.sp280, func_001FDD10(0x5249), -1);
            var_5_513 = D_0015F508 + 0x30;
            var_6_514 = f.sp280;
            var_7_515 = func_001FDD10(0x5248);
        } else {
            temp_18_821 = D_0015F510 - func_001FA6D0((f32) D_0015F518 * var_f21_688);
            temp_17_827 = D_0015F510 + func_001FA6D0((f32) D_0015F518 * var_f21_688);
            temp_16_833 = D_0015F50C - func_001FA6D0((f32) D_0015F514 * var_f21_688);
            func_001F6060(temp_18_821, temp_17_827, temp_16_833, D_0015F50C + func_001FA6D0((f32) D_0015F514 * var_f21_688), temp_20_680);
            sprintf(f.buf2, D_0015F560, func_001FDD10(0x5240));
            if (D_0013F350.unk8C0 == 1) {
                sprintf(f.buf2, D_0015F568, 1);
            } else if (D_0013F350.unk8C0 == 2) {
                sprintf(f.buf2, D_0015F570, 2);
            } else if (D_0013F350.unk8C0 == 3) {
                sprintf(f.buf2, D_0015F578, 3);
            } else {
                sprintf(f.buf2, D_0015F580, D_0013F350.unk8C0);
            }
            f.buf2[3] = 0x20;
            if (D_0013F350.unk8C0 == 1) {
                func_001F6AF0(0x100, D_0015F51C, var_23_707, f.buf2, -1);
            } else {
                func_001F6AF0(0x100, D_0015F51C, var_30_711, f.buf2, -1);
            }
            if ((D_0015ED84 ^ 0x10) != 0) {
                var_4_922 = 0;
            } else {
                var_4_922 = 4;
            }
            temp_2_922 = var_4_922 + D_0015EE58;
            temp_6_923 = *temp_2_922;
            if (temp_6_923 == D_0013F350.unk894) {
                if (D_0015ED80[0] != 0) {
                    var_4_931 = 0xBB8;
                } else {
                    var_4_931 = 0xE10;
                }
                temp_16_931 = var_4_931;
                temp_19_941 = temp_6_923 / temp_16_931;
                temp_lo_942 = temp_16_931 / 60;
                temp_18_947 = (s32) (*temp_2_922 % temp_16_931) / temp_lo_942;
                temp_16_952 = (s32) (((*temp_2_922 % temp_16_931) % temp_lo_942) * 0x64) / temp_lo_942;
                sprintf(f.buf2, D_0015F588, func_001FDD10(0x50A3));
                func_001F6AF0(0x100, D_0015F51C + 0x26, var_23_707, f.buf2, -1);
                sprintf(f.buf2, D_0015F590, temp_19_941, temp_18_947, temp_16_952);
                func_001F6AF0(0x100, D_0015F51C + 0x3A, var_23_707, f.buf2, -1);
            } else {
                sprintf(f.buf2, D_0015F588, func_001FDD10(0x50A2));
                func_001F6AF0(0x100, D_0015F51C + 0x30, var_30_711, f.buf2, -1);
            }
            if ((D_0015ED84 ^ 0x10) != 0) {
                var_4_1005 = 0;
            } else {
                var_4_1005 = 4;
            }
            temp_3_1005 = *(s32 *) (var_4_1005 + D_0015EE68);
            if (temp_3_1005 != 0) {
                if (temp_3_1005 == D_0013F350.unk8A8) {
                    sprintf(f.buf2, D_0015F588, func_001FDD10(0x50A5));
                    func_001F6AF0(0x100, D_0015F51C + 0x56, var_23_707, f.buf2, -1);
                    sprintf(f.buf2, D_0015F5A0, D_0013F350.unk8A8);
                    func_001F6AF0(0x100, D_0015F51C + 0x6A, var_23_707, f.buf2, -1);
                } else {
                    goto block_88;
                }
            } else {
block_88:
                sprintf(f.buf2, D_0015F588, func_001FDD10(0x50A4));
                func_001F6AF0(0x100, D_0015F51C + 0x60, var_30_711, f.buf2, -1);
            }
            func_001F6AF0(0x100, D_0015F51C + 0x90, f.sp280, func_001FDD10(0x5249), -1);
            var_5_513 = D_0015F51C + 0xA8;
            var_6_514 = f.sp280;
            var_7_515 = func_001FDD10(0x5248);
        }
        goto block_91;
    case 2:                                         /* switch 1 */
        var_f2_1089 = func_001FA6C0(D_0015F520);
        temp_hi_1089 = (s32) D_0015F438[0] % (s32) D_0015F520;
        func_001F6060(0x64, 0xA0, 0xB0, 0x150, func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0(temp_hi_1089, (((f32) temp_hi_1089 / var_f2_1089) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f));
        var_7_515 = D_00193300.unk8;
        var_5_513 = 0x7A;
        var_6_514 = (0x8000 << 0x10) | 0xC0C0;
block_91:
        func_001F6AF0(0x100, var_5_513, var_6_514, var_7_515, -1);
        break;
    default:                                        /* switch 1 */
        var_f2_1140 = func_001FA6C0(D_0015F520);
        temp_hi_1140 = (s32) D_0015F438[0] % (s32) D_0015F520;
        func_001F6060(0x50, D_0015F4EC + 0x1A, 0xB0, 0x150, func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0(temp_hi_1140, (((f32) temp_hi_1140 / var_f2_1140) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f));
        if (D_00193300.unk8 != NULL) {
            func_001F6AF0(0x100, 0x5A, (0x8000 << 0x10) | 0xC0C0, D_00193300.unk8, -1);
        }
        if (D_00193300.unkC != NULL) {
            func_001F6AF0(0x100, D_0015F4E8, (0x80FF << 0x10) | 0xA888, D_00193300.unkC, -1);
        }
        if (D_00193300.unk10 != NULL) {
            func_001F6AF0(0x100, D_0015F4EC, (0x80FF << 0x10) | 0xA888, D_00193300.unk10, -1);
        }
        break;
    }
    func_001F4398();
}

extern void func_001FBC50(void) __attribute__((alias("FUN_001fbc50")));
#endif /* NON_MATCHING */
