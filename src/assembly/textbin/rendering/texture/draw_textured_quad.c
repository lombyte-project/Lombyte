#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/texture/draw_textured_quad/FUN_001f5450.s", FUN_001f5450);
#else
#include "rnc/rendering_texture_draw_textured_quad_types.h"
#include "types.h"








extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_00160840;
extern struct M2c_D_00160F00 *D_00160F00;
void draw_textured_quad(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s64 arg_sp0, s64 arg_sp8) __asm__("FUN_001f5450");

void draw_textured_quad(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s64 arg_sp0, s64 arg_sp8) {    s32 arg_sp0;
    s32 arg_sp8;
    s32 temp_10_36;
    s32 temp_11_57;
    s32 temp_4_39;
    s32 temp_6_40;
    s32 temp_8_37;
    s32 temp_9_61;
    s64 temp_2_68;
    s64 temp_5_60;
    s64 temp_7_55;
    struct M2c_temp_13_49 *temp_13_49;
    struct M2c_temp_3_58 *temp_3_58;

    D_00160F00->unk0 = 0x10000007;
    D_00160F00->unk4 = 0;
    temp_10_36 = (arg4 + arg6) * 0x10;
    temp_8_37 = arg4 * 0x10;
    D_00160F00->unk8 = 0;
    temp_4_39 = ((arg0 * 0x10) + D_0013E500.unk10) - 8;
    temp_6_40 = (((arg0 + arg2) * 0x10) + D_0013E500.unk10) - 8;
    D_00160F00->unkC = 0x50000007;
    temp_13_49 = D_00160F00;
    D_00160F00 = ((u8 *)temp_13_49 + (0x10));
    temp_13_49->unk10 = (s64) D_00160840;
    temp_7_55 = ((((arg1 + arg3) * 0x10) + D_0013E500.unk14) - 8) << 0x10;
    temp_11_57 = (arg5 + arg7) << 0x14;
    temp_3_58 = ((u8 *)temp_13_49 + (0x20));
    temp_5_60 = (((arg1 * 0x10) + D_0013E500.unk14) - 8) << 0x10;
    temp_9_61 = arg5 << 0x14;
    D_00160F00 = temp_3_58;
    temp_2_68 = ((0xFFFF << 0x10) | 0xF000) << 0x18;
    temp_13_49->unk20 = arg_sp8;
    temp_3_58->unk10 = arg_sp0;
    temp_3_58->unk8 = 0x154;
    temp_3_58->unk18 = (s64) (temp_9_61 + temp_8_37);
    temp_3_58->unk20 = (s64) (temp_4_39 | temp_5_60 | temp_2_68);
    temp_3_58->unk28 = (s64) (temp_9_61 + temp_10_36);
    temp_3_58->unk30 = (s64) (temp_6_40 | temp_5_60 | temp_2_68);
    temp_3_58->unk38 = (s64) (temp_11_57 + temp_8_37);
    temp_3_58->unk40 = (s64) (temp_4_39 | temp_7_55 | temp_2_68);
    temp_3_58->unk48 = (s64) (temp_11_57 + temp_10_36);
    temp_3_58->unk50 = (s64) (temp_6_40 | temp_7_55 | temp_2_68);
    temp_3_58->unk58 = 0;
    D_00160F00 += 0x60;
}
#endif /* NON_MATCHING */
