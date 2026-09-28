#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/draw_localized_three_option_menu/FUN_00222a98.s", FUN_00222a98);
#else
#include "rnc/ui_menus_draw_localized_three_option_menu_types.h"
#include "types.h"


extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern s32 func_00116248();
extern s32 func_001F4280();
extern s32 func_001F4398();
extern s32 func_001F44B8();
extern s32 func_001F61E8();
extern s32 func_001F61F8();
extern s32 func_001F6250();
extern s32 func_001F62B0();
extern s32 func_001F6AF0();
extern s32 func_001FDD10();
extern s32 func_00215290();
extern s32 func_00215300();
extern s32 func_00215348();
extern s32 func_0021F8E8();
s32 draw_localized_three_option_menu(struct M2c_arg0 *arg0) __asm__("FUN_00222a98");

s32 draw_localized_three_option_menu(struct M2c_arg0 *arg0) {
u8 sp_slot[0xC0];    s32 *var_18_74;
    s32 var_21_12;
    s32 temp_16_108;
    s32 temp_16_137;
    s32 temp_16_166;
    s32 temp_16_30;
    s32 temp_16_45;
    s32 temp_16_60;
    s32 temp_16_88;
    s32 temp_17_126;
    s32 temp_17_155;
    s32 temp_17_43;
    s32 temp_17_58;
    s32 temp_17_78;
    s32 temp_17_97;
    s32 temp_20_26;
    s32 temp_2_37;
    s32 temp_2_52;
    s32 temp_2_67;

    var_21_12 = 1;
    func_001F4280(0);
    if (5 == 0) {
        M2C_BREAK(0);
    }
    temp_20_26 = (s32) arg0->unk24 / 5;
    temp_16_30 = func_001FDD10(0x522F);
    func_00116248(sp_slot, temp_16_30, func_00215290(), 0x28);
    temp_2_37 = func_001F6250(sp_slot, -1);
    temp_17_43 = (temp_2_37 > 0) ? temp_2_37 : 0;
    temp_16_45 = func_001FDD10(0x5230, temp_2_37);
    func_00116248(sp_slot, temp_16_45, func_00215300(), 0xA);
    temp_2_52 = func_001F6250(sp_slot, -1);
    temp_17_58 = (temp_17_43 < temp_2_52) ? temp_2_52 : temp_17_43;
    temp_16_60 = func_001FDD10(0x5231, temp_2_52);
    func_00116248(sp_slot, temp_16_60, func_00215348(), 0x1E);
    temp_2_67 = func_001F6250(sp_slot, -1);
    var_18_74 = D_001DF050;
    temp_17_78 = temp_20_26 - 8;
    if (arg0->unk20 < (((temp_17_58 < temp_2_67) ? temp_2_67 : temp_17_58) + 0x18)) {
        var_21_12 = 2;
        var_18_74 = D_001DF3F0;
    }
    func_001F61F8(D_001DF050, temp_2_67);
    temp_16_88 = (s32) arg0->unk20 >> 1;
    temp_17_97 = temp_17_78 + temp_20_26;
    func_001F6AF0(temp_16_88, temp_17_78, (0x80FF << 0x10) | 0xA888, func_001FDD10(0x522E), -1);
    func_0021F8E8(0xB, temp_17_97 + 9, func_00215290() == 0x28);
    temp_16_108 = func_001FDD10(0x522F);
    func_00116248(sp_slot, temp_16_108, func_00215290(), 0x28);
    temp_17_126 = temp_17_97 + temp_20_26;
    func_001F62B0(0x14, temp_17_97, (0x80FF << 0x10) | 0xA888, sp_slot, -1, func_001F44B8(var_21_12), var_18_74);
    func_0021F8E8(0xB, temp_17_126 + 9, func_00215300() == 0xA);
    temp_16_137 = func_001FDD10(0x5230);
    func_00116248(sp_slot, temp_16_137, func_00215300(), 0xA);
    temp_17_155 = temp_17_126 + temp_20_26;
    func_001F62B0(0x14, temp_17_126, (0x80FF << 0x10) | 0xA888, sp_slot, -1, func_001F44B8(var_21_12), var_18_74);
    func_0021F8E8(0xB, temp_17_155 + 9, func_00215348() == 0x1E);
    temp_16_166 = func_001FDD10(0x5231);
    func_00116248(sp_slot, temp_16_166, func_00215348(), 0x1E);
    func_001F62B0(0x14, temp_17_155, (0x80FF << 0x10) | 0xA888, sp_slot, -1, func_001F44B8(var_21_12), var_18_74);
    func_001F61E8();
    func_001F4398();
    return 2;
}
#endif /* NON_MATCHING */
