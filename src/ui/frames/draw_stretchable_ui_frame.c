#include "types.h"
extern s32 get_icon_frame() __asm__("FUN_001ff960");
extern s32 draw_hud_sprite() __asm__("FUN_001ffc30");
extern s32 draw_hud_sprite_flipped() __asm__("FUN_001ffe18");
void draw_stretchable_ui_frame(s32 x, s32 y, s32 width, s32 arg3,
                               s32 arg4) __asm__("FUN_00201128");

void draw_stretchable_ui_frame(s32 x, s32 y, s32 width, s32 arg3, s32 arg4) {
    s32 middle_frame;
    s32 edge_frame;

    middle_frame = get_icon_frame(0x7580, 0);
    edge_frame = get_icon_frame(0x7580, 1);
    draw_hud_sprite(edge_frame, x, y, 0x20, arg3, arg4);
    draw_hud_sprite(middle_frame, x + 0x20, y, width - 0x40, arg3, arg4);
    draw_hud_sprite_flipped(edge_frame, (x + width) - 0x20, y, 0x20, arg3, arg4);
}

extern void func_00201128(s32 x, s32 y, s32 width, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_00201128")));
