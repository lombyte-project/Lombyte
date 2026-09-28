#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002196b8/FUN_002196b8.s", FUN_002196b8);
#else
#include "rnc/fun_002196b8_types.h"
#include "types.h"
















extern s32 D_0015EED0;
extern s32 D_0015FF18[];
extern s32 D_001601B0;
extern u8 D_001CE2C0[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern u8 D_001D5D90[];
extern void func_001F2260();
extern void func_001F4280();
extern void func_001F4398();
extern void func_001F5450();
extern void func_001F7888();
extern void func_001F7978();
extern void func_00200E08();
extern void func_00200F90();
extern void func_0020D1F0();
extern void func_0020D218();
extern void func_0020D248();
extern void func_0020D278();
extern void func_0020D330();
extern void func_0020D3B0();
extern void func_00218D10();
extern void func_00223E28();
extern void func_00233980();
extern void func_00237A78();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_001601B0, 4");

void FUN_002196b8(void) {    s32 temp_6_200;
    s64 sp10;
    s64 sp20;
    s64 sp30;
    s64 sp40;
    s64 spA0;
    s64 spB0;
    s32 spC0;
    s32 spC4;
    s32 spC8;
    s32 spCC;
    void **spD0;
    s32 spD4;
    s32 spD8;
    s32 spDC;
    s32 spE0;
    s32 spE4;
s32 *var_17_435;
    s32 *var_18_32;
    s32 temp_16_311;
    s32 temp_17_337;
    s32 temp_18_336;
    s32 temp_19_334;
    s32 temp_20_296;
    s32 temp_21_295;
    s32 temp_22_333;
    s32 temp_23_241;
    s32 temp_30_242;
    s32 temp_4_338;
    s32 temp_5_190;
    s32 temp_5_339;
    s32 var_16_14;
    s32 var_16_170;
    s32 var_16_264;
    s32 var_16_428;
    s32 var_17_238;
    s32 var_17_320;
    s32 var_18_316;
    s32 var_18_65;
    s32 var_19_317;
    s32 var_22_315;
    s32 var_2_173;
    s32 var_2_221;
    u32 temp_2_327;
    u32 temp_3_328;
    void **temp_2_70;
    void **var_17_38;
    void **var_17_73;
    void **var_18_434;
    void **var_19_63;
    struct M2c_temp_16_91 *temp_16_91;
    struct M2c_temp_2_124 *temp_2_124;
    struct M2c_temp_3_229 *temp_3_229;
    struct M2c_temp_4_187 *temp_4_187;
    struct M2c_temp_4_306 *temp_4_306;
    s32 *temp_4_43;
    struct M2c_temp_4_75 *temp_4_75;
    struct M2c_temp_7_182 *temp_7_182;

    var_16_14 = 0;
    func_00233980(0x47, 0x5360B);
    func_0020D278();
    func_0020D1F0();
    func_0020D218();
    var_18_32 = D_001CE2C0;
    func_0020D330(D_0015FF18[0], 4);
    func_00218D10();
    func_001F2260();
    var_17_38 = D_001D5D90;
    do {
        if (*var_18_32 != 0) {
            temp_4_43 = *var_17_38;
            if ((temp_4_43 != NULL) && ((var_16_14 != 6) || (D_001D5BF0.unkD8 != 0))) {
                func_0020D330(temp_4_43, 1);
            }
        }
        var_16_14 += 1;
        var_17_38 += 4;
        var_18_32 += 4;
    } while (var_16_14 < 0xE);
    var_19_63 = D_001D5D90;
    var_18_65 = 0;
    temp_2_70 = (D_001D5BF0.unk4 == 0) ? NULL : (D_001D5BF0.unk4 + 0x44);
    spD0 = temp_2_70;
    var_17_73 = temp_2_70;
    do {
        temp_4_75 = *var_19_63;
        if ((temp_4_75 != NULL) && (((s32 *)D_001CE2C0 + var_18_65) != 0) && ((var_18_65 != 6) || (D_001D5BF0.unkD8 != 0))) {
            temp_16_91 = temp_4_75->unk78;
            sp10 = temp_16_91->unk0;
            sp20 = temp_16_91->unk10;
            sp30 = temp_16_91->unk20;
            sp40 = temp_16_91->unk30;
            spA0 = sp10;
            spB0 = sp40;
            func_00237A78(&spA0, &spB0, &spC0, &spC4, &spC8, &spCC);
            spC8 += 1;
            spCC += 1;
            if (spD0 != NULL) {
                temp_2_124 = *var_17_73;
                if (temp_2_124 != NULL) {
                    temp_2_124->unk20 = spC0;
                    *(s32 *)((u8 *)(*var_17_73) + 0x24) = spC4;
                    *(s32 *)((u8 *)(*var_17_73) + 0x18) = spC8;
                    *(s32 *)((u8 *)(*var_17_73) + 0x1C) = spCC;
                }
            }
            temp_16_91->unk50 = spC8;
            temp_16_91->unk54 = spCC;
            temp_16_91->unk58 = spC0;
            temp_16_91->unk5C = spC4;
            func_00200E08(spC8 + 1, spCC + 1, (spC8 + spC0) - 1, (spCC + spC4) - 1, D_001601B0, 0);
        }
        var_18_65 += 1;
        var_17_73 += 4;
        var_19_63 += 4;
    } while (var_18_65 < 0xE);
    spD4 = 0;
    do {
        var_16_170 = 0;
        spE0 = spD4 + 1;
        var_2_173 = 0 * 4;
loop_20:
        spE4 = var_16_170 + 1;
        temp_7_182 = *(s32 *)(var_2_173 + D_001D5D90);
        if ((temp_7_182 != NULL) && (spD0 != NULL) && (temp_4_187 = *(s32 *)(var_2_173 + spD0), (temp_4_187 != NULL)) && (temp_5_190 = temp_4_187->unk10, ((temp_5_190 & 4) == 0)) && (*(s32 *)(var_2_173 + D_001CE2C0) != 0) && (temp_6_200 = temp_4_187->unk4, (temp_6_200 != NULL)) && ((var_16_170 != 6) || (D_001D5BF0.unkD8 != 0))) {
            if (spD4 == 0) {
                spE4 = var_16_170 + 1;
                if (temp_5_190 & 2) {
                    goto block_31;
                }
                goto block_62;
            }
block_31:
            var_2_221 = temp_5_190 & 1;
            if (spD4 == 1) {
                spE4 = var_16_170 + 1;
                if (!(temp_5_190 & 2)) {
                    var_2_221 = temp_5_190 & 1;
                    goto block_34;
                }
                goto block_62;
            }
block_34:
            if (var_2_221 == 0) {
                temp_3_229 = temp_7_182->unk78;
                spE4 = var_16_170 + 1;
                var_17_238 = 7;
                temp_23_241 = temp_3_229->unk58;
                temp_30_242 = temp_3_229->unk5C;
                spD8 = temp_3_229->unk50;
                spDC = temp_3_229->unk54;
                if (temp_23_241 > 0x80) {
                    do {
                        var_17_238 += 1;
                    } while ((1 << var_17_238) < temp_23_241);
                }
                var_16_264 = 7;
                if (temp_30_242 > 0x80) {
                    var_16_264 = 8;
loop_43:
                    if ((1 << var_16_264) < temp_30_242) {
                        var_16_264 += 1;
                        goto loop_43;
                    }
                }
loop_48:
                if ((var_17_238 + var_16_264) >= 0x12) {
                    var_16_264 -= 1;
                    goto loop_48;
                }
                func_001F7888(var_17_238, var_16_264, spD4 != 0, temp_7_182, 0x3F800000);
                temp_21_295 = 1 << var_16_264;
                temp_20_296 = 1 << var_17_238;
                func_00200F90(0, 0, temp_20_296, temp_21_295, D_001601B0, 0, 0);
                temp_4_306 = *(s32 *)(var_2_173 + spD0);
                temp_16_311 = ((s32 (*)())(temp_4_306->unk4))(temp_4_306);
                func_001F7978();
                if (!(temp_16_311 & 1)) {
                    var_22_315 = 0;
                    var_18_316 = temp_20_296;
                    var_19_317 = 0;
                    var_17_320 = temp_21_295;
                    if (temp_16_311 & 2) {
                        var_18_316 = temp_23_241;
                        var_17_320 = temp_30_242;
                        goto block_61;
                    }
                    temp_2_327 = var_18_316 - temp_23_241;
                    if (temp_16_311 & 8) {
                        temp_3_328 = var_17_320 - temp_30_242;
                        temp_22_333 = (s32) (temp_2_327 + (temp_2_327 >> 0x1F)) >> 1;
                        temp_19_334 = (s32) (temp_3_328 + (temp_3_328 >> 0x1F)) >> 1;
                        temp_18_336 = var_18_316 - temp_22_333;
                        temp_17_337 = var_17_320 - temp_19_334;
                        temp_4_338 = temp_22_333 > -1;
                        temp_5_339 = temp_19_334 > -1;
                        var_18_316 = (temp_20_296 < temp_18_336) ? temp_20_296 : temp_18_336;
                        var_17_320 = (temp_21_295 < temp_17_337) ? temp_21_295 : temp_17_337;
                        var_22_315 = (temp_4_338 == 0) ? 0 : temp_22_333;
                        var_19_317 = (temp_5_339 == 0) ? 0 : temp_19_334;
                        goto block_61;
                    }
                    if (temp_16_311 & 4) {
                        if (temp_23_241 < temp_30_242) {
                            var_22_315 = ((s32) (var_18_316 + ((u32) var_18_316 >> 0x1F)) >> 1) - ((s32) (temp_23_241 * var_18_316) / (s32) (temp_30_242 * 2));
                            var_18_316 -= var_22_315;
                        } else {
                            var_19_317 = ((s32) (var_17_320 + ((u32) var_17_320 >> 0x1F)) >> 1) - ((s32) (temp_30_242 * var_17_320) / (s32) (temp_23_241 * 2));
                            var_17_320 -= var_19_317;
                        }
                        goto block_61;
                    }
                    var_16_170 = spE4;
                    if (temp_16_311 & 0x10) {
block_61:
                        func_00233980(0x42, (0x8000 << 0x18) | 0x64);
                        func_00233980(0x47, 0x43);
                        func_001F5450(spD8, spDC, temp_23_241, temp_30_242, var_22_315, var_19_317, var_18_316 - var_22_315, var_17_320 - var_19_317, (0x8080 << 0x10) | 0x8080, D_0015EED0);
                        goto block_62;
                    }
                } else {
                    goto block_62;
                }
            } else {
                spE4 = var_16_170 + 1;
                ((s32 (*)())temp_6_200)(temp_4_187, temp_5_190, temp_6_200, temp_7_182);
                goto block_62;
            }
        } else {
block_62:
            var_16_170 = spE4;
        }
        var_2_173 = var_16_170 * 4;
        if (var_16_170 < 0xE) {
            goto loop_20;
        }
        if (spD4 == 0) {
            func_0020D248();
            func_0020D3B0();
        }
        spD4 = spE0;
    } while (spE0 < 2);
    var_16_428 = 0;
    func_001F4280(0);
    var_18_434 = D_001D5D90;
    var_17_435 = D_001CE2C0;
    do {
        if ((*var_17_435 != 0) && ((var_16_428 != 6) || (D_001D5BF0.unkD8 != 0))) {
            func_00223E28(*var_18_434);
        }
        var_16_428 += 1;
        var_18_434 += 4;
        var_17_435 += 4;
    } while (var_16_428 < 0xE);
    func_001F4398();
}

extern void func_002196B8(void) __attribute__((alias("FUN_002196b8")));
#endif /* NON_MATCHING */
