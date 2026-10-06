#include "types.h"
extern s32 find_valid_animation_frame_index() __asm__("FUN_001ff960");
extern s32 draw_hud_sprite() __asm__("FUN_001ffc30");
extern s32 draw_hud_sprite_flipped() __asm__("FUN_001ffe18");
void draw_stretchable_ui_frame(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                               s32 arg4) __asm__("FUN_00201128");

void draw_stretchable_ui_frame(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_21_20;
    s32 temp_2_22;

    temp_21_20 = find_valid_animation_frame_index(0x7580, 0);
    temp_2_22 = find_valid_animation_frame_index(0x7580, 1);
    draw_hud_sprite(temp_2_22, arg0, arg1, 0x20, arg3, arg4);
    draw_hud_sprite(temp_21_20, arg0 + 0x20, arg1, arg2 - 0x40, arg3, arg4);
    draw_hud_sprite_flipped(temp_2_22, (arg0 + arg2) - 0x20, arg1, 0x20, arg3, arg4);
}

extern void func_00201128(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_00201128")));
