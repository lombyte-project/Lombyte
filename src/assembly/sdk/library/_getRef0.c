#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _getRef0; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/_getRef0/_getRef0.s", _getRef0);
#else
#include "rnc/sdk_library__getRef0_types.h"
#include "types.h"










extern u8 D_00132E30[];
extern u8 D_00132E50[];
void _getRef0(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18) {    s32 arg_sp0;
    s32 arg_sp10;
    s32 arg_sp18;
    s32 arg_sp8;
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    s32 sp10;
u8 *sp14;
    s32 sp18;
    s32 *temp_8_242;
    s32 temp_10_32;
    s32 temp_18_126;
    s32 temp_20_56;
    s32 temp_21_60;
    s32 temp_23_36;
    s32 temp_25_171;
    s32 temp_25_72;
    s32 temp_2_138;
    s32 temp_2_184;
    s32 temp_2_198;
    s32 temp_2_29;
    s32 temp_2_53;
    s32 temp_2_84;
    s32 temp_2_98;
    s32 temp_3_123;
    s32 temp_3_127;
    s32 temp_4_230;
    s32 temp_4_39;
    s32 temp_5_124;
    s32 temp_6_170;
    s32 temp_6_235;
    s32 temp_6_70;
    s32 temp_7_134;
    s32 temp_7_229;
    s32 temp_8_220;
    s32 temp_9_113;
    s32 temp_9_143;
    s32 temp_9_246;
    s32 var_2_48;
    s32 var_3_46;
    s32 var_6_150;
    struct M2c_temp_12_44 *temp_12_44;
    struct M2c_temp_15_42 *temp_15_42;
    s32 *temp_2_38;
    struct M2c_temp_4_253 *temp_4_253;

    sp14 = ((u8 *)arg0 + (0x6BC));
    sp8 = arg0->unk81C;
    sp4 = arg6;
    temp_2_29 = arg0->unk810 * 0x140;
    sp0 = arg3;
    temp_10_32 = arg_sp0 >> 1;
    temp_23_36 = *(s32 *)((u8 *)sp14 + temp_2_29);
    temp_2_38 = ((u8 *)arg0 + ((temp_2_29 + 0x590)));
    temp_4_39 = temp_23_36 * 0x1C;
    temp_15_42 = temp_2_38 + (temp_4_39 + 0xB8);
    temp_12_44 = temp_2_38 + (temp_4_39 + 0x48);
    if (arg_sp10 == 0) {
        goto block_2;
    }
    var_3_46 = arg4 + arg2;
    var_2_48 = (arg_sp8 >> 1) * 2;
    goto block_3;
block_2:
    var_2_48 = arg_sp8 >> 1;
    var_3_46 = arg4 + arg2;
block_3:
    temp_2_53 = var_2_48 + arg7;
    temp_20_56 = (s32) (temp_10_32 + sp4) >> 4;
    temp_21_60 = (s32) (temp_2_53 + var_3_46) >> 4;
    temp_12_44->unk4 = (s32) ((temp_10_32 + sp4) % 16);
    sp10 = (temp_20_56 * arg1->unk10) + temp_21_60;
    temp_6_70 = (temp_2_53 + var_3_46) % 16;
    temp_12_44->unk0 = (s32) (sp8 + ((sp0 + arg4) << 5));
    temp_25_72 = arg_sp8 & 1;
    if (temp_25_72 == 0) {
        goto block_8;
    }
    if ((temp_6_70 + (arg5 << arg_sp10)) >= 0x10) {
        goto block_6;
    }
    temp_12_44->unk8 = arg5;
    goto block_12;
block_6:
    temp_2_84 = ((0x10 >> arg_sp10) - (temp_6_70 >> arg_sp10)) - 1;
    temp_12_44->unk8 = temp_2_84;
    temp_12_44->unkC = (s32) (arg5 - temp_2_84);
    goto block_13;
block_8:
    if ((temp_6_70 + (arg5 << arg_sp10)) >= 0x11) {
        goto block_10;
    }
    temp_12_44->unk8 = arg5;
    goto block_12;
block_10:
    temp_2_98 = (0x10 >> arg_sp10) - (temp_6_70 >> arg_sp10);
    temp_12_44->unk8 = temp_2_98;
    temp_12_44->unkC = (s32) (arg5 - temp_2_98);
    goto block_13;
block_12:
    temp_12_44->unkC = 0;
block_13:
    temp_9_113 = temp_6_70 * 0x10;
    sp18 = arg_sp18 * 4;
    temp_3_123 = arg_sp0 + ((u32) arg_sp0 >> 0x1F);
    temp_5_124 = arg_sp8 + ((u32) arg_sp8 >> 0x1F);
    temp_18_126 = *(s32 *)((u8 *)(((u8 *)arg0 + ((arg0->unk810 * 0x140)))) + 0x590) + (temp_23_36 * 0x600);
    temp_3_127 = temp_3_123 >> 2;
    temp_7_134 = arg4 >> 1;
    spC = sp18 | ((arg_sp0 & 1) * 2) | temp_25_72;
    temp_12_44->unk14 = (s32) (temp_18_126 + temp_9_113);
    temp_2_138 = sp4 >> 1;
    temp_12_44->unk10 = (s32) (0x10 << arg_sp10);
    temp_12_44->unk18 = (s32) (temp_18_126 + (temp_9_113 + 0x300));
    temp_9_143 = arg5 >> 1;
    if (arg_sp10 == 0) {
        goto block_15;
    }
    var_6_150 = ((temp_5_124 >> 2) * 2) + (arg7 >> 1) + (temp_7_134 + arg2);
    goto block_16;
block_15:
    var_6_150 = (temp_5_124 >> 2) + (arg7 >> 1) + (temp_7_134 + arg2);
block_16:
    temp_6_170 = var_6_150 % 8;
    temp_25_171 = (temp_5_124 >> 1) & 1;
    temp_15_42->unk4 = (s32) ((temp_3_127 + temp_2_138) % 8);
    temp_15_42->unk0 = (s32) (sp8 + (((sp0 + temp_7_134) * 0x10) + 0x200));
    if (temp_25_171 == 0) {
        goto block_21;
    }
    if ((temp_6_170 + (temp_9_143 << arg_sp10)) >= 8) {
        goto block_19;
    }
    temp_15_42->unk8 = temp_9_143;
    goto block_25;
block_19:
    temp_2_184 = ((8 >> arg_sp10) - (temp_6_170 >> arg_sp10)) - 1;
    temp_15_42->unk8 = temp_2_184;
    temp_15_42->unkC = (s32) (temp_9_143 - temp_2_184);
    goto block_26;
block_21:
    if ((temp_6_170 + (temp_9_143 << arg_sp10)) >= 9) {
        goto block_23;
    }
    temp_15_42->unk8 = temp_9_143;
    goto block_25;
block_23:
    temp_2_198 = (8 >> arg_sp10) - (temp_6_170 >> arg_sp10);
    temp_15_42->unk8 = temp_2_198;
    temp_15_42->unkC = (s32) (temp_9_143 - temp_2_198);
    goto block_26;
block_25:
    temp_15_42->unkC = 0;
block_26:
    temp_15_42->unk10 = (s32) (8 << arg_sp10);
    temp_8_220 = arg0->unk810 * 0x140;
    temp_7_229 = (temp_23_36 * 4) + temp_8_220;
    temp_4_230 = ((((((s32) (temp_3_127 + temp_2_138) >> 3) - temp_20_56) * 2) + ((var_6_150 >> 3) - temp_21_60)) * 0x180) + temp_18_126;
    temp_6_235 = temp_6_170 * 8;
    (*(s32 *)((u8 *)arg0 + temp_7_229 + 0x5B8)) = (s32) *((spC * 4) + D_00132E30);
    temp_8_242 = sp14 + temp_8_220;
    temp_9_246 = arg1->unk0;
    temp_15_42->unk14 = (s32) (temp_4_230 + (temp_6_235 + 0x100));
    temp_15_42->unk18 = (s32) (temp_4_230 + (temp_6_235 + 0x400));
    temp_4_253 = ((u8 *)arg0 + (temp_7_229));
    temp_4_253->unk598 = (s32) (temp_9_246 + (sp10 * 0x180));
    temp_4_253->unk5C8 = (s32) *(((sp18 | (((temp_3_123 >> 1) & 1) * 2) | temp_25_171) * 4) + D_00132E50);
    temp_4_253->unk5A8 = (s32) (((sp10 + arg1->unk10) * 0x180) + temp_9_246);
    *temp_8_242 += 1;
    return;
}
#endif /* NON_MATCHING */
