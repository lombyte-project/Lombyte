#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/inventory/draw_items_menu/FUN_0021eb20.s", FUN_0021eb20);
#else
#include "rnc/ui_menus_inventory_draw_items_menu_types.h"
#include "types.h"






extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_0015ED88[];
extern s32 D_001601B8;
extern s32 D_001601BC;
extern u8 D_001602A0[];
extern s32 func_001F4280();
extern s32 func_001F4398();
extern s32 func_001F6940();
extern s32 func_001F6A60();
extern s32 func_001F7580();
extern s32 func_001FDD10();
extern s32 func_00200E08();
extern s32 func_0020D330();
extern s32 func_00215248();
extern s32 func_00215290();
extern s32 func_00215300();
extern s32 memset();
extern s32 sprintf();
extern s32 strchr();
extern s32 strcpy();
extern s32 strlen();
s32 draw_items_menu(struct M2c_arg0 *arg0) __asm__("FUN_0021eb20");

s32 draw_items_menu(struct M2c_arg0 *arg0) {    u16 sp84;
u8 sp_slot[0x110];    u16 sp86;
    u16 sp8E;
    s32 sp80;
    s16 sp88;
    s16 sp8A;
    u16 sp92;
    s32 spA0;
    u16 spA2;
    s16 spA4;
    s16 spA6;
    s16 spB0;
    s16 spB2;
    s32 temp_16_124;
    s32 temp_16_137;
    s32 temp_16_150;
    s32 temp_17_122;
    s32 temp_17_135;
    s32 temp_17_148;
    s32 temp_4_10;
    s32 var_3_74;
    u32 temp_2_67;
    u8 *var_4_75;
    struct M2c_var_5_79 *var_5_79;

    temp_4_10 = arg0->unk44;
    if (temp_4_10 == 0) {
        goto block_2;
    }
    func_0020D330(temp_4_10, 1);
block_2:
    func_001F4280(0);
    memset(&spA0, 0, 0x18);
    spA4 = 8;
    spA2 = arg0->unk24;
    spB0 = 0x10;
    spB2 = 5;
    spA6 = (s16) ((s32) arg0->unk20 / 3);
    /* m2c-unknown:  unknown instruction: ldl $v0, 0xa7($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $v0, 0xa0($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldl $v1, 0xaf($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $v1, 0xa8($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldl $a1, 0xb7($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $a1, 0xb0($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $v0, 0x87($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdr $v0, 0x80($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $v1, 0x8f($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdr $v1, 0x88($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $a1, 0x97($sp_slot)  */
    sp88 = (s16) ((s32) (sp84 + sp86) >> 1);
    strcpy(sp_slot, func_001FDD10(0x4F4E, 0 /*  unknown instruction: sdr $a1, 0x90($sp_slot)  */));
    if (D_0015ED88[0] != 3) {
        goto block_8;
    }
    temp_2_67 = strchr(sp_slot, 0x2D);
    if (temp_2_67 == 0) {
        goto block_9;
    }
    var_3_74 = strlen(sp_slot);
    var_4_75 = sp_slot + var_3_74;
    if (temp_2_67 >= (u32) var_4_75) {
        goto block_7;
    }
    var_5_79 = var_4_75;
loop_6:
    var_5_79->unk1 = (u8) *var_4_75;
    var_5_79 -= 1;
    var_4_75 = var_5_79;
    var_3_74 -= 1;
    if (temp_2_67 < (u32) var_4_75) {
        goto loop_6;
    }
block_7:
    *(s32 *)((u8 *)(var_3_74 + sp_slot) + 0x1) = 0x20;
block_8:
block_9:
    func_001F7580(&sp80, (0x8000 << 0x10) | 0xC0C0, sp_slot, -1);
    sp92 ^= 4;
    sp8A = (s16) ((s32) (D_0013E500.unk4 - sp8E) >> 1);
    func_001F7580(&sp80, (0x8000 << 0x10) | 0xC0C0, sp_slot, -1);
    temp_17_122 = D_001601B8 + 0xC8;
    temp_16_124 = D_001601BC + 0x1D;
    func_001F6940(temp_17_122, temp_16_124, 0x8000 << 0x10, func_001FDD10(0x4F4F), -1);
    temp_17_135 = D_001601B8 + 0xC8;
    temp_16_137 = D_001601BC + 0x36;
    func_001F6940(temp_17_135, temp_16_137, 0x8000 << 0x10, func_001FDD10(0x4F50), -1);
    temp_17_148 = D_001601B8 + 0xC8;
    temp_16_150 = D_001601BC + 0x54;
    func_001F6940(temp_17_148, temp_16_150, 0x8000 << 0x10, func_001FDD10(0x4F51), -1);
    func_001F6940(0xC8, 0x1D, (0x80FF << 0x10) | 0xA888, func_001FDD10(0x4F4F), -1);
    func_001F6940(0xC8, 0x36, (0x80FF << 0x10) | 0xA888, func_001FDD10(0x4F50), -1);
    func_001F6940(0xC8, 0x54, (0x80FF << 0x10) | 0xA888, func_001FDD10(0x4F51), -1);
    sprintf(sp_slot, D_001602A0, func_00215290());
    func_001F6A60(D_001601B8 + 0xF0, D_001601BC + 0x1D, 0x8000 << 0x10, sp_slot, -1);
    func_001F6A60(0xF0, 0x1D, (0x80FF << 0x10) | 0xA888, sp_slot, -1);
    sprintf(sp_slot, D_001602A0, func_00215300() * 4);
    func_001F6A60(D_001601B8 + 0xF0, D_001601BC + 0x36, 0x8000 << 0x10, sp_slot, -1);
    func_001F6A60(0xF0, 0x36, (0x80FF << 0x10) | 0xA888, sp_slot, -1);
    sprintf(sp_slot, D_001602A0, func_00215248());
    func_001F6A60(D_001601B8 + 0xF0, D_001601BC + 0x54, 0x8000 << 0x10, sp_slot, -1);
    func_001F6A60(0xF0, 0x54, (0x80FF << 0x10) | 0xA888, sp_slot, -1);
    func_00200E08(D_001601B8 + 0xD0, D_001601BC + 0x4D, D_001601B8 + 0xF2, D_001601BC + 0x50, 0x8000 << 0x10, 0);
    func_00200E08(0xD0, 0x4D, 0xF2, 0x50, (0x80FF << 0x10) | 0xA888, 0);
    func_001F4398();
    return 8;
}
#endif /* NON_MATCHING */
