#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f5808/FUN_001f5808.s", FUN_001f5808);
#else
#include "rnc/fun_001f5808_types.h"
#include "types.h"








extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_00160860;
extern struct M2c_D_00160F00 *D_00160F00[];
extern s32 func_001FA6D0(f32);
void FUN_001f5808(s64 arg0, s64 arg1, s32 arg2, s32 arg3, s64 arg4, s64 arg5, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3) {
    s64 sp0;
    s32 temp_10_130;
    s32 temp_11_104;
    s32 temp_15_80;
    s32 temp_16_79;
    s32 temp_20_45;
    s32 temp_21_40;
    s64 temp_14_78;
    s64 temp_19_51;
    s64 temp_2_127;
    s64 temp_4_74;
    s64 temp_6_107;
    s64 temp_7_117;
    s64 temp_8_57;
    struct M2c_temp_12_90 *temp_12_90;
    struct M2c_temp_3_103 *temp_3_103;

    sp0 = arg4;
    temp_21_40 = (func_001FA6D0(fparg0 * 16.0f) + D_0013E500.unk10) - 8;
    temp_20_45 = (func_001FA6D0((fparg0 + fparg2) * 16.0f) + D_0013E500.unk10) - 8;
    temp_19_51 = (func_001FA6D0(fparg1 * 16.0f) + D_0013E500.unk14) - 8;
    temp_8_57 = (func_001FA6D0((fparg1 + fparg3) * 16.0f) + D_0013E500.unk14) - 8;
    if (temp_21_40 <= 0x9000) {
        if ((temp_20_45 >= 0x7000) && (temp_19_51 <= 0x9000) && (temp_8_57 >= 0x7000)) {
            D_00160F00[0]->unk0 = 0x10000008;
            temp_4_74 = arg0 + arg2;
            temp_14_78 = arg1 + arg3;
            temp_16_79 = arg0 * 0x10;
            temp_15_80 = temp_4_74 * 0x10;
            do {
                D_00160F00[0]->unk4 = 0;
                D_00160F00[0]->unk8 = 0;
            } while (0);
            D_00160F00[0]->unkC = 0x50000008;
            temp_12_90 = D_00160F00[0];
            D_00160F00[0] = ((u8 *)temp_12_90 + (0x10));
            temp_12_90->unk10 = (s64) D_00160860;
            temp_3_103 = ((u8 *)temp_12_90 + (0x20));
            temp_11_104 = temp_14_78 << 0x14;
            temp_6_107 = temp_8_57 << 0x10;
            D_00160F00[0] = temp_3_103;
            temp_12_90->unk20 = arg5;
            temp_7_117 = temp_19_51 << 0x10;
            temp_3_103->unk10 = (s64) ((arg0 * 0x10) | ((temp_4_74 << 0xE) | 0xA) | (arg1 << 0x18) | (temp_14_78 << 0x22));
            temp_2_127 = ((0xFFFF << 0x10) | 0xF000) << 0x18;
            temp_3_103->unk8 = 0x154;
            temp_10_130 = arg1 << 0x14;
            temp_3_103->unk18 = sp0;
            temp_3_103->unk20 = (s64) (temp_10_130 + temp_16_79);
            temp_3_103->unk28 = (s64) (temp_21_40 | temp_7_117 | temp_2_127);
            temp_3_103->unk30 = (s64) (temp_10_130 + temp_15_80);
            temp_3_103->unk38 = (s64) (temp_20_45 | temp_7_117 | temp_2_127);
            temp_3_103->unk40 = (s64) (temp_11_104 + temp_16_79);
            temp_3_103->unk48 = (s64) (temp_21_40 | temp_6_107 | temp_2_127);
            temp_3_103->unk50 = (s64) (temp_11_104 + temp_15_80);
            temp_3_103->unk58 = (s64) (temp_20_45 | temp_6_107 | temp_2_127);
            temp_3_103->unk60 = 5;
            temp_3_103->unk68 = 0;
            D_00160F00[0] += 0x70;
        }
    }
}
#endif /* NON_MATCHING */
