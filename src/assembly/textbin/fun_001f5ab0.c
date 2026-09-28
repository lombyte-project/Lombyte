#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f5ab0/FUN_001f5ab0.s", FUN_001f5ab0);
#else
#include "rnc/fun_001f5ab0_types.h"
#include "types.h"








extern struct M2c_D_0013E500 D_0013E500;
extern struct M2c_D_00160F00 *D_00160F00;
extern void func_001F9A10();
extern void func_001F9A28();
extern void func_001F9A68();
extern s32 func_001F9DC8();
extern s32 func_001F9DE0();
extern s32 func_001FA6D0();
extern void sp10();
extern void sp20();
extern void sp40();
extern void sp50();
extern void sp60();
extern void sp70();
void FUN_001f5ab0(s32 arg0, s32 arg1, s64 arg2, s64 arg3, s32 arg4, s32 arg5, s32 arg6, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3, f32 fparg4, f32 fparg5, f32 fparg6) {    s32 sp44;
u8 sp_slot[0x170];    s32 sp54;
    s32 sp64;
    s32 sp74;
    f32 sp10;
    f32 sp14;
    f32 sp20;
    f32 sp24;
    s32 sp30;
    f32 sp40;
    f32 sp50;
    f32 sp60;
    f32 sp70;
    s64 sp80;
    s32 sp88;
    f32 temp_f20_60;
    f32 temp_f22_57;
    s32 temp_10_6;
    s32 temp_16_188;
    s32 temp_16_209;
    s32 temp_16_228;
    s32 temp_16_247;
    s32 temp_2_196;
    s32 temp_2_215;
    s32 temp_2_234;
    s32 temp_2_252;
    s32 temp_9_4;
    s32 var_21_41;
    s32 var_22_33;
    s32 var_23_43;
    s32 var_30_35;
    s64 temp_20_49;
    s64 temp_4_213;
    s64 temp_4_232;
    s64 temp_5_154;
    s64 temp_5_192;
    s64 temp_6_159;
    s64 temp_8_163;
    struct M2c_temp_18_173 *temp_18_173;
    struct M2c_temp_2_172 *temp_2_172;

    temp_9_4 = arg5 & 0xFF;
    temp_10_6 = arg6 & 0xFF;
    sp80 = arg2;
    sp88 = arg4;
    if (temp_9_4 != 0) {
        var_22_33 = arg0 * 0x10;
        var_30_35 = 0x10;
    } else {
        var_30_35 = arg0 * 0x10;
        var_22_33 = 0x10;
    }
    var_21_41 = arg1 << 0x14;
    if (temp_10_6 != 0) {
        var_23_43 = 0x100000;
    } else {
        var_23_43 = arg1 << 0x14;
        var_21_41 = 0x100000;
    }
    sp20 = fparg0;
    temp_20_49 = arg3 << 0x20;
    sp24 = fparg1;
    temp_f22_57 = 1.0f - fparg6;
    temp_f20_60 = 1.0f - fparg5;
    sp10 = fparg2 * func_001F9DC8(fparg4, fparg3 * func_001F9DC8(fparg4, fparg3 * func_001F9DE0(temp_9_4, temp_10_6, fparg4)));
    sp14 = -fparg2 * func_001F9DE0((s32)(u32) fparg4);
    func_001F9A68(&sp30, sp_slot, temp_f22_57);
    func_001F9A10(&sp40, &sp20, &sp30);
    func_001F9A68(&sp30, &sp10, temp_f20_60);
    func_001F9A28(&sp40, &sp40, &sp30);
    func_001F9A68(&sp30, sp_slot, temp_f22_57);
    func_001F9A10(&sp50, &sp20, &sp30);
    func_001F9A68(&sp30, &sp10, fparg5);
    func_001F9A10(&sp50, &sp50, &sp30);
    func_001F9A68(&sp30, sp_slot, fparg6);
    func_001F9A28(&sp60, &sp20, &sp30);
    func_001F9A68(&sp30, &sp10, temp_f20_60);
    func_001F9A28(&sp60, &sp60, &sp30);
    func_001F9A68(&sp30, sp_slot, fparg6);
    func_001F9A28(&sp70, &sp20, &sp30);
    func_001F9A68(&sp30, &sp10, fparg5);
    func_001F9A10(&sp70, &sp70, &sp30);
    D_00160F00->unk0 = 0x10000007;
    temp_5_154 = (0xB400 << 0x30) | 0x8001;
    temp_6_159 = (((0xA6A6 << 0x10) | 0xA6A6) << 0xB) | 0x106;
    temp_8_163 = var_22_33 | var_21_41;
    do {
        D_00160F00->unk4 = 0;
        D_00160F00->unk8 = 0;
    } while (0);
    D_00160F00->unkC = 0x50000007;
    temp_2_172 = D_00160F00;
    temp_18_173 = ((u8 *)temp_2_172 + (0x10));
    D_00160F00 = temp_18_173;
    temp_2_172->unk10 = temp_5_154;
    temp_18_173->unk8 = temp_6_159;
    temp_18_173->unk18 = 0x154;
    temp_18_173->unk10 = sp80;
    temp_18_173->unk28 = temp_8_163;
    temp_18_173->unk20 = (s64) sp88;
    temp_16_188 = func_001FA6D0(8.589942e9f, temp_5_154, temp_6_159, 0x154, temp_8_163, sp40 * 16.0f);
    temp_5_192 = var_30_35 | var_21_41;
    temp_2_196 = func_001FA6D0(sp44 * 16.0f) + D_0013E500.unk14;
    temp_18_173->unk38 = temp_5_192;
    temp_18_173->unk30 = (s64) (((temp_16_188 + D_0013E500.unk10) - 8) | ((temp_2_196 - 8) << 0x10) | temp_20_49);
    temp_16_209 = func_001FA6D0((f32) D_0013E500.unk10, temp_5_192, (s64)(u64) (sp50 * 16.0f));
    temp_4_213 = var_22_33 | var_23_43;
    temp_2_215 = func_001FA6D0(sp54 * 16.0f) + D_0013E500.unk14;
    temp_18_173->unk48 = temp_4_213;
    temp_18_173->unk40 = (s64) (((temp_16_209 + D_0013E500.unk10) - 8) | ((temp_2_215 - 8) << 0x10) | temp_20_49);
    temp_16_228 = func_001FA6D0((f32) temp_4_213, (s64) D_0013E500.unk10, (s64)(u64) (sp60 * 16.0f));
    temp_4_232 = var_30_35 | var_23_43;
    temp_2_234 = func_001FA6D0(sp64 * 16.0f) + D_0013E500.unk14;
    temp_18_173->unk58 = temp_4_232;
    temp_18_173->unk50 = (s64) (((temp_16_228 + D_0013E500.unk10) - 8) | ((temp_2_234 - 8) << 0x10) | temp_20_49);
    temp_16_247 = func_001FA6D0((f32) temp_4_232, (s64) D_0013E500.unk10, (s64)(u64) (sp70 * 16.0f));
    temp_2_252 = func_001FA6D0(sp74 * 16.0f) + D_0013E500.unk14;
    temp_18_173->unk68 = 0;
    temp_18_173->unk60 = (s64) (((temp_16_247 + D_0013E500.unk10) - 8) | ((temp_2_252 - 8) << 0x10) | temp_20_49);
    D_00160F00 += 0x70;
}
#endif /* NON_MATCHING */
