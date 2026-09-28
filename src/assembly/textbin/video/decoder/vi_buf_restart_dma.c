#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/vi_buf_restart_dma/FUN_0023c280.s", FUN_0023c280);
#else
#include "rnc/video_decoder_vi_buf_restart_dma_types.h"
#include "types.h"


extern s32 func_00118990();
extern s32 func_001189B0();
extern s32 func_0023BAF8();
extern s32 func_0023BB40();
extern s32 func_0023BBB0();
s32 vi_buf_restart_dma(struct M2c_arg0 *arg0) __asm__("FUN_0023c280");

s32 vi_buf_restart_dma(struct M2c_arg0 *arg0) {
    s32 temp_12_89;
    s32 temp_13_101;
    s32 temp_17_82;
    s32 temp_2_23;
    s32 temp_2_84;
    s32 temp_3_141;
    s32 temp_4_99;
    s32 temp_5_24;
    s32 temp_5_36;
    s32 temp_6_22;
    s32 temp_6_39;
    s32 temp_hi_119;
    s32 temp_hi_59;
    s32 var_19_29;
    s32 var_20_31;
    s32 var_2_77;
    s32 var_4_46;
    u32 temp_2_15;
    u32 temp_7_16;
    u32 temp_7_32;
    u32 temp_8_90;
    u32 temp_9_41;
    u32 var_18_27;
    u32 var_21_28;

    temp_2_15 = arg0->unk38;
    temp_7_16 = arg0->unk1C;
    temp_6_22 = arg0->unk24;
    temp_2_23 = ((temp_2_15 >> 0x10) & 3) + ((temp_2_15 >> 8) & 0xF);
    temp_5_24 = arg0->unk28;
    var_18_27 = temp_7_16 - (temp_2_23 * 0x10);
    var_21_28 = temp_6_22 + temp_2_23;
    var_19_29 = temp_5_24 | 0x100;
    var_20_31 = arg0->unk20;
    func_001189B0(arg0->unk40, temp_5_24, temp_6_22, temp_7_16);
    temp_7_32 = arg0->unk0;
    if (var_18_27 >= temp_7_32) {
        goto block_6;
    }
    temp_5_36 = arg0->unk8;
    temp_6_39 = temp_5_36 << 0xB;
    temp_9_41 = arg0->unk1C;
    var_21_28 = (u32) (temp_7_32 - var_18_27) >> 4;
    var_20_31 = arg0->unk4 & 0x0FFFFFFF;
    var_18_27 += temp_6_39;
    var_4_46 = 0;
    if (temp_9_41 == temp_7_32) {
        goto block_3;
    }
    var_4_46 = ((temp_9_41 ^ (temp_7_32 + temp_6_39)) == 0) ? 0 : 3;
block_3:
    temp_hi_59 = (s32) (temp_5_36 - arg0->unkC) % temp_5_36;
    var_19_29 = (arg0->unk28 & 0x0FFFFFFF) | (var_4_46 << 0x1C) | 0x100;
    if (temp_hi_59 < 0) {
        goto block_5;
    }
    if (temp_hi_59 < arg0->unk10) {
        goto block_11;
    }
block_5:
    arg0->unkC = (s32) (temp_5_36 - 1);
    var_2_77 = arg0->unk10 + 1;
    goto block_10;
block_6:
    temp_17_82 = func_0023BAF8(arg0, arg0->unk1C);
    temp_2_84 = func_0023BAF8(arg0, var_18_27);
    if (temp_17_82 == temp_2_84) {
        goto block_11;
    }
    temp_12_89 = arg0->unk8;
    temp_8_90 = arg0->unk0;
    temp_4_99 = arg0->unkC;
    temp_13_101 = arg0->unk10;
    var_21_28 = (u32) ((temp_8_90 + (temp_17_82 << 0xB)) - var_18_27) >> 4;
    var_20_31 = ((temp_17_82 * 0x10) + arg0->unk4) & 0x0FFFFFFF;
    temp_hi_119 = (s32) ((temp_2_84 + temp_12_89) - temp_4_99) % temp_12_89;
    var_19_29 = (arg0->unk28 & 0x0FFFFFFF) | (((((temp_8_90 + ((u32) (arg0->unk1C - temp_8_90) % (u32) (temp_12_89 << 0xB))) ^ (temp_8_90 + (((s32) (temp_4_99 + temp_13_101) % temp_12_89) << 0xB))) == 0) ? 0 : 3) << 0x1C) | 0x100;
    if (temp_hi_119 < 0) {
        goto block_9;
    }
    if (temp_hi_119 < temp_13_101) {
        goto block_11;
    }
block_9:
    var_2_77 = temp_13_101 + 1;
    arg0->unk10 = var_2_77;
block_10:
    arg0->unkC = temp_2_84;
block_11:
    temp_3_141 = arg0->unk2C;
    if (temp_3_141 == 0) {
        goto block_14;
    }
    if (arg0->unk30 == 0) {
        goto block_14;
    }
    *(s32 *)0x1000B010 = temp_3_141;
    *(s32 *)0x1000B020 = arg0->unk30;
    func_0023BB40(arg0->unk34 | 0x100);
block_14:
    if (arg0->unk10 == 0) {
        goto block_21;
    }
    if (*(s32 *)0x10002010 >= 0) {
        goto block_18;
    }
loop_17:
    if (*(volatile u32 *)0x10002010 < 0) {
        goto loop_17;
    }
block_18:
    *(s32 *)0x10002000 = temp_2_15 & 0x7F;
    if (*(volatile u32 *)0x10002010 >= 0) {
        goto block_22;
    }
loop_20:
    if (*(volatile u32 *)0x10002010 < 0) {
        goto loop_20;
    }
block_21:
block_22:
    *(u32 *)0x1000B410 = var_18_27;
    *(s32 *)0x1000B430 = var_20_31;
    *(u32 *)0x1000B420 = var_21_28;
    if (arg0->unk10 == 0) {
        goto block_24;
    }
    func_0023BBB0(var_19_29);
block_24:
    *(volatile u32 *)0x10002010 = (s32) arg0->unk3C;
    arg0->unk44 = 1;
    func_00118990(arg0->unk40);
    return 1;
}
#endif /* NON_MATCHING */
