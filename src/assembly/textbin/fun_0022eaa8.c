#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022eaa8/FUN_0022eaa8.s", FUN_0022eaa8);
#else
#include "rnc/fun_0022eaa8_types.h"
#include "types.h"






























extern struct M2c_D_0013DD40 D_0013DD40;
extern u8 D_0013DD43;
extern struct M2c_D_0013E030 D_0013E030;
extern u8 D_0013E5C0[];
extern s32 D_0015ED5C;
extern s32 D_0015ED84;
extern s32 D_0015EE48;
extern s32 D_0015EE4A;
extern f32 D_0015F43C;
extern s32 D_0015F618;
extern s32 D_00160404;
extern u8 D_00160460[];
extern s32 D_001604E0;
extern u8 D_001604F0[];
extern u8 D_00160500[];
extern s32 D_00160F0C;
extern struct M2c_D_0018CB20 D_0018CB20;
extern u8 D_0018CC98[];
extern struct M2c_D_0018CD00 D_0018CD00;
extern struct M2c_D_001940C0 D_001940C0;
extern u8 D_001D9A30[];
extern u8 D_001D9AE0[];
extern u8 D_001D9B30[];
extern u8 D_001D9B48[];
extern s32 FillTransferWords();
extern s32 ReadGlobalTableEntry();
extern s32 func_0012DC80();
extern s32 func_0012E308();
extern s32 func_0012EB00();
extern s32 func_001E9428();
extern s32 func_001E9430();
extern s32 func_001F2D98();
extern s32 func_001F96F8();
extern s32 func_001F98D0();
extern s32 func_001F9A10();
extern void func_001F9A68();
extern s32 func_001FA6C0();
extern s32 func_002049F0();
extern s32 func_0020C828();
extern s32 func_0020C880();
extern s32 func_0020CCA8();
extern s32 func_0020DEF8();
extern s32 func_0022DE10();
extern s32 rand();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_0015EE48, 2");
__asm__(".extern D_0015F43C, 4");
__asm__(".extern D_0015F618, 4");
__asm__(".extern D_00160404, 4");
__asm__(".extern D_001604E0, 4");
__asm__(".extern D_001604F0, 4");
__asm__(".extern D_00160500, 4");

void FUN_0022eaa8(void) {
u8 sp_slot[0x170];    s64 sp10;
    s32 sp20;
    s64 sp50;
    s32 sp60;
    s64 sp90;
    s64 *spA0;
    s32 *spA4;
    s64 *spA8;
    s32 *spAC;
    s32 spB0;
    s64 *spB4;
    s32 *temp_16_279;
    f32 temp_f0_27;
    register f32 temp_f0_360 asm("$22");
    f32 temp_f1_239;
    f32 var_f20_248;
    s16 var_4_295;
    s32 *temp_4_177;
    s32 *var_3_503;
    s32 temp_16_357;
    register s32 temp_17_328 asm("s1");
    s32 temp_18_330;
    s32 temp_19_344;
    s32 temp_20_342;
    s32 temp_2_337;
    s32 temp_3_426;
    s32 temp_4_227;
    s32 temp_4_31;
    s32 temp_6_334;
    s32 var_16_84;
    s32 var_23_322;
    s32 var_2_130;
    s32 var_2_314;
    s32 var_5_296;
    s32 var_5_504;
    s32 var_6_481;
    s32 var_7_438;
    s32 var_8_168;
    void **var_17_86;
    struct M2c_var_5_183 *var_5_183;
    void **var_7_164;
    struct M2c_temp_21_323 *temp_21_323;
    s32 *temp_2_189;
    struct M2c_temp_3_88 *temp_3_88;
    struct M2c_temp_3_95 *temp_3_95;
    struct M2c_temp_5_324 *temp_5_324;
    struct M2c_temp_5_333 *temp_5_333;
    register struct M2c_temp_5_91 * temp_5_91 asm("s0");
    s32 *temp_6_182;
    struct M2c_temp_7_398 *temp_7_398;

    func_001E9430();
    temp_f0_27 = D_0015F43C - 0.25f;
    __asm__ volatile ("" : "+f" (temp_f0_27));
    __asm__ volatile ("" : "+f" (temp_f0_27));
    temp_4_31 = D_0018CB20.unk34 + 1;
    D_0018CB20.unk38 = (s32) (D_0018CB20.unk38 + 1);
    D_0018CB20.unk34 = temp_4_31;
    D_0015F43C = temp_f0_27;
    if (!(temp_f0_27 < 0.0f)) {
        goto block_2;
    }
    D_0015F43C = 0.0f;
block_2:
    if (temp_4_31 != 1) {
        goto block_7;
    }
    if (*(s32 *)0x15ED84 == 0) {
        goto block_8;
    }
    if (*(s32 *)0x15ED84 != temp_4_31) {
        goto block_6;
    }
    if (*(s32 *)0x13DD43 == 0) {
        goto block_8;
    }
block_6:
    ReadGlobalTableEntry(temp_4_31, &D_0018CB20);
    func_0012E308(*(s32 *)0x15ED5C, D_0013E030.unk58, 0x400, 0, 0, 0, 0, D_0013E5C0);
    func_0012EB00();
    func_0012DC80();
block_7:
block_8:
    if (D_0018CB20.unk34 >= D_0018CB20.unk40) {
        goto block_10;
    }
    goto block_34;
block_10:
    var_16_84 = 0;
    if (D_0018CB20.unk44 <= 0) {
        goto block_16;
    }
    var_17_86 = ((u8 *)&D_0018CB20) + 0x178;
loop_13:
    temp_3_88 = *(s32 **)var_17_86;
    if (temp_3_88 == NULL) {
        goto block_15;
    }
    temp_5_91 = temp_3_88->unk24;
    temp_5_91->unkC = (u8) (temp_5_91->unkC - 1);
    temp_3_95 = temp_3_88->unk24;
    *(s32 *)((u8 *)(((u8 *)temp_3_95) + ((u32)temp_3_95->unkC * 4)) + 0x48) = 0;
    func_0020C828(temp_3_88, temp_5_91);
block_15:
    var_16_84 += 1;
    var_17_86 += 4;
    if (var_16_84 < D_0018CB20.unk44) {
        goto loop_13;
    }
block_16:
    if (*(s32 *)0x15ED84 == 0) {
        goto block_21;
    }
    if (*(s32 *)0x15ED84 != 1) {
        goto block_19;
    }
    if (D_0013DD43 == 0) {
        goto block_21;
    }
block_19:
    if (D_0015EE48 >= 3) {
        goto block_21;
    }
    D_0013E030.unk5C = 0;
block_21:
    if (D_0013E030.unk5C >= 2) {
        goto block_31;
    }
    if (D_0013E030.unk5C == 0) {
        goto block_24;
    }
    var_2_130 = 4;
    goto block_26;
block_24:
    var_2_130 = (D_0013E030.unk58 + (((s32) (rand() >> 0x10) % 3) + 1)) & 3;
block_26:
    D_0013E030.unk58 = var_2_130;
    D_0013E030.unk50 = 0;
    D_0013E030.unk54 = 0;
    D_0013E030.unk5C = (s32) (D_0013E030.unk5C + 1);
    *(s32 *)D_001604F0 = D_001604E0;
    FillTransferWords(&D_0018CB20, 0, 0x1C0);
    var_7_164 = D_00160F0C;
    var_8_168 = 0;
    D_0018CB20.unk58 = (void *) (D_001940C0.unk4 + var_7_164);
    D_0018CB20.unk5C = (void *) (D_001940C0.unk8 + var_7_164);
    temp_4_177 = D_001940C0.unk14 + 0x50 + (D_0013E030.unk58 * 4);
    temp_6_182 = D_001940C0.unk14 + D_001940C0.unk14->unk4;
    var_5_183 = temp_6_182 + *temp_4_177;
    if (var_5_183->unk4 == 0) {
        goto block_30;
    }
    var_7_164 = ((u8 *)&D_0018CB20) + 0x60;
loop_28:
    temp_2_189 = *(s32 **)var_5_183;
    var_8_168 += 1;
    var_5_183 += 8;
    *var_7_164 = temp_6_182 + *temp_4_177 + ((u32)temp_2_189 + 0x800);
    var_7_164 += 4;
    if (var_8_168 >= 0x46) {
        goto block_30;
    }
    if (var_5_183->unk4 != 0) {
        goto loop_28;
    }
block_30:
    func_002049F0(0, var_5_183, temp_6_182, var_7_164, var_8_168);
    goto block_36;
block_31:
    if (D_0015EE4A == 0) {
        goto block_33;
    }
    D_0015EE4A = 0;
block_33:
    *(s32 *)0x15F618 = 1;
    return;
block_34:
    if (D_0018CB20.unk38 < 0x60) {
        goto block_36;
    }
    temp_4_227 = D_0018CB20.unk3C + 1;
    D_0018CB20.unk3C = temp_4_227;
    func_002049F0(temp_4_227, &D_0018CB20);
block_36:
    func_0022DE10();
    temp_f1_239 = *((D_0013E030.unk58 * 4) + D_001D9B48);
    if (!(D_0018CD00.unkB0 < temp_f1_239)) {
        goto block_38;
    }
    D_0018CD00.unkB0 = temp_f1_239;
block_38:
    func_001F2D98(&D_0018CD00);
    var_f20_248 = 1.0f;
    if (D_0013E030.unk58 != 4) {
        goto block_40;
    }
    var_f20_248 = (f32) (D_0018CB20.unk40 - D_0018CB20.unk34) / (f32) D_0018CB20.unk40;
    func_001F9A68(sp_slot, D_00160500, &D_0018CB20, var_f20_248);
    func_001F9A10(D_001604F0, D_001604F0, sp_slot);
block_40:
    temp_16_279 = (D_0013E030.unk58 * 0x10) + D_001D9AE0;
    func_001F9A68(D_00160460, temp_16_279, (void *)(u32) ((f32) (D_0018CB20.unk34 - func_001F96F8(0x78)) * 20.0f * var_f20_248));
    var_4_295 = D_0018CB20.unk44;
    var_5_296 = 0;
    D_00160404 = *((D_0013E030.unk58 * 4) + D_001D9B30);
    if (var_4_295 <= 0) {
        goto block_58;
    }
    spA0 = &sp10;
    spA4 = &sp20;
    spAC = &sp60;
    spA8 = &sp50;
    spB4 = &sp90;
    var_2_314 = 0 * 4;
loop_42:
    spB0 = var_5_296 + 1;
    var_23_322 = 0;
    temp_21_323 = *(s32 *)(var_2_314 + D_0018CC98);
loop_43:
    temp_5_324 = temp_21_323->unk24;
    temp_17_328 = (s32) D_0018CB20.unk38 >> 1;
    temp_18_330 = temp_17_328 + 1;
    temp_5_333 = ((u8 *)temp_5_324) + ((temp_5_324->unkC - 1) * 4);
    temp_6_334 = temp_17_328 * 4;
    temp_2_337 = temp_5_333->unk48 + 0x1C;
    temp_20_342 = *(s32 *)(temp_2_337 + (temp_18_330 * 4)) + 0x10;
    temp_19_344 = *(s32 *)(temp_2_337 + temp_6_334) + 0x10;
    temp_16_357 = temp_21_323->unk78;
    temp_f0_360 = (func_001FA6C0(D_0018CB20.unk38 & 1, temp_5_333, temp_6_334) * 0.5f) + ((f32) var_23_322 * 0.25f);
    temp_21_323->unk54 = temp_f0_360;
    func_001F9A68(sp_slot, temp_16_357 + (temp_17_328 * 0x10), (void *)(u32) (1.0f - temp_f0_360));
    func_001F9A68(spA0, temp_16_357 + (temp_18_330 * 0x10), (void *)(u32) temp_21_323->unk54);
    func_001F9A10(((u8 *)temp_21_323) + 0x10, sp_slot, spA0);
    temp_21_323->unk52 = 2;
    temp_21_323->unk53 = 2;
    temp_21_323->unk50 = 0;
    temp_21_323->unk51 = 0;
    func_0020C880(temp_21_323, 2);
    func_001F98D0(temp_21_323->unk68 + 0x10, temp_19_344, 0x20);
    func_001F98D0(temp_21_323->unk6C + 0x10, temp_20_342, 0x20);
    temp_21_323->unk72 = 0xFF;
    temp_21_323->unk71 = 0xFF;
    temp_21_323->unk32 = 0x1FF;
    func_0020DEF8(temp_21_323);
    temp_7_398 = temp_21_323->unk24;
    temp_21_323->unk52 = (s8) (temp_7_398->unkC - 1);
    temp_21_323->unk53 = (s8) (temp_7_398->unkC - 1);
    func_0020CCA8(temp_21_323, 1, spA4, temp_7_398);
    func_0020CCA8(temp_21_323, 2, spAC);
    D_0013E030.unk50 = (s32) ((D_0013E030.unk50 + 1) & 0x1F);
    if (D_0013E030.unk54 >= 0x20) {
        goto block_45;
    }
    D_0013E030.unk54 = (s32) (D_0013E030.unk54 + 1);
block_45:
    temp_3_426 = D_0013E030.unk50 * 0x10;
    *(s32 *)(temp_3_426 + (((u8 *)&D_0013E030) + 0xC0)) = *spA8;
    *(s32 *)(temp_3_426 + (((u8 *)&D_0013E030) + 0x2C0)) = *spB4;
    var_4_295 = (s16) D_0013E030.unk58;
    var_7_438 = var_23_322 + 1;
    if (var_4_295 != 4) {
        goto block_56;
    }
    if (*(s32 *)0x15ED84 == 0) {
        goto block_49;
    }
    if (*(s32 *)0x15ED84 != 1) {
        goto block_50;
    }
    if (D_0013DD40.unk3 != 0) {
        goto block_50;
    }
block_49:
    temp_21_323->unk34 = (u16) (temp_21_323->unk34 | 1);
    D_0013E030.unk54 = 0;
block_50:
    if ((D_0018CB20.unk40 - 0x38) >= D_0018CB20.unk34) {
        goto block_54;
    }
    var_4_295 = (s16) D_001D9A30;
    var_7_438 = var_23_322 + 1;
    var_6_481 = 2;
    temp_21_323->unk2C = (f32) (temp_21_323->unk24->unk24 * ((f32) (D_0018CB20.unk40 - D_0018CB20.unk34) * 0.017857144f));
loop_52:
    var_6_481 -= 1;
    *(s32 *)var_4_295 = (s32) ((*(s32 *)var_4_295 & 0xFFFFFF) | ((D_0018CB20.unk40 - D_0018CB20.unk34) << 0x18));
    var_4_295 += 8;
    if (var_6_481 >= 0) {
        goto loop_52;
    }
    goto block_56;
block_54:
    var_7_438 = var_23_322 + 1;
    var_4_295 = 0xFFFFFF;
    var_3_503 = D_001D9A30;
    var_5_504 = 2;
loop_55:
    var_5_504 -= 1;
    *var_3_503 = (*var_3_503 & 0xFFFFFF) | 0x38000000;
    var_3_503 += 8;
    if (var_5_504 >= 0) {
        goto loop_55;
    }
block_56:
    var_23_322 = var_7_438;
    if (var_23_322 < 2) {
        goto loop_43;
    }
    var_5_296 = spB0;
    var_2_314 = var_5_296 * 4;
    if (var_5_296 < D_0018CB20.unk44) {
        goto loop_42;
    }
block_58:
    func_001E9428((s32 *) var_4_295, var_5_296);
    return;
}

extern void func_0022EAA8(void) __attribute__((alias("FUN_0022eaa8")));
#endif /* NON_MATCHING */
