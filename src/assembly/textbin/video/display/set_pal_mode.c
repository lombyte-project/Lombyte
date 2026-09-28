#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/display/set_pal_mode/FUN_001f34e8.s", FUN_001f34e8);
#else
#include "rnc/video_display_set_pal_mode_types.h"
#include "types.h"






extern struct M2c_D_0013CF10 D_0013CF10;
extern u8 D_0013D100[];
extern s32 D_0013D170;
extern struct M2c_D_0013E500 D_0013E500;
extern struct M2c_D_00151780 D_00151780;
extern s32 D_0015ED80;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE80;
extern s32 D_0015EE84;
extern s32 D_0015EE88;
extern s32 D_0015EE8C;
extern u8 D_001941C0[];
extern void func_00118A80();
extern s32 func_00120558();
extern s32 func_00122330();
extern s32 func_00122658();
extern s32 func_001F97E8();
extern s32 func_001FA978();
extern s32 func_001FB2A8();
extern s32 func_001FB2D0();
extern s32 func_001FB368();
void set_pal_mode(void) __asm__("FUN_001f34e8");

void set_pal_mode(void) {
u8 sp_slot[0xD0];    s32 temp_10_68;
    s32 temp_16_164;
    s32 temp_5_80;
    s32 temp_6_77;
    s32 temp_7_79;
    s32 temp_8_78;
    s32 temp_9_70;
    s32 var_20_85;
    s32 var_5_167;
    s64 temp_10_114;
    s64 temp_11_132;
    s64 temp_5_108;
    s64 temp_7_121;
    s64 temp_8_120;
    s64 temp_9_129;

    func_00118A80(0);
    if (D_0015ED80 != 0) {
        D_0015EE8C = 0x2C0000;
        D_0015EE84 = 0x100000;
        D_0015EE88 = 0x1E0000;
        D_0015EE80 = 0;
        func_001FA978(0x200, 0x1C0, 0x200, 0x200, 4, 0);
    } else {
        D_0015EE8C = 0x280000;
        D_0015EE84 = 0xE0000;
        D_0015EE88 = 0x1B0000;
        D_0015EE80 = 0;
        func_001FA978(0x200, 0x1A0, 0x200, 0x1C0, 0, 0);
    }
    temp_10_68 = (s32) (D_00151780.unk152 << 0x10) >> 0x11;
    temp_9_70 = (s32) (D_00151780.unk150 << 0x10) >> 0x11;
    D_0013E500.unk0 = (s32) (s16) D_00151780.unk150;
    temp_6_77 = (0x800 - temp_9_70) * 0x10;
    temp_8_78 = (temp_9_70 + 0x800) * 0x10;
    temp_7_79 = (temp_10_68 + 0x800) * 0x10;
    temp_5_80 = (0x800 - temp_10_68) * 0x10;
    D_0013E500.unk10 = temp_6_77;
    D_0013E500.unk18 = temp_8_78;
    var_20_85 = 0;
    D_0013E500.unk1C = temp_7_79;
    D_0013E500.unk8 = temp_9_70;
    D_0013E500.unkC = temp_10_68;
    D_0013E500.unk4 = (s32) (s16) D_00151780.unk152;
    D_0013E500.unk14 = temp_5_80;
    func_00118A80(0, temp_5_80, temp_6_77, temp_7_79, temp_8_78, temp_9_70, temp_10_68);
    func_00120558(0, 0);
    temp_5_108 = ((s32) D_0015EE88 >> 0xD) | 0x01000000;
    temp_10_114 = ((D_0013E500.unk0 - 1) << 0x10) | ((D_0013E500.unk4 - 1) << 0x30);
    D_0013CF10.unk80 = temp_10_114;
    temp_8_120 = (s64) D_0013E500.unk14 << 0x20;
    temp_7_121 = (s64) D_0013E500.unk14 << 0x20;
    D_0015EE78 = D_0015EE8C;
    temp_9_129 = ((s64) D_0015EE84 >> 0xD) | (((s64) D_0013E500.unk0 >> 6) << 0x10);
    D_0013D170 = temp_5_108 | (0x8000 << 0x11);
    temp_11_132 = D_0013E500.unk10 | temp_7_121;
    D_0013CF10.unk20 = temp_9_129;
    D_0013CF10.unk50 = (s64) (D_0013E500.unk10 | temp_8_120);
    D_0013CF10.unk60 = temp_11_132;
    *(s32 *)D_0013D100 = temp_5_108;
    D_0015EE74 = D_0015EE8C;
    D_0013CF10.unk30 = temp_5_108;
    D_0013CF10.unk40 = temp_5_108;
    D_0013CF10.unk10 = temp_9_129;
    D_0013CF10.unk70 = temp_10_114;
    func_00118A80(0, (s32) temp_5_108, (s32) D_0013D100, (s32) temp_7_121, (s32) temp_8_120, (s32) temp_9_129, (s32) temp_10_114, temp_11_132);
    func_001FB2D0();
    func_001FB368();
    func_00118A80(0);
    func_00120558(0, 0);
    func_001FB2A8();
    func_001F97E8(D_001941C0, 0, 0x1000);
    temp_16_164 = (s32) (D_00151780.unk158 * D_00151780.unk15A) >> 0xA;
    if (temp_16_164 > 0) {
        var_5_167 = 0 << 0x14;
loop_5:
        var_20_85 += 1;
        func_00122330(sp_slot, var_5_167 >> 0x10, 1, 0, 0, 0, 0x20, 0x20);
        func_00118A80(0);
        func_00122658(sp_slot, D_001941C0);
        func_00120558(0, 0);
        if (var_20_85 < temp_16_164) {
            var_5_167 = var_20_85 << 0x14;
            goto loop_5;
        }
    }
}
#endif /* NON_MATCHING */
