#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_rect_overlay/FUN_001f52a0.s", FUN_001f52a0);
#else
#include "rnc/rendering_draw_rect_overlay_types.h"
#include "types.h"
















extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_00160820;
extern s32 D_00160830;
extern struct M2c_D_00160F00 *D_00160F00;
void draw_rect_overlay(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s64 arg4) __asm__("FUN_001f52a0");

void draw_rect_overlay(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s64 arg4) {
    s32 temp_4_51;
    s32 temp_5_73;
    s32 temp_6_52;
    s32 temp_7_59;
    s64 temp_11_57;
    struct M2c_temp_10_67 *temp_10_67;
    struct M2c_temp_11_22 *temp_11_22;
    struct M2c_temp_11_41 *temp_11_41;
    struct M2c_temp_2_34 *temp_2_34;
    struct M2c_temp_3_35 *temp_3_35;
    struct M2c_temp_8_62 *temp_8_62;

    D_00160F00->unk0 = 0x10000005;
    D_00160F00->unk4 = 0;
    D_00160F00->unk8 = 0;
    D_00160F00->unkC = 0x50000005;
    temp_11_22 = D_00160F00;
    D_00160F00 = ((u8 *)temp_11_22 + (0x10));
    temp_11_22->unk10 = (s64) D_00160820;
    temp_11_22->unk10 = -0x7FFF;
    temp_2_34 = D_00160F00;
    temp_3_35 = ((u8 *)temp_2_34 + (0x10));
    D_00160F00 = temp_3_35;
    temp_2_34->unk10 = 0x144;
    temp_3_35->unk8 = arg4;
    temp_11_41 = D_00160F00;
    D_00160F00 = ((u8 *)temp_11_41 + (0x10));
    temp_11_41->unk10 = (s64) D_00160830;
    temp_11_41->unk10 = -0x7FFC;
    temp_4_51 = arg0 * 0x10;
    temp_6_52 = arg2 * 0x10;
    temp_11_57 = ((0xFFFF << 0x10) | 0xF000) << 0x18;
    temp_7_59 = arg3 * 0x10;
    temp_8_62 = D_00160F00;
    temp_10_67 = ((u8 *)temp_8_62 + (0x10));
    D_00160F00 = temp_10_67;
    temp_8_62->unk10 = (s64) (((temp_6_52 + D_0013E500.unk10) - 8) | (((temp_4_51 + D_0013E500.unk14) - 8) << 0x10) | temp_11_57);
    temp_5_73 = arg1 * 0x10;
    temp_10_67->unk8 = (s64) (((temp_7_59 + D_0013E500.unk10) - 8) | (((temp_4_51 + D_0013E500.unk14) - 8) << 0x10) | temp_11_57);
    temp_10_67->unk10 = (s64) (((temp_6_52 + D_0013E500.unk10) - 8) | (((temp_5_73 + D_0013E500.unk14) - 8) << 0x10) | temp_11_57);
    temp_10_67->unk18 = (s64) (((temp_7_59 + D_0013E500.unk10) - 8) | (((temp_5_73 + D_0013E500.unk14) - 8) << 0x10) | temp_11_57);
    D_00160F00 += 0x20;
}
#endif /* NON_MATCHING */
