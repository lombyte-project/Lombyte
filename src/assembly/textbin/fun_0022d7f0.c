#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022d7f0/FUN_0022d7f0.s", FUN_0022d7f0);
#else
#include "rnc/fun_0022d7f0_types.h"
#include "types.h"
















extern struct M2c_D_0013E550 D_0013E550;
extern u8 D_0013E5C0[];
extern u8 D_0013E5E0[];
extern struct M2c_D_0013F350 D_0013F350;
extern s32 func_001F99F8();
extern s32 func_00213260();
extern s32 func_0022C7E8();
s32 FUN_0022d7f0(struct M2c_arg0 *arg0, s8 arg1, struct M2c_arg2 *arg2, s64 *arg3, s32 arg4) {
    s32 temp_2_125;
    s32 temp_2_146;
    s32 temp_3_147;
    s32 temp_3_66;
    s32 var_16_43;
    s32 var_2_122;
    s32 var_2_19;
    s32 var_2_49;
    s32 var_4_25;
    s32 var_5_149;
    s8 var_18_8;
    u8 *var_3_50;
    u8 temp_3_14;
    struct M2c_temp_2_142 *temp_2_142;
    struct M2c_temp_2_69 *temp_2_69;
    struct M2c_temp_3_160 *temp_3_160;
    struct M2c_temp_4_97 *temp_4_97;

    var_18_8 = arg1;
    temp_3_14 = arg0->unk18;
    if (var_18_8 & 4) {
        goto block_3;
    }
    var_2_19 = -1;
    if (temp_3_14 != 0) {
        goto block_32;
    }
    goto block_4;
block_3:
    var_2_19 = -1;
    if (temp_3_14 == 0) {
        goto block_32;
    }
block_4:
    var_4_25 = 0x1A;
    if (arg2 == NULL) {
        goto block_9;
    }
    if (D_0013F350.unk2080 == arg2) {
        goto block_8;
    }
    if (D_0013F350.unk1090 == arg2) {
        goto block_8;
    }
    if (arg2->unkA6 != 0x472) {
        goto block_9;
    }
block_8:
    var_4_25 = 0x1E;
block_9:
    var_16_43 = 0;
    if (var_4_25 == 0) {
        goto block_30;
    }
    var_2_49 = var_4_25 > 0;
    if (D_0013E550.unk74 == 0) {
        goto block_17;
    }
    var_3_50 = ((u8 *)&D_0013E550 + 0x74);
    var_16_43 = 1;
loop_12:
    var_3_50 += 0x70;
    if (var_16_43 >= var_4_25) {
        goto block_30;
    }
    if (*var_3_50 == 0) {
        goto block_15;
    }
    var_16_43 += 1;
    goto loop_12;
block_15:
    var_2_49 = var_16_43 < var_4_25;
block_17:
    if (var_2_49 == 0) {
        goto block_30;
    }
    temp_3_66 = var_16_43 * 0x70;
    temp_2_69 = temp_3_66 + &D_0013E550;
    temp_2_69->unk78 = arg0;
    temp_2_69->unk7E = -1;
    temp_2_69->unk7C = (u16) arg0->unk1A;
    temp_2_69->unk8C = 0;
    temp_2_69->unk80 = arg4;
    temp_2_69->unk88 = 0;
    *(temp_3_66 + (((u8 *)&D_0013E550 + 0xA0))) = 0;
    if (arg3 != NULL) {
        goto block_20;
    }
    if (arg2 == NULL) {
        goto block_23;
    }
block_20:
    if (arg2 == NULL) {
        goto block_22;
    }
    *((var_16_43 * 0x70) + D_0013E5E0) = arg2->unk10;
    temp_4_97 = (var_16_43 * 0x70) + (((u8 *)D_0013E5E0 - (u8 *)0x90));
    temp_4_97->unk98 = (f32) (temp_4_97->unk98 + 1.0f);
    goto block_24;
block_22:
    *((var_16_43 * 0x70) + D_0013E5E0) = *arg3;
    goto block_24;
block_23:
    var_18_8 |= 0x11;
    func_001F99F8((var_16_43 * 0x70) + D_0013E5E0, -1);
block_24:
    var_2_122 = arg4;
    if (var_18_8 & 0x10) {
        goto block_26;
    }
    temp_2_125 = var_16_43 * 0x70;
    var_2_122 = func_0022C7E8(temp_2_125 + D_0013E5C0, temp_2_125 + (D_0013E5C0 + 0x20));
block_26:
    var_2_19 = -1;
    if (var_2_122 < 0x20) {
        goto block_32;
    }
    temp_2_142 = (var_16_43 * 0x70) + &D_0013E550;
    temp_2_142->unk75 = var_18_8;
    temp_2_142->unk74 = 7;
    temp_2_142->unkB0 = 0;
    temp_2_146 = arg0->unk14;
    temp_3_147 = arg0->unk10;
    var_5_149 = temp_2_146;
    if (temp_2_146 == temp_3_147) {
        goto block_29;
    }
    var_5_149 = func_00213260(temp_2_146 - temp_3_147, var_5_149) + arg0->unk10;
block_29:
    temp_3_160 = (var_16_43 * 0x70) + &D_0013E550;
    temp_3_160->unk70 = 0xFFFFFFFF;
    temp_3_160->unk84 = var_5_149;
    goto block_31;
block_30:
    var_16_43 = -1;
block_31:
    var_2_19 = var_16_43;
block_32:
    return var_2_19;
}
#endif /* NON_MATCHING */
