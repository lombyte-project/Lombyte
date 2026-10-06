#include "types.h"
extern s32 D_001601B0;
extern s32 get_icon_frame() __asm__("func_001FF960");
extern s32 draw_hud_sprite() __asm__("func_001FFC30");
extern s32 append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
void draw_menu_selection_marker(s32 x, s32 y, s32 arg2) __asm__("FUN_0021f8e8");

void draw_menu_selection_marker(s32 x, s32 y, s32 arg2) {
    append_screen_rect_packet(x - 5, y - 5, x + 5, y + 5, (u64)0x80FFA888, 0);
    append_screen_rect_packet(x - 4, y - 4, x + 4, y + 4, (u64)D_001601B0, 0);
    if (arg2 != 0) {
        draw_hud_sprite(get_icon_frame(0xE99E, 1), x - 0xD, y - 0x12, 0x1E,
                        0x1E, 0x80);
    }
}

extern void func_0021F8E8(void) __attribute__((alias("FUN_0021f8e8")));
