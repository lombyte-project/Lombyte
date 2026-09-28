#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f55d8/FUN_001f55d8.s", FUN_001f55d8);
#else
#include "rnc/fun_001f55d8_types.h"
#include "types.h"








extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_00160840;
extern struct M2c_D_00160F00 *D_00160F00;
extern s32 func_001FA6D0(f32);
void FUN_001f55d8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s64 arg4, s64 arg5, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3) {
    s64 sp0;
    s32 temp_18_45;
    s32 temp_19_40;
    s32 temp_20_65;
    s32 temp_21_98;
    s32 temp_22_106;
    s32 temp_23_67;
    s32 temp_2_50;
    s64 temp_17_51;
    s64 temp_17_90;
    s64 temp_2_96;
    s64 temp_5_85;
    struct M2c_temp_3_84 *temp_3_84;
    struct M2c_temp_7_78 *temp_7_78;

    sp0 = arg4;
    temp_19_40 = (func_001FA6D0(fparg0 * 16.0f) + D_0013E500.unk10) - 8;
    temp_18_45 = (func_001FA6D0((fparg0 + fparg2) * 16.0f) + D_0013E500.unk10) - 8;
    temp_17_51 = (func_001FA6D0(fparg1 * 16.0f) + D_0013E500.unk14) - 8;
    temp_2_50 = func_001FA6D0((fparg1 + fparg3) * 16.0f);
    D_00160F00->unk0 = 0x10000007;
    temp_20_65 = (arg0 + arg2) * 0x10;
    D_00160F00->unk4 = 0;
    temp_23_67 = arg0 * 0x10;
    D_00160F00->unk8 = 0;
    D_00160F00->unkC = 0x50000007;
    temp_7_78 = D_00160F00;
    D_00160F00 = ((u8 *)temp_7_78 + (0x10));
    temp_7_78->unk10 = (s64) D_00160840;
    temp_3_84 = ((u8 *)temp_7_78 + (0x20));
    temp_5_85 = ((temp_2_50 + D_0013E500.unk14) - 8) << 0x10;
    D_00160F00 = temp_3_84;
    temp_7_78->unk20 = arg5;
    temp_17_90 = temp_17_51 << 0x10;
    temp_2_96 = ((0xFFFF << 0x10) | 0xF000) << 0x18;
    temp_3_84->unk8 = 0x154;
    temp_21_98 = (arg1 + arg3) << 0x14;
    temp_22_106 = arg1 << 0x14;
    temp_3_84->unk20 = (s64) (temp_19_40 | temp_17_90 | temp_2_96);
    temp_3_84->unk18 = (s64) (temp_22_106 + temp_23_67);
    temp_3_84->unk28 = (s64) (temp_22_106 + temp_20_65);
    temp_3_84->unk30 = (s64) (temp_18_45 | temp_17_90 | temp_2_96);
    temp_3_84->unk38 = (s64) (temp_21_98 + temp_23_67);
    temp_3_84->unk10 = sp0;
    temp_3_84->unk40 = (s64) (temp_19_40 | temp_5_85 | temp_2_96);
    temp_3_84->unk48 = (s64) (temp_21_98 + temp_20_65);
    temp_3_84->unk50 = (s64) (temp_18_45 | temp_5_85 | temp_2_96);
    temp_3_84->unk58 = 0;
    D_00160F00 += 0x60;
}
#endif /* NON_MATCHING */
