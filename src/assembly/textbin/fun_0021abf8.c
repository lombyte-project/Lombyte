#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021abf8/FUN_0021abf8.s", FUN_0021abf8);
#else
#include "rnc/fun_0021abf8_types.h"
#include "types.h"
















extern struct M2c_D_0013C940 D_0013C940;
extern s32 D_0015ED84;
extern s32 D_0015ED88;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern s32 D_001601B4;
extern s32 D_001601E0;
extern u8 D_00199478[];
extern s32 D_001A0314[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 func_001F96F8();
extern s32 func_001FBAB8();
extern s32 func_0022DA68();
s32 FUN_0021abf8(struct M2c_arg0 *arg0) {
    s16 *temp_2_298;
    s16 *temp_4_113;
    s16 *var_4_306;
    s16 temp_16_40;
    s16 temp_3_191;
    s16 temp_4_118;
    s16 var_2_35;
u8 *temp_19_23;
    s32 temp_2_333;
    s32 temp_2_96;
    s32 temp_3_112;
    s32 temp_5_361;
    s32 temp_6_305;
    s32 temp_8_301;
    s32 var_18_24;
    s32 var_20_12;
    s32 var_2_147;
    s32 var_2_332;
    s32 var_3_143;
    s32 var_5_321;
    s32 var_7_300;
    u16 var_3_213;
    struct M2c_temp_2_366 *temp_2_366;
    struct M2c_temp_3_273 *temp_3_273;
    struct M2c_var_3_32 *var_3_32;
    struct M2c_var_7_201 *var_7_201;

    var_20_12 = 0;
    temp_19_23 = D_001D5BF0.unk4->unk40 == (s32) arg0;
    if (*(s32 *)arg0->unk34 != 0) {
        var_18_24 = 0;
        do {
            if (temp_19_23 == 0) {
                goto block_9;
            }
            if (arg0->unk40 != var_20_12) {
block_9:
                temp_16_40 = *(s32 *)((u8 *)(var_18_24 + arg0->unk34) + 0xA);
                if (func_001F96F8(D_001601B4, arg0->unk34) < temp_16_40) {
                    *(s32 *)((u8 *)(var_18_24 + arg0->unk34) + 0xA) = func_001F96F8(D_001601B4);
                }
                var_3_32 = var_18_24 + arg0->unk34;
                if (var_3_32->unkA > 0) {
                    var_2_35 = (u16) var_3_32->unkA - 1;
                } else {
                    var_2_35 = 0;
                }
            } else {
                var_3_32 = var_18_24 + arg0->unk34;
                var_2_35 = var_3_32->unkA + 1;
            }
            var_3_32->unkA = var_2_35;
            var_18_24 += 0xC;
            var_20_12 += 1;
        } while (*(s32 *)(var_18_24 + arg0->unk34) != 0);
    }
    if (temp_19_23 != 0) {
        if (D_0013C940.unk1C4 & 0xD00) {
            if (arg0->unk30 & 0x20) {
                D_001A0314[0] = D_0015ED84;
            }
            goto block_26;
        }
        if (D_0013C940.unk1C4 & 0x10) {
            if (arg0->unk30 & 0x20) {
                D_001A0314[0] = D_0015ED84;
            }
            temp_2_96 = D_001D5BF0.unk4->unk38;
            if (temp_2_96 != 0) {
                D_001D5BF0.unk8 = temp_2_96;
                goto block_28;
            }
            if (D_001D5BF0.unk124 == 0) {
block_26:
                return -1;
            }
            goto block_28;
        }
block_28:
        if (D_0013C940.unk1C4 & 0x40) {
            temp_3_112 = arg0->unk40;
            temp_4_113 = arg0->unk34;
            temp_4_118 = *(s32 *)((u8 *)((temp_3_112 * 0xC) + temp_4_113) + 0x2);
            switch (temp_4_118) {                   /* switch 1 */
            case 4:                                 /* switch 1 */
                func_0022DA68(0, 0x11, arg0->unk14);
                if ((*(s32 *)0x15EEB0 != 1) && (*(s32 *)0x15EEB0 != 0x10)) {
                    var_3_143 = arg0->unk40 * 0xC;
                    var_2_147 = D_0015EEB4 | 2;
block_40:
                    D_0015EEB4 = var_2_147;
                    func_001FBAB8(3, *(s32 *)((u8 *)(var_3_143 + arg0->unk34) + 0x4), arg0->unk34);
                } else {
                case 3:                             /* switch 1 */
                case 1:                             /* switch 2 */
block_38:
                    *(s32 *)((u8 *)(((u8 *)&D_001D5BF0) + 0x5BF0) + 0x8) = (s32) *(s32 *)((u8 *)((arg0->unk40 * 0xC) + arg0->unk34) + 0x4);
                }
                goto block_52;
            case 5:                                 /* switch 1 */
                func_0022DA68(0, 0x11, arg0->unk14);
                switch (*(s32 *)0x15EEB0) {               /* switch 2; irregular */
                case 16:                            /* switch 2 */
                    goto block_38;
                default:                            /* switch 2 */
                    var_3_143 = arg0->unk40 * 0xC;
                    var_2_147 = D_0015EEB4 | 4;
                    goto block_40;
                }
                goto block_52;
            case 6:                                 /* switch 1 */
                temp_3_191 = *(s32 *)((u8 *)((temp_3_112 * 0xC) + temp_4_113) + 0x6);
                if (temp_3_191 != 0) {
                    *(s32 *)((u8 *)(((u8 *)&D_001D5BF0) + 0x5BF0) + 0xEC) = (s32) *(s32 *)((temp_3_191 * 4) + D_00199478);
                }
                var_7_201 = ((u8 *)&D_001D5BF0) + 0x5BF0;
                var_7_201->unkC = 5;
                var_7_201->unkF0 = (s32) var_7_201->unk4;
                var_7_201->unkF4 = 0;
                var_3_213 = *(s32 *)((u8 *)((arg0->unk40 * 0xC) + arg0->unk34) + 0x4);
block_44:
                var_7_201->unkE4 = (s32) var_3_213;
block_45:
                func_0022DA68(0, 0x11, arg0->unk14);
                goto block_80;
            case 7:                                 /* switch 1 */
                var_7_201 = ((u8 *)&D_001D5BF0) + 0x5BF0;
                var_7_201->unkC = 3;
                var_7_201->unkF4 = 2;
                var_7_201->unkF0 = (s32) var_7_201->unk4;
                var_3_213 = (u16) *(s32 *)((u8 *)((arg0->unk40 * 0xC) + arg0->unk34) + 0x4);
                goto block_44;
            case 8:                                 /* switch 1 */
                var_7_201 = ((u8 *)&D_001D5BF0) + 0x5BF0;
                var_7_201->unkC = 4;
                var_7_201->unkF4 = 2;
                var_7_201->unkF0 = (s32) var_7_201->unk4;
                var_3_213 = (u16) *(s32 *)((u8 *)((arg0->unk40 * 0xC) + arg0->unk34) + 0x4);
                goto block_44;
            case 10:                                /* switch 1 */
                var_7_201 = ((u8 *)&D_001D5BF0) + 0x5BF0;
                var_7_201->unkC = 6;
                var_7_201->unkF4 = 2;
                var_7_201->unkF0 = (s32) var_7_201->unk4;
                var_3_213 = (u16) *(s32 *)((u8 *)((arg0->unk40 * 0xC) + arg0->unk34) + 0x4);
                goto block_44;
            case 11:                                /* switch 1 */
                temp_3_273 = ((u8 *)&D_001D5BF0) + 0x5BF0;
                temp_3_273->unkC = 7;
                temp_3_273->unkF0 = (s32) temp_3_273->unk4;
                temp_3_273->unkF4 = 2;
                goto block_45;
            case 9:                                 /* switch 1 */
                D_0015ED88 = *(s32 *)((u8 *)((temp_3_112 * 0xC) + temp_4_113) + 0x4);
                return 0;
            case 2:                                 /* switch 1 */
                func_0022DA68(2, 0x11, arg0->unk14);
                goto block_52;
            }
        } else {
        /* default: */                                    /* switch 1 */
block_52:
            temp_2_298 = arg0->unk34;
            var_7_300 = 0;
            temp_8_301 = arg0->unk40;
            temp_6_305 = arg0->unk30;
            if (*temp_2_298 != 0) {
                var_4_306 = temp_2_298;
                do {
                    var_4_306 += 0xC;
                    var_7_300 += 1;
                } while (*var_4_306 != 0);
            }
            if (temp_6_305 & 1) {
                var_5_321 = D_0013C940.unk1B4;
            } else {
                var_5_321 = D_0013C940.unk1C4;
            }
            if (!(var_5_321 & 0x1000)) {
                if (temp_6_305 & 0x100) {
                    var_2_332 = var_5_321 & 0x4000;
                    if (var_5_321 & 4) {
                        goto block_61;
                    }
                } else {
                    goto block_66;
                }
            } else {
block_61:
                temp_2_333 = arg0->unk40;
                if (temp_2_333 != 0) {
                    arg0->unk40 = (s32) (temp_2_333 - 1);
                } else if (temp_6_305 & 0x1000) {
                    arg0->unk40 = (s32) (var_7_300 - 1);
                } else {
                    D_001D5BF0.unk4->unk80 = (s32) arg0->unk38;
                }
block_66:
                var_2_332 = var_5_321 & 0x4000;
            }
            if ((var_2_332 != 0) || ((arg0->unk30 & 0x100) && (var_5_321 & 8))) {
                temp_5_361 = arg0->unk40;
                temp_2_366 = (temp_5_361 * 0xC) + arg0->unk34;
                if ((temp_2_366->unkC != 0) && (*(s32 *)((u8 *)(((u8 *)temp_2_366) + 0xC) + 0x2) != 0)) {
                    arg0->unk40 = (s32) (temp_5_361 + 1);
                } else if (arg0->unk30 & 0x1000) {
                    arg0->unk40 = 0;
                } else {
                    D_001D5BF0.unk4->unk80 = (s32) arg0->unk3C;
                }
            }
            if ((arg0->unk40 != temp_8_301) || (D_001D5BF0.unk4->unk80 != 0)) {
                func_0022DA68(1, 0x11, arg0->unk14, var_7_300, temp_8_301);
                if (arg0->unk30 & 0x20) {
                    D_001A0314[0] = *(s32 *)((arg0->unk40 * 4) + D_001601E0);
                }
block_80:
            }
            /* Duplicate return node #81. Try simplifying control flow for better match */
            return 0;
        }
    } else {
        return 0;
    }
}

extern s32 func_0021ABF8(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0021abf8")));
#endif /* NON_MATCHING */
