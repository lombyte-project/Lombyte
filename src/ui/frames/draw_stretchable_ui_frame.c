#include "types.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite_flipped.h"

void draw_stretchable_ui_frame(s32 x, s32 y, s32 width, s32 height, s32 alpha) __asm__("FUN_00201128");

void draw_stretchable_ui_frame(s32 x, s32 y, s32 width, s32 height, s32 alpha) {
    s32 middle_frame;
    s32 edge_frame;

    middle_frame = get_icon_frame(0x7580, 0);
    edge_frame = get_icon_frame(0x7580, 1);
    draw_hud_sprite(edge_frame, x, y, 0x20, height, alpha);
    draw_hud_sprite(middle_frame, x + 0x20, y, width - 0x40, height, alpha);
    draw_hud_sprite_flipped(edge_frame, (x + width) - 0x20, y, 0x20, height, alpha);
}

extern void func_00201128(s32 x, s32 y, s32 width, s32 arg3, s32 arg4)
    __attribute__((alias("FUN_00201128")));
